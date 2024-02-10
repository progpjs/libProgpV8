/*
 * (C) Copyright 2024 Johan Michel PIQUET, France (https://johanpiquet.fr/).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package libProgpV8

import "C"

// #cgo CXXFLAGS: -fno-rtti -fPIC -std=c++17 -DV8_COMPRESS_POINTERS -DV8_31BIT_SMIS_ON_64BIT_ARCH -I${SRCDIR}/include -I${SRCDIR}/include/v8 -I${SRCDIR}/include/boost -Wall -DV8_ENABLE_SANDBOX
// #cgo LDFLAGS: -pthread -lv8
// #cgo darwin,arm64 LDFLAGS: -L${SRCDIR}/libs/darwin_arm64
// #cgo linux,amd64 LDFLAGS: -L${SRCDIR}/libs/linux_x86_64 -ldl
// #cgo linux,arm64 LDFLAGS: -L${SRCDIR}/libs/linux_arm64 -ldl
//
// #include <stdlib.h> // For C.free
// #include "progpAPI.h"
//
import "C"

import (
	"encoding/json"
	"errors"
	"fmt"
	_ "github.com/progpjs/libProgpV8/libs/darwin_arm64"
	_ "github.com/progpjs/libProgpV8/libs/linux_arm64"
	_ "github.com/progpjs/libProgpV8/libs/linux_x86_64"
	"github.com/progpjs/progpAPI"
	"reflect"
	"runtime"
	"sync"
	"unsafe"
)

//region V8Engine

type V8Engine struct {
	isStarted bool
}

var gProgpV8Engine *V8Engine

func RegisterEngine() {
	progpAPI.ConfigRegisterScriptEngineBuilder("progpV8",
		func() progpAPI.ScriptEngine {
			return getProgpV8Engine()
		})
}

func getProgpV8Engine() *V8Engine {
	if gProgpV8Engine == nil {
		gTaskQueue = progpAPI.NewTaskQueue()
		gProgpV8Engine = &V8Engine{}
		gSizeOfAnyValueStruct = int(C.progp_GetSizeOfAnyValueStruct())
	}

	return gProgpV8Engine
}

func (m *V8Engine) Start() {
	if m.isStarted {
		return
	}
	m.isStarted = true

	C.progp_StartupEngine()
}

func (m *V8Engine) GetEngineLanguage() string {
	return "javascript"
}

func (m *V8Engine) GetEngineName() string {
	return "progpV8"
}

func (m *V8Engine) GetInternalEngineVersion() string {
	return C.GoString(C.progp_GetV8EngineVersion())
}

func (m *V8Engine) WaitDebuggerReady() {
	C.progp_WaitDebuggerReady()
}

func (m *V8Engine) AddDraftFunction(functionName string) {
	cFunctionName := C.CString(functionName)
	defer C.free(unsafe.Pointer(cFunctionName))
	C.progp_DeclareDynamicFunction(cFunctionName)
}

func (m *V8Engine) ExecuteScript(scriptContent string, compiledFilePath string) *progpAPI.ScriptErrorMessage {
	progpAPI.DeclareBackgroundTaskStarted()
	defer progpAPI.DeclareBackgroundTaskEnded()

	gCurrentScriptMutex.Lock()
	gCurrentScriptCallerIsWaiting = true

	gTaskQueue.Push(func() {
		gCurrentScriptPath = compiledFilePath
		gLastErrorMessage = nil

		cScriptContent := C.CString(scriptContent)
		defer C.free(unsafe.Pointer(cScriptContent))

		cCompiledFilePath := C.CString(compiledFilePath)
		defer C.free(unsafe.Pointer(cCompiledFilePath))

		C.progp_ExecuteScript(cScriptContent, cCompiledFilePath)
	})

	gCurrentScriptMutex.Lock()

	err := gLastErrorMessage
	gLastErrorMessage = nil
	return err
}

func (m *V8Engine) DisarmError(error *progpAPI.ScriptErrorMessage) {
	if gLastErrorMessage == error {
		gLastErrorMessage = nil
	}
}

func (m *V8Engine) Shutdown() {
	if gTaskQueue.IsDisposed() {
		// Already shutdown, then quit.
		return
	}

	gShutdownMutex.Lock()
	defer gShutdownMutex.Unlock()

	if gTaskQueue.IsDisposed() {
		// Already shutdown, then quit.
		return
	}

	gTaskQueue.Dispose()

	freeCurrentScriptCaller()
}

func asScriptErrorMessage(ptr *C.s_progp_v8_errorMessage) *progpAPI.ScriptErrorMessage {
	m := progpAPI.ScriptErrorMessage{ScriptEngine: gProgpV8Engine}

	m.ScriptPath = gCurrentScriptPath

	m.Error = C.GoString(ptr.error)
	m.ErrorLevel = int(ptr.errorLevel)

	m.StartColumn = int(ptr.startColumn)
	m.EndColumn = int(ptr.endColumn)

	m.StartPosition = int(ptr.startPosition)
	m.EndPosition = int(ptr.endPosition)

	m.SourceMapUrl = C.GoString(ptr.sourceMapUrl)

	m.StackTraceFrameCount = int(ptr.stackTraceFrameCount)
	frameArrayPtr := ptr.stackTraceFrameArray
	sizeOfFrameType := int(ptr.stackTraceFrameSize)

	var frames []progpAPI.StackTraceFrame

	for i := 0; i < m.StackTraceFrameCount; i++ {
		frame := progpAPI.StackTraceFrame{}
		frame.Line = int(frameArrayPtr.line)
		frame.Column = int(frameArrayPtr.column)
		frame.Source = C.GoString(frameArrayPtr.source)
		frame.Function = C.GoString(frameArrayPtr.function)

		frames = append(frames, frame)

		frameArrayPtr = (*C.s_progp_v8_stackTraceFrame)(unsafe.Add(unsafe.Pointer(frameArrayPtr), sizeOfFrameType))
	}

	m.StackTraceFrames = frames
	return &m
}

func freeCurrentScriptCaller() {
	if !gCurrentScriptCallerIsWaiting {
		return
	}

	gCurrentScriptCallerIsWaiting = false
	gCurrentScriptMutex.Unlock()
}

var gCurrentScriptMutex sync.Mutex
var gCurrentScriptCallerIsWaiting bool

var gCurrentScriptPath string
var gLastErrorMessage *progpAPI.ScriptErrorMessage
var gTaskQueue *progpAPI.TaskQueue
var gShutdownMutex sync.Mutex
var gSizeOfAnyValueStruct int
var gFunctionRegistry = progpAPI.GetFunctionRegistry()

const cInt0 = C.int(0)
const cInt1 = C.int(1)

//endregion

//region V8Function

type v8Function struct {
	isAsync             C.int
	mustDisposeFunction C.int
	functionPtr         C.ProgpV8FunctionPtr
}

func newV8Function(isAsync C.int, ptr C.ProgpV8FunctionPtr) progpAPI.ScriptFunction {
	res := new(v8Function)
	res.functionPtr = ptr
	res.isAsync = isAsync

	// Will be automatically disposed on first call.
	res.mustDisposeFunction = cInt1

	// Will allow to automatically dispose the C.ProgpV8FunctionPtr.
	runtime.SetFinalizer(res, (*v8Function).finalizer)

	return res
}

func (m *v8Function) finalizer() {
	if m.functionPtr != nil {
		C.free(unsafe.Pointer(m.functionPtr))
		m.functionPtr = nil
	}
}

func (m *v8Function) prepareCall() C.ProgpV8FunctionPtr {
	if m.functionPtr == nil {
		return nil
	}

	if m.mustDisposeFunction == cInt1 {
		f := m.functionPtr
		m.functionPtr = nil
		return f
	}

	return m.functionPtr
}

func (m *v8Function) CallWithString(value string) {
	functionPtr := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		gTaskQueue.Push(func() {
			uPtr := unsafe.Pointer(unsafe.StringData(value))
			C.progp_CallFunctionWithStringP2(functionPtr, cInt1, m.mustDisposeFunction, (*C.char)(uPtr), C.size_t(len(value)))
		})
	} else {
		uPtr := unsafe.Pointer(unsafe.StringData(value))
		C.progp_CallFunctionWithStringP2(functionPtr, cInt0, m.mustDisposeFunction, (*C.char)(uPtr), C.size_t(len(value)))
	}
}

func (m *v8Function) CallWithArrayBuffer(value []byte) {
	functionPtr := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		gTaskQueue.Push(func() {
			C.progp_CallFunctionWithArrayBufferP2(functionPtr, cInt1, m.mustDisposeFunction, unsafe.Pointer(&value[0]), C.size_t(len(value)))
		})
	} else {
		C.progp_CallFunctionWithArrayBufferP2(functionPtr, cInt0, m.mustDisposeFunction, unsafe.Pointer(&value[0]), C.size_t(len(value)))
	}
}

func (m *v8Function) CallWithStringBuffer(value []byte) {
	functionPtr := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		gTaskQueue.Push(func() {
			uPtr := unsafe.Pointer(&value[0])
			C.progp_CallFunctionWithStringP2(functionPtr, cInt1, m.mustDisposeFunction, (*C.char)(uPtr), C.size_t(len(value)))
		})
	} else {
		uPtr := unsafe.Pointer(&value[0])
		C.progp_CallFunctionWithStringP2(functionPtr, cInt0, m.mustDisposeFunction, (*C.char)(uPtr), C.size_t(len(value)))
	}
}

func (m *v8Function) CallWithResource(value *progpAPI.SharedResource) {
	m.CallWithDouble(float64(value.GetId()))
}

func (m *v8Function) CallWithDouble(value float64) {
	functionPtr := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		gTaskQueue.Push(func() {
			C.progp_CallFunctionWithDoubleP2(functionPtr, cInt1, m.mustDisposeFunction, C.double(value))
		})
	} else {
		C.progp_CallFunctionWithDoubleP2(functionPtr, cInt0, m.mustDisposeFunction, C.double(value))
	}
}

func (m *v8Function) CallWithBool(value bool) {
	functionPtr := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		gTaskQueue.Push(func() {
			if value {
				C.progp_CallFunctionWithBoolP2(functionPtr, cInt1, m.mustDisposeFunction, cInt1)
			} else {
				C.progp_CallFunctionWithBoolP2(functionPtr, cInt1, m.mustDisposeFunction, cInt0)
			}
		})
	} else {
		if value {
			C.progp_CallFunctionWithBoolP2(functionPtr, cInt0, m.mustDisposeFunction, cInt1)
		} else {
			C.progp_CallFunctionWithBoolP2(functionPtr, cInt0, m.mustDisposeFunction, cInt0)
		}
	}
}

func (m *v8Function) CallWithUndefined() {
	functionPtr := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		gTaskQueue.Push(func() {
			C.progp_CallFunctionWithUndefined(functionPtr, cInt1, m.mustDisposeFunction)
		})
	} else {
		C.progp_CallFunctionWithUndefined(functionPtr, cInt0, m.mustDisposeFunction)
	}
}

func (m *v8Function) CallWithError(err error) {
	functionPtr := m.prepareCall()
	if functionPtr == nil {
		return
	}

	value := err.Error()

	if m.isAsync == cInt1 {
		gTaskQueue.Push(func() {
			uPtr := unsafe.Pointer(unsafe.StringData(value))
			C.progp_CallFunctionWithErrorP1(functionPtr, cInt1, m.mustDisposeFunction, (*C.char)(uPtr), C.size_t(len(value)))
		})
	} else {
		uPtr := unsafe.Pointer(unsafe.StringData(value))
		C.progp_CallFunctionWithErrorP1(functionPtr, cInt0, m.mustDisposeFunction, (*C.char)(uPtr), C.size_t(len(value)))
	}
}

func (m *v8Function) KeepAlive() {
	m.mustDisposeFunction = cInt0
}

//endregion

//region AnyValue

var gUndefinedAnyValue = C.s_progp_anyValue{valueType: C.int(AnyValueTypeUndefined)}
var gNilValue = reflect.ValueOf(nil)

func createErrorAnyValue(error string) C.s_progp_anyValue {
	var ev C.s_progp_anyValue
	ev.errorMessage = C.CString(error)
	return ev
}

func decodeAnyValue(value *C.s_progp_anyValue, expectedTypeName string, expectedTypeRef reflect.Type, isAsync bool) (reflect.Value, error) {
	valueType := AnyValueType(value.valueType)

	switch valueType {
	case AnyValueTypeUndefined:
		if expectedTypeName != "undefined" {
			return gNilValue, errors.New("Type " + expectedTypeName + " expected")
		}
		return gNilValue, nil

	case AnyValueTypeNull:
		if expectedTypeName != "null" {
			return gNilValue, errors.New("Type " + expectedTypeName + " expected")
		}
		return gNilValue, nil

	case AnyValueTypeInvalid:
		return gNilValue, errors.New("Type " + expectedTypeName + " expected")

	case AnyValueTypeNumber:
		if expectedTypeName == "float64" {
			if expectedTypeName != "float64" {
				return gNilValue, errors.New("Type " + expectedTypeName + " expected")
			}
			asRealValue := float64(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "float32" {
			if expectedTypeName != "float32" {
				return gNilValue, errors.New("Type " + expectedTypeName + " expected")
			}
			asRealValue := float32(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "int64" {
			if expectedTypeName != "int64" {
				return gNilValue, errors.New("Type " + expectedTypeName + " expected")
			}
			asRealValue := int64(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "int" {
			if expectedTypeName != "int" {
				return gNilValue, errors.New("Type " + expectedTypeName + " expected")
			}
			asRealValue := int(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "int32" {
			if expectedTypeName != "int32" {
				return gNilValue, errors.New("Type " + expectedTypeName + " expected")
			}
			asRealValue := int32(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "*progpAPI.SharedResource" {
			resId := int(value.number)
			asRealValue := progpAPI.GetSharedResource(resId)
			return reflect.ValueOf(asRealValue), nil
		} else {
			println("ProgpV8 - Unmanaged type for anyValue to any conversion: " + expectedTypeName)
			return gNilValue, errors.New("Type " + expectedTypeName + " expected")
		}
	case AnyValueTypeBoolean:
		if expectedTypeName != "bool" {
			return gNilValue, errors.New("Type " + expectedTypeName + " expected")
		}
		asRealValue := value.size == cInt1
		return reflect.ValueOf(asRealValue), nil
	case AnyValueTypeString:
		if expectedTypeName != "string" {
			return gNilValue, errors.New("Type " + expectedTypeName + " expected")
		}
		asRealValue := C.GoString((*C.char)(value.voidPtr))
		return reflect.ValueOf(asRealValue), nil
	case AnyValueTypeBuffer:
		if expectedTypeName != "[]uint8" {
			return gNilValue, errors.New("Type " + expectedTypeName + " expected")
		}
		asRealValue := C.GoBytes(unsafe.Pointer(value.voidPtr), value.size)
		return reflect.ValueOf(asRealValue), nil
	case AnyValueTypeJson:
		asBytes := C.GoBytes(value.voidPtr, value.size)

		genValue := reflect.New(expectedTypeRef)
		asAny := genValue.Interface()

		// Here asAny can be of type **Struct
		// but json.Unmarshal support it, so keep
		// as-is for simplicity.
		//
		err := json.Unmarshal(asBytes, asAny)

		// Here "reflect.New" always result on a pointer on the underlying type.
		// It's why we take the pointer value after creating instance.
		// If type is *Struct, then we have **Struct.
		// If type is Struct, then we have *Struct.
		//
		genValue = genValue.Elem()

		return genValue, err
	case AnyValueTypeFunction:
		if expectedTypeName != "progpAPI.ScriptFunction" {
			return gNilValue, errors.New("Type " + expectedTypeName + " expected")
		}

		var asRealValue progpAPI.ScriptFunction

		if isAsync {
			asRealValue = newV8Function(cInt1, C.ProgpV8FunctionPtr(value.voidPtr))
		} else {
			asRealValue = newV8Function(cInt0, C.ProgpV8FunctionPtr(value.voidPtr))
		}

		return reflect.ValueOf(asRealValue), nil
	}

	return gNilValue, nil
}

var gTypeByteArray = reflect.TypeOf([]byte{})
var gTypeResource = reflect.TypeOf(&progpAPI.SharedResource{})

func encodeAnyValue(resV reflect.Value) C.s_progp_anyValue {
	if !resV.IsValid() {
		return gUndefinedAnyValue
	} else {
		var res C.s_progp_anyValue
		res.errorMessage = nil
		kind := resV.Kind()

		if kind == reflect.String {
			res.valueType = C.int(AnyValueTypeString)
			res.voidPtr = unsafe.Pointer(C.CString(resV.String()))
			res.mustFree = cInt1
		} else if kind == reflect.Bool {
			res.valueType = C.int(AnyValueTypeBoolean)
			asBool := resV.Bool()
			if asBool == true {
				res.size = cInt1
			} else {
				res.size = cInt0
			}
		} else if resV.CanFloat() {
			res.valueType = C.int(AnyValueTypeNumber)
			res.number = C.double(resV.Float())
		} else if resV.CanInt() {
			res.valueType = C.int(AnyValueTypeNumber)
			res.number = C.double(resV.Int())
		} else if resV.CanUint() {
			res.valueType = C.int(AnyValueTypeNumber)
			res.number = C.double(resV.Uint())
		} else if resV.Type() == gTypeResource {
			rs := resV.Interface().(*progpAPI.SharedResource)
			res.valueType = C.int(AnyValueTypeNumber)
			res.number = C.double(rs.GetId())
		} else {
			var asBytes []byte
			var err error

			if resV.Type() == gTypeByteArray {
				res.valueType = C.int(AnyValueTypeBuffer)
				asBytes = resV.Interface().([]byte)
			} else if resV.CanInterface() {
				if resV.IsNil() {
					return gUndefinedAnyValue
				} else {
					// This code is called for Struct, *Struct, []Struct
					asInterface := resV.Interface()
					asBytes, err = json.Marshal(asInterface)
					if err != nil {
						return gUndefinedAnyValue
					}

					res.valueType = C.int(AnyValueTypeJson)
				}
			}

			res.size = C.int(len(asBytes))
			res.mustFree = cInt1

			// The GC can dispose the value while sending to C++.
			// It's why we must do a copy, even if it's for a very short time.
			res.voidPtr = C.progp_CopyGoBuffer(unsafe.Pointer(&asBytes[0]), res.size)
		}

		return res
	}
}

//endregion

//region Call from C++

// cppCallOnJavascriptError is called when a un-catch error occurs.
//
//export cppCallOnJavascriptError
func cppCallOnJavascriptError(error *C.s_progp_v8_errorMessage) {
	gLastErrorMessage = asScriptErrorMessage(error)
	progpAPI.OnUnCatchScriptError(gLastErrorMessage)
}

// cppCallOnDebuggerExited is called when the debugger windows has been closed
// or when the connexion is lost. Once stopped the debugger can't restart because
// he lost all the metadata about the current script.
//
//export cppCallOnDebuggerExited
func cppCallOnDebuggerExited() {
	println("--> Debugger has been closed.")
}

// cppCallOnNoMoreTask is called when all background tasks are finished.
// Generally once called the app will exit.
//
//export cppCallOnNoMoreTask
func cppCallOnNoMoreTask() {
	freeCurrentScriptCaller()
}

// cppOnDynamicFunctionProviderRequested is call when the engine must create a new function group.
// It's call when start for the group "global" which is where live the main function, and it's called
// for each call to the special js function "progpGetModule" which return an object with external functions.
//
//export cppOnDynamicFunctionProviderRequested
func cppOnDynamicFunctionProviderRequested(cGroupName *C.char) {
	groupName := C.GoString(cGroupName)

	if gFunctionRegistry.IsUsingDynamicMode() {
		for _, fct := range gFunctionRegistry.GetAllFunctions(false) {
			if fct.Group == groupName {
				jsName := fct.JsFunctionName
				gProgpV8Engine.AddDraftFunction(jsName)
			}
		}
	}
}

// cppOnDynamicFunctionCalled is called when a dynamic function is executed.
// Which mean a function which binding code isn't compiled. Dynamic function
// are slow and use reflective capacities to dynamically call the underlying
// Go function.
//
//export cppOnDynamicFunctionCalled
func cppOnDynamicFunctionCalled(cFunctionName *C.char, cAnyValueArray *C.s_progp_anyValue, cValueCount C.int) C.s_progp_anyValue {
	functionName := C.GoString(cFunctionName)
	registry := progpAPI.GetFunctionRegistry()
	fctRef := registry.GetRefToFunction(functionName)

	if fctRef == nil {
		return createErrorAnyValue(fmt.Sprintf("Unknown function %s", functionName))
	}
	parserFunctionInfos := fctRef.GoFunctionInfos

	expectedInputSize := len(parserFunctionInfos.ParamTypes)
	size := int(cValueCount)

	if size != expectedInputSize {
		if size > expectedInputSize {
			size = expectedInputSize
		} else {
			return createErrorAnyValue(fmt.Sprintf("%d argument expected", expectedInputSize))
		}
	}

	var values = make([]reflect.Value, size)

	for i := 0; i < size; i++ {
		// Here we work with reflect.Value because of struct values
		// which must be differentiated from pointer on value, something
		// which can't be done if returning an any.
		//
		var err error
		values[i], err = decodeAnyValue(cAnyValueArray, parserFunctionInfos.ParamTypes[i], parserFunctionInfos.ParamTypeRefs[i], fctRef.IsAsync)

		if err != nil {
			return createErrorAnyValue(err.Error())
		}

		ptr := unsafe.Add(unsafe.Pointer(cAnyValueArray), gSizeOfAnyValueStruct)
		cAnyValueArray = (*C.s_progp_anyValue)(ptr)
	}

	if fctRef.IsAsync {
		C.progp_IncreaseContextRef()
	}

	resultValues, errorMsg := progpAPI.DynamicCallFunction(fctRef.GoFunctionRef, values)

	if errorMsg != "" {
		if fctRef.IsAsync {
			C.progp_DecreaseContextRef()
		}

		return createErrorAnyValue(errorMsg)
	}

	if len(resultValues) == 0 {
		return gUndefinedAnyValue
	}

	if fctRef.GoFunctionInfos.ReturnErrorOffset != -1 {
		errOffset := fctRef.GoFunctionInfos.ReturnErrorOffset
		errValue := resultValues[errOffset]

		if !errValue.IsNil() {
			asAny := errValue.Interface()
			asError, ok := asAny.(error)

			if ok {
				return createErrorAnyValue(asError.Error())
			} else {
				return createErrorAnyValue("Unknown error when calling function " + functionName)
			}
		}

		if len(resultValues) == 1 {
			return gUndefinedAnyValue
		}

		if errOffset == 0 {
			resultValues = resultValues[1:]
		}
	}

	return encodeAnyValue(resultValues[0])
}

//endregion
