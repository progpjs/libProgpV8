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

package progpV8Engine

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
	"os"
	"reflect"
	"runtime"
	"sync"
	"unsafe"

	"github.com/progpjs/progpAPI/v2"
	_ "github.com/progpjs/progpV8Engine/v2/libs/darwin_arm64"
	_ "github.com/progpjs/progpV8Engine/v2/libs/linux_arm64"
	_ "github.com/progpjs/progpV8Engine/v2/libs/linux_x86_64"
)

//region V8Engine

type V8Engine struct {
	isStarted  bool
	isShutdown bool

	runtimeErrorHandler     progpAPI.RuntimeErrorHandlerF
	scriptTerminatedHandler progpAPI.ScriptTerminatedHandlerF
	shutdownMutex           sync.Mutex
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
		gProgpV8Engine = &V8Engine{}
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

func (m *V8Engine) Shutdown() {
	if m.isShutdown {
		return
	}

	m.shutdownMutex.Lock()
	defer m.shutdownMutex.Unlock()

	if m.isShutdown {
		return
	}
	m.isShutdown = true

	gShutdownMutex.Lock()
	defer gShutdownMutex.Unlock()

	for _, c := range gContexts {
		if c != nil {
			c.dispose()
		}
	}
}

func (m *V8Engine) CreateNewScriptContext(securityGroup string, mustDebug bool) progpAPI.JsContext {
	return newV8ScriptContext(securityGroup)
}

func (m *V8Engine) SetRuntimeErrorHandler(handler progpAPI.RuntimeErrorHandlerF) {
	m.runtimeErrorHandler = handler
}

func (m *V8Engine) SetScriptTerminatedHandler(handler progpAPI.ScriptTerminatedHandlerF) {
	m.scriptTerminatedHandler = handler
}

func (m *V8Engine) SetAllowedFunctionsChecker(handler progpAPI.CheckAllowedFunctionsF) {
	gAllowedFunctionsChecker = handler
}

func (m *V8Engine) GetFunctionCaller(functionSignature string) any {
	return gFunctionCallerMap[functionSignature]
}

var gFunctionCallerMap = make(map[string]any)

func registerFunctionCaller(fct any, signature string) {
	gFunctionCallerMap[signature] = fct
}

func asScriptErrorMessage(ctx *v8ScriptContext, ptr *C.s_progp_v8_errorMessage) *progpAPI.JsErrorMessage {
	m := progpAPI.NewScriptErrorMessage(ctx)

	m.ScriptPath = ctx.scriptPath

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
	return m
}

func getSharedResourceContainerFromUIntPtr(cEventId C.uintptr_t) *progpAPI.SharedResourceContainer {
	return (*progpAPI.SharedResourceContainer)(unsafe.Pointer(uintptr(cEventId)))
}

func resolveSharedResourceFromDouble(cEventId C.uintptr_t, resId C.double) *progpAPI.SharedResource {
	rc := (*progpAPI.SharedResourceContainer)(unsafe.Pointer(uintptr(cEventId)))
	return rc.GetResource(int(resId))
}

func resolveSharedResourceFromUIntPtr(cEventId C.uintptr_t, resId C.uintptr_t) *progpAPI.SharedResource {
	rc := (*progpAPI.SharedResourceContainer)(unsafe.Pointer(uintptr(cEventId)))
	return rc.GetResource(int(resId))
}

var gLastErrorMessage *progpAPI.JsErrorMessage
var gShutdownMutex sync.Mutex
var gSizeOfAnyValueStruct = int(C.progp_GetSizeOfAnyValueStruct())
var gFunctionRegistry = progpAPI.GetFunctionRegistry()
var gAllowedFunctionsChecker progpAPI.CheckAllowedFunctionsF

func asCBool(v bool) C.int {
	if v {
		return cInt1
	} else {
		return cInt0
	}
}

const cInt0 = C.int(0)
const cInt1 = C.int(1)
const cUintPtr0 = C.uintptr_t(0)

//endregion

//region V8ScriptContext

type v8ScriptContext struct {
	sharedResourceContainer *progpAPI.SharedResourceContainer
	progpCtx                C.ProgpContext
	securityGroup           string
	taskQueue               *progpAPI.TaskQueue
	callerLockMutex         sync.Mutex
	scriptPath              string
	isDisposed              bool
	contextId               int
}

var gContexts []*v8ScriptContext
var gContextsMutex sync.Mutex

func newV8ScriptContext(securityGroup string) *v8ScriptContext {
	m := &v8ScriptContext{securityGroup: securityGroup, taskQueue: progpAPI.NewTaskQueue()}
	m.sharedResourceContainer = progpAPI.NewSharedResourceContainer(nil, m)

	gContextsMutex.Lock()
	{
		isFound := false

		for i, e := range gContexts {
			if e == nil {
				gContexts[i] = m
				m.contextId = i
				isFound = true
				break
			}
		}

		if !isFound {
			m.contextId = len(gContexts)
			gContexts = append(gContexts, m)
		}
	}
	gContextsMutex.Unlock()

	m.progpCtx = C.progp_CreateNewContext(C.uintptr_t(uintptr(unsafe.Pointer(m))))

	// Note: the context must be registered in v8ScriptContext et gContexts
	// before calling since the C++ part will do call about this context.
	//
	C.progp_InitializeContext(m.progpCtx)

	return m
}

func (m *v8ScriptContext) dispose() {
	if m.isDisposed {
		return
	}
	gContextsMutex.Lock()

	if m.isDisposed {
		gContextsMutex.Unlock()
		return
	}

	m.isDisposed = true

	gContexts[m.contextId] = nil
	gContextsMutex.Unlock()

	// Allow avoiding case where the current tasks
	// isn't completely finished while the context is exited.
	//
	m.taskQueue.Exit(func() {
		C.progp_DisposeContext(m.progpCtx)
	})

	m.sharedResourceContainer.Dispose()

	m.callerLockMutex.Unlock()
}

func (m *v8ScriptContext) GetScriptEngine() progpAPI.ScriptEngine {
	return gProgpV8Engine
}

func (m *v8ScriptContext) GetSecurityGroup() string {
	return m.securityGroup
}

func (m *v8ScriptContext) ExecuteScript(scriptContent string, compiledFilePath string, sourceScriptPath string) *progpAPI.JsErrorMessage {
	progpAPI.DeclareBackgroundTaskStarted()
	defer progpAPI.DeclareBackgroundTaskEnded()

	m.scriptPath = compiledFilePath

	m.callerLockMutex.Lock()

	m.taskQueue.Push(func() {
		gLastErrorMessage = nil

		cScriptContent := C.CString(scriptContent)
		defer C.free(unsafe.Pointer(cScriptContent))

		cCompiledFilePath := C.CString(compiledFilePath)
		defer C.free(unsafe.Pointer(cCompiledFilePath))

		C.progp_ExecuteScript(m.progpCtx, cScriptContent, cCompiledFilePath, C.uintptr_t(uintptr(unsafe.Pointer(m.sharedResourceContainer))))
	})

	// Is unlocked when disposing the context.
	// Allow waiting execution ends.
	//
	m.callerLockMutex.Lock()

	err := gLastErrorMessage
	gLastErrorMessage = nil

	if gProgpV8Engine.scriptTerminatedHandler != nil {
		err = gProgpV8Engine.scriptTerminatedHandler(m, sourceScriptPath, err)
	}

	return err
}

func (m *v8ScriptContext) ExecuteScriptFile(scriptPath string, onCompiledSuccess func()) *progpAPI.JsErrorMessage {
	// It's required since script translation is in progpScripts and not progpAPI.
	ex := progpAPI.GetScriptFileExecutor()
	return ex(m, scriptPath, onCompiledSuccess)
}

func (m *v8ScriptContext) ExecuteChildScriptFile(scriptPath string) error {
	// It's required since script translation is in progpScripts and not progpAPI.
	cp := progpAPI.GetScriptFileCompiler()
	scriptContent, scriptOrigin, err := cp(scriptPath)

	if err == nil {
		jsErr := m.ExecuteScript(scriptContent, scriptOrigin, scriptPath)
		if jsErr != nil {
			err = createError(jsErr.Error)
		}
	} else {
		// Will avoid exception when unlocking.
		m.callerLockMutex.Lock()

		// If no error then dispose is done by a call from cppCallOnNoMoreTask
		// before m.ExecuteScript returns. Here we only handle the case where
		// the script hasn't reached the execute point.
		//
		m.dispose()
	}

	return err
}

func (m *v8ScriptContext) TryDispose() bool {
	C.progp_DisposeContext(m.progpCtx)
	return true
}

func (m *v8ScriptContext) DisarmError(error *progpAPI.JsErrorMessage) {
	if gLastErrorMessage == error {
		gLastErrorMessage = nil
	}
}

func (m *v8ScriptContext) IncreaseRefCount() {
	C.progp_IncreaseContextRef(m.progpCtx)
}

func (m *v8ScriptContext) DecreaseRefCount() {
	C.progp_DecreaseContextRef(m.progpCtx)
}

func (m *v8ScriptContext) AddDraftFunction(functionName string) {
	cFunctionName := C.CString(functionName)
	defer C.free(unsafe.Pointer(cFunctionName))
	C.progp_DeclareDynamicFunction(m.progpCtx, cFunctionName)
}

//endregion

//region V8Function

type v8Function struct {
	isAsync                 C.int
	mustDecreaseTasks       C.int
	mustDisposeFunction     C.int
	functionPtr             C.ProgpV8FunctionPtr
	currentEvent            C.ProgpEvent
	parentResourceContainer *progpAPI.SharedResourceContainer
	v8Context               *v8ScriptContext
}

func newV8Function(isAsync C.int, ptr C.ProgpV8FunctionPtr, currentEvent C.ProgpEvent) progpAPI.JsFunction {
	res := new(v8Function)
	res.functionPtr = ptr
	res.isAsync = isAsync
	res.mustDecreaseTasks = isAsync
	res.currentEvent = currentEvent
	res.v8Context = (*v8ScriptContext)(unsafe.Pointer(uintptr(currentEvent.contextData)))

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

func (m *v8Function) prepareCall() (C.ProgpV8FunctionPtr, C.uintptr_t) {
	if m.functionPtr == nil {
		return nil, cUintPtr0
	}

	var rcPtr C.uintptr_t

	// If not nil mean we must manage the resources created while calling this function.
	if m.parentResourceContainer != nil {
		rc := progpAPI.NewSharedResourceContainer(m.parentResourceContainer, nil)
		rcPtr = C.uintptr_t(uintptr(unsafe.Pointer(rc)))
	}

	if m.mustDisposeFunction == cInt1 {
		f := m.functionPtr
		m.functionPtr = nil
		return f, rcPtr
	}

	return m.functionPtr, rcPtr
}

func (m *v8Function) KeepAlive() {
	m.mustDisposeFunction = cInt0
	m.mustDecreaseTasks = cInt0

	// Avoid restoring the event context that was the context of the function.
	m.currentEvent = nil

	// Force the use of async mode, which is a little slower but allow avoiding
	// difficulties in high load scenarios or if your function use slow/blocking functions.
	//
	m.isAsync = cInt1
}

func (m *v8Function) EnabledResourcesAutoDisposing(currentResourceContainer *progpAPI.SharedResourceContainer) {
	m.parentResourceContainer = currentResourceContainer
}

func (m *v8Function) CallWithString2(value string) {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			uPtr := unsafe.Pointer(unsafe.StringData(value))
			C.progp_CallFunctionWithStringP2(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, (*C.char)(uPtr), C.size_t(len(value)))
		})
	} else {
		uPtr := unsafe.Pointer(unsafe.StringData(value))
		C.progp_CallFunctionWithStringP2(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, (*C.char)(uPtr), C.size_t(len(value)))
	}
}

