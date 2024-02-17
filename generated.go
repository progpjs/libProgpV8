package progpV8Engine

// #include <stdlib.h> // For C.free
// #include "progpAPI.h"
//
import "C"

import (
	"encoding/json"
	"github.com/progpjs/httpServer"
	"github.com/progpjs/modules/modCore"
	"github.com/progpjs/modules/modHttp"
	"github.com/progpjs/modules/modNodeJs"
	"github.com/progpjs/progpAPI"
	"github.com/progpjs/samples/modSamples"
	"unsafe"
)

//export progpCgoBinding__3
func progpCgoBinding__3(res *C.ProgpFunctionReturnVoid, p1 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modCore.JsProgpAutoDispose(getSharedResourceContainerFromUIntPtr(res.currentEvent.id), newV8Function(res.isAsync, p1, res.currentEvent))

}

//export progpCgoBinding__1
func progpCgoBinding__1(res *C.ProgpFunctionReturnVoid, p0 C.double, p1 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modCore.JsProgpCallAfterMsAsync(int(p0), newV8Function(res.isAsync, p1, res.currentEvent))

}

//export progpCgoBinding__2
func progpCgoBinding__2(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp0 := C.GoBytes(unsafe.Pointer(p0.p), p0.n)
	var vp0 progpAPI.SharedResource
	if err := json.Unmarshal(bp0, &vp0); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	modCore.JsProgpDispose(vp0)

}

//export progpCgoBinding__88
func progpCgoBinding__88(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr, p1 C.ProgpV8BufferPtr) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsCallbackWithArrayBuffer(newV8Function(res.isAsync, p0, res.currentEvent), C.GoBytes(p1.buffer, p1.length))

}

//export progpCgoBinding__84
func progpCgoBinding__84(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr, p1 C.int) {
	defer progpAPI.CatchFatalErrors()

	var p1_asBool = true
	if C.int(p1) == 0 {
		p1_asBool = false
	}

	modSamples.JsCallbackWithBool(newV8Function(res.isAsync, p0, res.currentEvent), p1_asBool)

}

//export progpCgoBinding__85
func progpCgoBinding__85(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr, p1 C.double) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsCallbackWithDouble(newV8Function(res.isAsync, p0, res.currentEvent), float64(p1))

}

//export progpCgoBinding__86
func progpCgoBinding__86(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsCallbackWithError(newV8Function(res.isAsync, p0, res.currentEvent), C.GoStringN(p1.p, p1.n))

}

//export progpCgoBinding__83
func progpCgoBinding__83(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsCallbackWithString(newV8Function(res.isAsync, p0, res.currentEvent), C.GoStringN(p1.p, p1.n))

}

//export progpCgoBinding__89
func progpCgoBinding__89(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsCallbackWithStringBuffer(newV8Function(res.isAsync, p0, res.currentEvent), C.GoStringN(p1.p, p1.n))

}

//export progpCgoBinding__87
func progpCgoBinding__87(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsCallbackWithoutValues(newV8Function(res.isAsync, p0, res.currentEvent))

}

//export progpCgoBinding__90
func progpCgoBinding__90(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsTestAsync(C.GoStringN(p0.p, p0.n), newV8Function(res.isAsync, p1, res.currentEvent))

}

//export progpCgoBinding__78
func progpCgoBinding__78(res *C.ProgpFunctionReturnInt, p0 C.int) {
	defer progpAPI.CatchFatalErrors()

	var p0_asBool = true
	if C.int(p0) == 0 {
		p0_asBool = false
	}

	goRes := modSamples.JsTestBool(p0_asBool)
	if goRes {
		res.value = C.int(1)
	} else {
		res.value = C.int(0)
	}
}

//export progpCgoBinding__82
func progpCgoBinding__82(res *C.ProgpFunctionReturnArrayBuffer, p0 C.ProgpV8BufferPtr) {
	defer progpAPI.CatchFatalErrors()

	goRes := modSamples.JsTestByteArray(C.GoBytes(p0.buffer, p0.length))
	res.value = unsafe.Pointer(&goRes[0])
	res.size = C.int(len(goRes))
}