func (m *v8Function) CallWithArrayBuffer2(value []byte) {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			C.progp_CallFunctionWithArrayBufferP2(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, unsafe.Pointer(&value[0]), C.size_t(len(value)))
		})
	} else {
		C.progp_CallFunctionWithArrayBufferP2(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, unsafe.Pointer(&value[0]), C.size_t(len(value)))
	}
}

func (m *v8Function) CallWithStringBuffer2(value []byte) {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			uPtr := unsafe.Pointer(&value[0])
			C.progp_CallFunctionWithStringP2(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, (*C.char)(uPtr), C.size_t(len(value)))
		})
	} else {
		uPtr := unsafe.Pointer(&value[0])
		C.progp_CallFunctionWithStringP2(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, (*C.char)(uPtr), C.size_t(len(value)))
	}
}

func (m *v8Function) CallWithResource1(value *progpAPI.SharedResource) {
	m.CallWithDouble1(float64(value.GetId()))
}

func (m *v8Function) CallWithResource2(value *progpAPI.SharedResource) {
	m.CallWithDouble2(float64(value.GetId()))
}

func (m *v8Function) CallWithDouble1(value float64) {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			C.progp_CallFunctionWithDoubleP1(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, C.double(value))
		})
	} else {
		C.progp_CallFunctionWithDoubleP1(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, C.double(value))
	}
}

func (m *v8Function) CallWithDouble2(value float64) {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			C.progp_CallFunctionWithDoubleP2(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, C.double(value))
		})
	} else {
		C.progp_CallFunctionWithDoubleP2(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, C.double(value))
	}
}

func (m *v8Function) CallWithBool2(value bool) {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			if value {
				C.progp_CallFunctionWithBoolP2(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, cInt1)
			} else {
				C.progp_CallFunctionWithBoolP2(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, cInt0)
			}
		})
	} else {
		if value {
			C.progp_CallFunctionWithBoolP2(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, cInt1)
		} else {
			C.progp_CallFunctionWithBoolP2(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, cInt0)
		}
	}
}

func (m *v8Function) CallWithUndefined() {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			C.progp_CallFunctionWithUndefined(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer)
		})
	} else {
		C.progp_CallFunctionWithUndefined(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer)
	}
}

func (m *v8Function) CallWithError(err error) {
	functionPtr, resourceContainer := m.prepareCall()
	if functionPtr == nil {
		return
	}

	value := err.Error()

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			uPtr := unsafe.Pointer(unsafe.StringData(value))
			C.progp_CallFunctionWithErrorP1(functionPtr, m.mustDecreaseTasks, m.mustDisposeFunction, m.currentEvent, resourceContainer, (*C.char)(uPtr), C.size_t(len(value)))
		})
	} else {
		uPtr := unsafe.Pointer(unsafe.StringData(value))
		C.progp_CallFunctionWithErrorP1(functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer, (*C.char)(uPtr), C.size_t(len(value)))
	}
}