//export progpCgoBinding__96
func progpCgoBinding__96(res *C.ProgpFunctionReturnLong, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes := modSamples.JsTestCreateResource(getSharedResourceContainerFromUIntPtr(res.currentEvent.id), C.GoStringN(p1.p, p1.n))
	res.value = C.long(goRes.GetId())
}

//export progpCgoBinding__74
func progpCgoBinding__74(res *C.ProgpFunctionReturnDouble, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	goRes := modSamples.JsTestFloat32(float32(p0))
	res.value = C.double(goRes)
}

//export progpCgoBinding__73
func progpCgoBinding__73(res *C.ProgpFunctionReturnDouble, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	goRes := modSamples.JsTestFloat64(float64(p0))
	res.value = C.double(goRes)
}

//export progpCgoBinding__77
func progpCgoBinding__77(res *C.ProgpFunctionReturnLong, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	goRes := modSamples.JsTestInt(int(p0))
	res.value = C.long(goRes)
}

//export progpCgoBinding__75
func progpCgoBinding__75(res *C.ProgpFunctionReturnArrayBuffer, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp0 := C.GoBytes(unsafe.Pointer(p0.p), p0.n)
	var vp0 int32
	if err := json.Unmarshal(bp0, &vp0); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	goRes := modSamples.JsTestInt32(vp0)
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__76
func progpCgoBinding__76(res *C.ProgpFunctionReturnArrayBuffer, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp0 := C.GoBytes(unsafe.Pointer(p0.p), p0.n)
	var vp0 int64
	if err := json.Unmarshal(bp0, &vp0); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	goRes := modSamples.JsTestInt64(vp0)
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__94
func progpCgoBinding__94(res *C.ProgpFunctionReturnVoid, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsTestReceiveSharedResource(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

}

//export progpCgoBinding__97
func progpCgoBinding__97(res *C.ProgpFunctionReturnVoid, p1 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modSamples.JsTestRegisterEventReactor(getSharedResourceContainerFromUIntPtr(res.currentEvent.id), newV8Function(res.isAsync, p1, res.currentEvent))

}

//export progpCgoBinding__95
func progpCgoBinding__95(res *C.ProgpFunctionReturnLong) {
	defer progpAPI.CatchFatalErrors()

	goRes := modSamples.JsTestReturnSharedResource(getSharedResourceContainerFromUIntPtr(res.currentEvent.id))
	res.value = C.long(goRes.GetId())
}

//export progpCgoBinding__72
func progpCgoBinding__72(res *C.ProgpFunctionReturnString, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes := modSamples.JsTestString(C.GoStringN(p0.p, p0.n))
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__81
func progpCgoBinding__81(res *C.ProgpFunctionReturnArrayBuffer, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp0 := C.GoBytes(unsafe.Pointer(p0.p), p0.n)
	var vp0 []string
	if err := json.Unmarshal(bp0, &vp0); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	goRes := modSamples.JsTestStringArray(vp0)
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__80
func progpCgoBinding__80(res *C.ProgpFunctionReturnArrayBuffer, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp0 := C.GoBytes(unsafe.Pointer(p0.p), p0.n)
	var vp0 *modSamples.TestStructA
	if err := json.Unmarshal(bp0, &vp0); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	goRes := modSamples.JsTestStructPtr(vp0)
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__79
func progpCgoBinding__79(res *C.ProgpFunctionReturnArrayBuffer, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp0 := C.GoBytes(unsafe.Pointer(p0.p), p0.n)
	var vp0 modSamples.TestStructA
	if err := json.Unmarshal(bp0, &vp0); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	goRes := modSamples.JsTestStructRef(vp0)
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__91
func progpCgoBinding__91(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modSamples.JsThrowError(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__92
func progpCgoBinding__92(res *C.ProgpFunctionReturnString, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modSamples.JsThrowErrorP1(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__93
func progpCgoBinding__93(res *C.ProgpFunctionReturnString, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modSamples.JsThrowErrorP2(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__24
func progpCgoBinding__24(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.double, p2 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsAccessAsync(C.GoStringN(p0.p, p0.n), int(p1), newV8Function(res.isAsync, p2, res.currentEvent))

}

//export progpCgoBinding__23
func progpCgoBinding__23(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.double) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsFsAccessSync(C.GoStringN(p0.p, p0.n), int(p1))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__48
func progpCgoBinding__48(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.ProgpV8BufferPtr, p2 *C.s_progp_goStringOut, p3 C.double) {
	defer progpAPI.CatchFatalErrors()

	bp2 := C.GoBytes(unsafe.Pointer(p2.p), p2.n)
	var vp2 uint32
	if err := json.Unmarshal(bp2, &vp2); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	err := modNodeJs.JsFsAppendFileBytesSync(C.GoStringN(p0.p, p0.n), C.GoBytes(p1.buffer, p1.length), vp2, int(p3))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__47
func progpCgoBinding__47(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut, p2 *C.s_progp_goStringOut, p3 C.double) {
	defer progpAPI.CatchFatalErrors()

	bp2 := C.GoBytes(unsafe.Pointer(p2.p), p2.n)
	var vp2 uint32
	if err := json.Unmarshal(bp2, &vp2); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	err := modNodeJs.JsFsAppendFileTexSync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n), vp2, int(p3))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__26
func progpCgoBinding__26(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut, p2 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	bp1 := C.GoBytes(unsafe.Pointer(p1.p), p1.n)
	var vp1 uint32
	if err := json.Unmarshal(bp1, &vp1); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	modNodeJs.JsFsChmodAsync(C.GoStringN(p0.p, p0.n), vp1, newV8Function(res.isAsync, p2, res.currentEvent))

}

//export progpCgoBinding__25
func progpCgoBinding__25(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp1 := C.GoBytes(unsafe.Pointer(p1.p), p1.n)
	var vp1 uint32
	if err := json.Unmarshal(bp1, &vp1); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	err := modNodeJs.JsFsChmodSync(C.GoStringN(p0.p, p0.n), vp1)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__28
func progpCgoBinding__28(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.double, p2 C.double, p3 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsChownAsync(C.GoStringN(p0.p, p0.n), int(p1), int(p2), newV8Function(res.isAsync, p3, res.currentEvent))

}

//export progpCgoBinding__27
func progpCgoBinding__27(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.double, p2 C.double) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsFsChownSync(C.GoStringN(p0.p, p0.n), int(p1), int(p2))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__32
func progpCgoBinding__32(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut, p2 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsCopyFileAsync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n), newV8Function(res.isAsync, p2, res.currentEvent))

}

//export progpCgoBinding__31
func progpCgoBinding__31(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsFsCopyFileSync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__20
func progpCgoBinding__20(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsExistsAsync(C.GoStringN(p0.p, p0.n), newV8Function(res.isAsync, p1, res.currentEvent))

}

//export progpCgoBinding__19
func progpCgoBinding__19(res *C.ProgpFunctionReturnInt, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsFsExistsSync(C.GoStringN(p0.p, p0.n))
	if goRes {
		res.value = C.int(1)
	} else {
		res.value = C.int(0)
	}
}

//export progpCgoBinding__34
func progpCgoBinding__34(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut, p2 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsLinkAsync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n), newV8Function(res.isAsync, p2, res.currentEvent))

}

//export progpCgoBinding__33
func progpCgoBinding__33(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsFsLinkSync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__40
func progpCgoBinding__40(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.int, p2 *C.s_progp_goStringOut, p3 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	var p1_asBool = true
	if C.int(p1) == 0 {
		p1_asBool = false
	}

	bp2 := C.GoBytes(unsafe.Pointer(p2.p), p2.n)
	var vp2 uint32
	if err := json.Unmarshal(bp2, &vp2); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	modNodeJs.JsFsMkdirAsync(C.GoStringN(p0.p, p0.n), p1_asBool, vp2, newV8Function(res.isAsync, p3, res.currentEvent))

}

//export progpCgoBinding__39
func progpCgoBinding__39(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.int, p2 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	var p1_asBool = true
	if C.int(p1) == 0 {
		p1_asBool = false
	}

	bp2 := C.GoBytes(unsafe.Pointer(p2.p), p2.n)
	var vp2 uint32
	if err := json.Unmarshal(bp2, &vp2); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	err := modNodeJs.JsFsMkdirSync(C.GoStringN(p0.p, p0.n), p1_asBool, vp2)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__42
func progpCgoBinding__42(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsMkdtempAsync(C.GoStringN(p0.p, p0.n), newV8Function(res.isAsync, p1, res.currentEvent))

}

//export progpCgoBinding__41
func progpCgoBinding__41(res *C.ProgpFunctionReturnString, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modNodeJs.JsFsMkdtempSync(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__44
func progpCgoBinding__44(res *C.ProgpFunctionReturnArrayBuffer, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modNodeJs.JsFsReadFileBytesSync(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes[0])
	res.size = C.int(len(goRes))
}

//export progpCgoBinding__43
func progpCgoBinding__43(res *C.ProgpFunctionReturnString, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modNodeJs.JsFsReadFileUtf8Sync(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	resString := unsafe.String(unsafe.SliceData(goRes), len(goRes))
	res.value = unsafe.Pointer(&resString)
}

//export progpCgoBinding__49
func progpCgoBinding__49(res *C.ProgpFunctionReturnString, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modNodeJs.JsFsReadlinkSync(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__50
func progpCgoBinding__50(res *C.ProgpFunctionReturnString, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modNodeJs.JsFsRealpathSync(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__45
func progpCgoBinding__45(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsFsRenameSync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__46
func progpCgoBinding__46(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.int, p2 C.int) {
	defer progpAPI.CatchFatalErrors()

	var p1_asBool = true
	if C.int(p1) == 0 {
		p1_asBool = false
	}

	var p2_asBool = true
	if C.int(p2) == 0 {
		p2_asBool = false
	}

	err := modNodeJs.JsFsRmSync(C.GoStringN(p0.p, p0.n), p1_asBool, p2_asBool)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__22
func progpCgoBinding__22(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.int, p2 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	var p1_asBool = true
	if C.int(p1) == 0 {
		p1_asBool = false
	}

	modNodeJs.JsFsStatAsync(C.GoStringN(p0.p, p0.n), p1_asBool, newV8Function(res.isAsync, p2, res.currentEvent))

}

//export progpCgoBinding__21
func progpCgoBinding__21(res *C.ProgpFunctionReturnArrayBuffer, p0 *C.s_progp_goStringOut, p1 C.int) {
	defer progpAPI.CatchFatalErrors()

	var p1_asBool = true
	if C.int(p1) == 0 {
		p1_asBool = false
	}

	goRes, err := modNodeJs.JsFsStatSync(C.GoStringN(p0.p, p0.n), p1_asBool)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__36
func progpCgoBinding__36(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut, p2 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsSymLinkAsync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n), newV8Function(res.isAsync, p2, res.currentEvent))

}

//export progpCgoBinding__35
func progpCgoBinding__35(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsFsSymLinkSync(C.GoStringN(p0.p, p0.n), C.GoStringN(p1.p, p1.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__30
func progpCgoBinding__30(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut, p2 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	bp1 := C.GoBytes(unsafe.Pointer(p1.p), p1.n)
	var vp1 int64
	if err := json.Unmarshal(bp1, &vp1); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	modNodeJs.JsFsTruncateAsync(C.GoStringN(p0.p, p0.n), vp1, newV8Function(res.isAsync, p2, res.currentEvent))

}

//export progpCgoBinding__29
func progpCgoBinding__29(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp1 := C.GoBytes(unsafe.Pointer(p1.p), p1.n)
	var vp1 int64
	if err := json.Unmarshal(bp1, &vp1); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	err := modNodeJs.JsFsTruncateSync(C.GoStringN(p0.p, p0.n), vp1)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__38
func progpCgoBinding__38(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut, p1 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsFsUnlinkAsync(C.GoStringN(p0.p, p0.n), newV8Function(res.isAsync, p1, res.currentEvent))

}

//export progpCgoBinding__37
func progpCgoBinding__37(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsFsUnlinkSync(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__16
func progpCgoBinding__16(res *C.ProgpFunctionReturnString) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modNodeJs.JsOsHomeDir()

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__17
func progpCgoBinding__17(res *C.ProgpFunctionReturnString) {
	defer progpAPI.CatchFatalErrors()

	goRes, err := modNodeJs.JsOsHostName()

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__18
func progpCgoBinding__18(res *C.ProgpFunctionReturnString) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsOsTempDir()
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__7
func progpCgoBinding__7(res *C.ProgpFunctionReturnString) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessArch()
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__9
func progpCgoBinding__9(res *C.ProgpFunctionReturnArrayBuffer) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessArgV()
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__13
func progpCgoBinding__13(res *C.ProgpFunctionReturnVoid, p0 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsProcessChDir(C.GoStringN(p0.p, p0.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__5
func progpCgoBinding__5(res *C.ProgpFunctionReturnString) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessCwd()
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__6
func progpCgoBinding__6(res *C.ProgpFunctionReturnString) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessEnv()
	resString := unsafe.String(unsafe.SliceData(goRes), len(goRes))
	res.value = unsafe.Pointer(&resString)
}

//export progpCgoBinding__10
func progpCgoBinding__10(res *C.ProgpFunctionReturnVoid, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsProcessExit(int(p0))

}

//export progpCgoBinding__14
func progpCgoBinding__14(res *C.ProgpFunctionReturnLong) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessGetUid()
	res.value = C.long(goRes)
}

//export progpCgoBinding__4
func progpCgoBinding__4(res *C.ProgpFunctionReturnVoid, p0 C.double, p1 C.double) {
	defer progpAPI.CatchFatalErrors()

	err := modNodeJs.JsProcessKill(int(p0), int(p1))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__15
func progpCgoBinding__15(res *C.ProgpFunctionReturnVoid, p0 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modNodeJs.JsProcessNextTickAsync(newV8Function(res.isAsync, p0, res.currentEvent))

}

//export progpCgoBinding__11
func progpCgoBinding__11(res *C.ProgpFunctionReturnLong) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessPID()
	res.value = C.long(goRes)
}

//export progpCgoBinding__12
func progpCgoBinding__12(res *C.ProgpFunctionReturnLong) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessParentPID()
	res.value = C.long(goRes)
}

//export progpCgoBinding__8
func progpCgoBinding__8(res *C.ProgpFunctionReturnString) {
	defer progpAPI.CatchFatalErrors()

	goRes := modNodeJs.JsProcessPlatform()
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__52
func progpCgoBinding__52(res *C.ProgpFunctionReturnInt, p0 C.double, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp1 := C.GoBytes(unsafe.Pointer(p1.p), p1.n)
	var vp1 httpServer.StartParams
	if err := json.Unmarshal(bp1, &vp1); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	goRes := modHttp.JsConfigureServer(int(p0), vp1)
	if goRes {
		res.value = C.int(1)
	} else {
		res.value = C.int(0)
	}
}

//export progpCgoBinding__53
func progpCgoBinding__53(res *C.ProgpFunctionReturnLong, p1 C.double, p2 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	goRes := modHttp.JsGetHost(getSharedResourceContainerFromUIntPtr(res.currentEvent.id), int(p1), C.GoStringN(p2.p, p2.n))
	res.value = C.long(goRes.GetId())
}

//export progpCgoBinding__68
func progpCgoBinding__68(res *C.ProgpFunctionReturnArrayBuffer, p0 C.double, p1 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestCookie(resolveSharedResourceFromDouble(res.currentEvent.id, p0), C.GoStringN(p1.p, p1.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__67
func progpCgoBinding__67(res *C.ProgpFunctionReturnArrayBuffer, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestCookies(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__69
func progpCgoBinding__69(res *C.ProgpFunctionReturnArrayBuffer, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestHeaders(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__60
func progpCgoBinding__60(res *C.ProgpFunctionReturnString, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestHost(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__58
func progpCgoBinding__58(res *C.ProgpFunctionReturnString, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestIP(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__59
func progpCgoBinding__59(res *C.ProgpFunctionReturnString, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestMethod(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__57
func progpCgoBinding__57(res *C.ProgpFunctionReturnString, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestPath(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__62
func progpCgoBinding__62(res *C.ProgpFunctionReturnArrayBuffer, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestPostArgs(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__61
func progpCgoBinding__61(res *C.ProgpFunctionReturnArrayBuffer, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestQueryArgs(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__63
func progpCgoBinding__63(res *C.ProgpFunctionReturnVoid, p0 C.double, p1 *C.s_progp_goStringOut, p2 C.double, p3 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modHttp.JsRequestReadFormFileAsync(resolveSharedResourceFromDouble(res.currentEvent.id, p0), C.GoStringN(p1.p, p1.n), int(p2), newV8Function(res.isAsync, p3, res.currentEvent))

}

//export progpCgoBinding__66
func progpCgoBinding__66(res *C.ProgpFunctionReturnArrayBuffer, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestRemainingSegments(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__64
func progpCgoBinding__64(res *C.ProgpFunctionReturnVoid, p0 C.double, p1 *C.s_progp_goStringOut, p2 C.double, p3 *C.s_progp_goStringOut, p4 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	modHttp.JsRequestSaveFormFileAsync(resolveSharedResourceFromDouble(res.currentEvent.id, p0), C.GoStringN(p1.p, p1.n), int(p2), C.GoStringN(p3.p, p3.n), newV8Function(res.isAsync, p4, res.currentEvent))

}

//export progpCgoBinding__71
func progpCgoBinding__71(res *C.ProgpFunctionReturnVoid, p0 C.double, p1 *C.s_progp_goStringOut, p2 *C.s_progp_goStringOut, p3 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	bp3 := C.GoBytes(unsafe.Pointer(p3.p), p3.n)
	var vp3 httpServer.HttpCookieOptions
	if err := json.Unmarshal(bp3, &vp3); err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}

	err := modHttp.JsRequestSetCookie(resolveSharedResourceFromDouble(res.currentEvent.id, p0), C.GoStringN(p1.p, p1.n), C.GoStringN(p2.p, p2.n), vp3)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__70
func progpCgoBinding__70(res *C.ProgpFunctionReturnVoid, p0 C.double, p1 *C.s_progp_goStringOut, p2 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modHttp.JsRequestSetHeader(resolveSharedResourceFromDouble(res.currentEvent.id, p0), C.GoStringN(p1.p, p1.n), C.GoStringN(p2.p, p2.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__56
func progpCgoBinding__56(res *C.ProgpFunctionReturnString, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestURI(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	res.value = unsafe.Pointer(&goRes)
}

//export progpCgoBinding__65
func progpCgoBinding__65(res *C.ProgpFunctionReturnArrayBuffer, p0 C.double) {
	defer progpAPI.CatchFatalErrors()

	err, goRes := modHttp.JsRequestWildcards(resolveSharedResourceFromDouble(res.currentEvent.id, p0))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
	asBytes, err := json.Marshal(goRes)

	if err != nil {
		res.errorMessage = C.CString(err.Error())
	} else {
		res.value = unsafe.Pointer(&asBytes[0])
		res.size = C.int(len(asBytes))
	}
}

//export progpCgoBinding__55
func progpCgoBinding__55(res *C.ProgpFunctionReturnVoid, p0 C.double, p1 C.double, p2 *C.s_progp_goStringOut, p3 *C.s_progp_goStringOut) {
	defer progpAPI.CatchFatalErrors()

	err := modHttp.JsReturnString(resolveSharedResourceFromDouble(res.currentEvent.id, p0), int(p1), C.GoStringN(p2.p, p2.n), C.GoStringN(p3.p, p3.n))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__51
func progpCgoBinding__51(res *C.ProgpFunctionReturnVoid, p1 C.double) {
	defer progpAPI.CatchFatalErrors()

	err := modHttp.JsStartServer(getSharedResourceContainerFromUIntPtr(res.currentEvent.id), int(p1))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}

//export progpCgoBinding__54
func progpCgoBinding__54(res *C.ProgpFunctionReturnVoid, p1 C.double, p2 *C.s_progp_goStringOut, p3 *C.s_progp_goStringOut, p4 C.ProgpV8FunctionPtr) {
	defer progpAPI.CatchFatalErrors()

	err := modHttp.JsVerbWithFunction(getSharedResourceContainerFromUIntPtr(res.currentEvent.id), resolveSharedResourceFromDouble(res.currentEvent.id, p1), C.GoStringN(p2.p, p2.n), C.GoStringN(p3.p, p3.n), newV8Function(res.isAsync, p4, res.currentEvent))

	if err != nil {
		res.errorMessage = C.CString(err.Error())
		return
	}
}