func (m *v8Function) DynamicFunctionCaller(values ...any) {
	cDynFunctionCaller := C.progp_DynamicFunctionCaller_New(C.int(len(values)))

	for offset, value := range values {
		anyValue := encodeAnyValue(reflect.ValueOf(value))
		C.progp_DynamicFunctionCaller_AddParam(cDynFunctionCaller, &anyValue, C.int(offset))
	}

	functionPtr, resourceContainer := m.prepareCall()

	if m.isAsync == cInt1 {
		m.v8Context.taskQueue.Push(func() {
			C.progp_DynamicFunctionCaller_Call(cDynFunctionCaller, functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer)
		})
	} else {
		C.progp_DynamicFunctionCaller_Call(cDynFunctionCaller, functionPtr, cInt0, m.mustDisposeFunction, m.currentEvent, resourceContainer)
	}
}

//endregion

//region Helpers

// createError purpose is to help debugging by grouping all the error emitters here.
// By doing that you only have to put a breakpoint here in order to use the stacktrace
// to known where exactly the error come from.
func createError(message string) error {
	return errors.New(message)
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

func decodeAnyValue(value *C.s_progp_anyValue, expectedTypeName string, expectedTypeRef reflect.Type, isAsync bool, currentEvent C.ProgpEvent) (reflect.Value, error) {
	valueType := AnyValueType(value.valueType)

	switch valueType {
	case AnyValueTypeUndefined:
		if expectedTypeName != "undefined" {
			return gNilValue, createError("Type " + expectedTypeName + " expected")
		}
		return gNilValue, nil

	case AnyValueTypeNull:
		if expectedTypeName != "null" {
			return gNilValue, createError("Type " + expectedTypeName + " expected")
		}
		return gNilValue, nil

	case AnyValueTypeInvalid:
		return gNilValue, createError("Type " + expectedTypeName + " expected")

	case AnyValueTypeNumber:
		if expectedTypeName == "float64" {
			if expectedTypeName != "float64" {
				return gNilValue, createError("Type " + expectedTypeName + " expected")
			}
			asRealValue := float64(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "float32" {
			if expectedTypeName != "float32" {
				return gNilValue, createError("Type " + expectedTypeName + " expected")
			}
			asRealValue := float32(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "int64" {
			if expectedTypeName != "int64" {
				return gNilValue, createError("Type " + expectedTypeName + " expected")
			}
			asRealValue := int64(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "int" {
			if expectedTypeName != "int" {
				return gNilValue, createError("Type " + expectedTypeName + " expected")
			}
			asRealValue := int(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "int32" {
			if expectedTypeName != "int32" {
				return gNilValue, createError("Type " + expectedTypeName + " expected")
			}
			asRealValue := int32(value.number)
			return reflect.ValueOf(asRealValue), nil
		} else if expectedTypeName == "*progpAPI.SharedResource" {
			resId := C.uintptr_t(value.number)
			asRealValue := resolveSharedResourceFromUIntPtr(currentEvent.id, resId)
			return reflect.ValueOf(asRealValue), nil
		} else {
			println("ProgpV8 - Unmanaged type for anyValue to any conversion: " + expectedTypeName)
			return gNilValue, createError("Type " + expectedTypeName + " expected")
		}
	case AnyValueTypeBoolean:
		if expectedTypeName != "bool" {
			return gNilValue, createError("Type " + expectedTypeName + " expected")
		}
		asRealValue := value.size == cInt1
		return reflect.ValueOf(asRealValue), nil
	case AnyValueTypeString:
		if expectedTypeName != "string" {
			return gNilValue, createError("Type " + expectedTypeName + " expected")
		}
		asRealValue := C.GoString((*C.char)(value.voidPtr))
		return reflect.ValueOf(asRealValue), nil
	case AnyValueTypeBuffer:
		if expectedTypeName != "[]uint8" {
			return gNilValue, createError("Type " + expectedTypeName + " expected")
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
		if expectedTypeName != "progpAPI.JsFunction" {
			return gNilValue, createError("Type " + expectedTypeName + " expected")
		}

		var asRealValue progpAPI.JsFunction

		if isAsync {
			asRealValue = newV8Function(cInt1, C.ProgpV8FunctionPtr(value.voidPtr), currentEvent)
		} else {
			asRealValue = newV8Function(cInt0, C.ProgpV8FunctionPtr(value.voidPtr), currentEvent)
		}

		return reflect.ValueOf(asRealValue), nil
	}

	return gNilValue, nil
}

func encodeAnyValue(resV reflect.Value) C.s_progp_anyValue {
	if !resV.IsValid() {
		return gUndefinedAnyValue
	} else {
		var res C.s_progp_anyValue
		res.errorMessage = nil
		kind := resV.Kind()
		resvType := resV.Type()

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
		} else if resvType == gTypeResource {
			rs := resV.Interface().(*progpAPI.SharedResource)
			res.valueType = C.int(AnyValueTypeNumber)
			res.number = C.double(rs.GetId())
		} else if resvType == gStringBuffer {
			res.valueType = C.int(AnyValueTypeString)
			bytes := resV.Bytes()
			res.voidPtr = unsafe.Pointer(C.CString(string(bytes)))
			res.mustFree = cInt1
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

var gTypeByteArray = reflect.TypeOf([]byte{})
var gTypeResource = reflect.TypeOf(&progpAPI.SharedResource{})
var gStringBuffer = reflect.TypeOf(progpAPI.StringBuffer{})

//endregion

//region Call from C++

// cppCallOnJavascriptError is called when a un-catch error occurs.
//
//export cppCallOnJavascriptError
func cppCallOnJavascriptError(ctxRef *C.void, error *C.s_progp_v8_errorMessage) {
	ctx := (*v8ScriptContext)(unsafe.Pointer(ctxRef))
	gLastErrorMessage = asScriptErrorMessage(ctx, error)

	if gProgpV8Engine.runtimeErrorHandler != nil {
		if gProgpV8Engine.runtimeErrorHandler(ctx, gLastErrorMessage) {
			return
		}
	}

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
func cppCallOnNoMoreTask(ctxRef *C.void) {
	ctx := (*v8ScriptContext)(unsafe.Pointer(ctxRef))
	ctx.dispose()
}

// cppOnDynamicFunctionProviderRequested is call when the engine must create a new function group.
// It's call when start for the group "global" which is where live the main function, and it's called
// for each call to the special js function "progpGetModule" which return an object with external functions.
//
//export cppOnDynamicFunctionProviderRequested
func cppOnDynamicFunctionProviderRequested(ctxRef *C.void, cGroupName *C.char) {
	ctx := (*v8ScriptContext)(unsafe.Pointer(ctxRef))
	groupName := C.GoString(cGroupName)

	if gFunctionRegistry.IsUsingDynamicMode() {
		for _, fct := range gFunctionRegistry.GetAllFunctions(false) {
			if fct.Group == groupName {
				jsName := fct.JsFunctionName
				ctx.AddDraftFunction(jsName)
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
func cppOnDynamicFunctionCalled(ctxRef *C.void, cFunctionName *C.char, cAnyValueArray *C.s_progp_anyValue, cValueCount C.int, currentEvent C.ProgpEvent) C.s_progp_anyValue {
	ctx := (*v8ScriptContext)(unsafe.Pointer(ctxRef))

	functionName := C.GoString(cFunctionName)
	registry := progpAPI.GetFunctionRegistry()

	fctRef := registry.GetRefToFunction(functionName)
	if fctRef == nil {
		return createErrorAnyValue(fmt.Sprintf("Unknown function %s", functionName))
	}

	parserFunctionInfos := fctRef.GoFunctionInfos
	inputParamsCount := len(parserFunctionInfos.ParamTypes)
	expectedInputSize := inputParamsCount
	providedParamCount := int(cValueCount)

	for _, pt := range parserFunctionInfos.ParamTypes {
		// It a virtual parameter so decrease the counter for him.
		if pt == "*progpAPI.SharedResourceContainer" {
			expectedInputSize--
		}
	}

	if providedParamCount != expectedInputSize {
		if providedParamCount > expectedInputSize {
			providedParamCount = expectedInputSize
		} else {
			return createErrorAnyValue(fmt.Sprintf("%d argument expected", expectedInputSize))
		}
	}

	var values = make([]reflect.Value, inputParamsCount)

	for i := 0; i < inputParamsCount; i++ {
		paramType := parserFunctionInfos.ParamTypes[i]
		if paramType == "*progpAPI.SharedResourceContainer" {
			values[i] = reflect.ValueOf(getSharedResourceContainerFromUIntPtr(currentEvent.id))
			continue
		}

		// Here we work with reflect.Value because of struct values
		// which must be differentiated from pointer on value, something
		// which can't be done if returning an any.
		//
		var err error
		values[i], err = decodeAnyValue(cAnyValueArray, paramType, parserFunctionInfos.ParamTypeRefs[i], fctRef.IsAsync, currentEvent)

		if err != nil {
			return createErrorAnyValue(err.Error())
		}

		ptr := unsafe.Add(unsafe.Pointer(cAnyValueArray), gSizeOfAnyValueStruct)
		cAnyValueArray = (*C.s_progp_anyValue)(ptr)
	}

	if fctRef.IsAsync {
		C.progp_IncreaseContextRef(ctx.progpCtx)
	}

	resultValues, errorMsg := progpAPI.DynamicCallFunction(fctRef.GoFunctionRef, values)

	if errorMsg != "" {
		if fctRef.IsAsync {
			C.progp_DecreaseContextRef(ctx.progpCtx)
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

//export cppCheckAllowedFunction
func cppCheckAllowedFunction(cSecurityGroup *C.char, cFunctionGroup *C.char, cFunctionName *C.char) C.int {
	if gAllowedFunctionsChecker != nil {
		securityGroup := C.GoString(cSecurityGroup)
		functionGroup := C.GoString(cFunctionGroup)
		cFunctionName := C.GoString(cFunctionName)

		if gAllowedFunctionsChecker(securityGroup, functionGroup, cFunctionName) {
			return cInt1
		}

		return cInt0
	}

	return cInt1
}

//export cppOnEventFinished
func cppOnEventFinished(cEventId C.uintptr_t) {
	rc := getSharedResourceContainerFromUIntPtr(cEventId)
	rc.Dispose()
}

//export cppOnPrintLine
func cppOnPrintLine(cText *C.char) {
	text := C.GoString(cText)
	_, _ = fmt.Fprint(os.Stdout, text)
}

//endregion
