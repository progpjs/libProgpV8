#ifndef PROGP_STANDALONE

#include "progpV8.h"
#include "_cgo_export.h"
#include <iostream>
#include <stdexcept>


void v8Function_1(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_FUNCTION(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__1(&resWrapper, p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_86(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);
    V8CALLARG_EXPECT_ARRAYBUFFER_DATA(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__86(&resWrapper, p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_82(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);
    V8CALLARG_EXPECT_BOOL(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__82(&resWrapper, p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_83(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);
    V8CALLARG_EXPECT_DOUBLE(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__83(&resWrapper, p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_84(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__84(&resWrapper, p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_81(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__81(&resWrapper, p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_87(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__87(&resWrapper, p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_85(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__85(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_88(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_FUNCTION(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__88(&resWrapper, &p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_76(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_BOOL(p0, 0);

	ProgpFunctionReturnInt resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__76(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_BOOL(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_80(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_ARRAYBUFFER_DATA(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__80(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(v8Iso, resWrapper.size);
    memcpy(buffer->GetBackingStore()->Data(), res, resWrapper.size);
    callInfo.GetReturnValue().Set(buffer);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_94(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_FUNCTION(p1, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__94(&resWrapper, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_72(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnDouble resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__72(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_71(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnDouble resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__71(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_75(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnLong resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__75(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_73(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__73(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_74(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__74(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_92(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__92(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_93(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnLong resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__93(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_70(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__70(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_79(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__79(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_78(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__78(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_77(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__77(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_89(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__89(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_90(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__90(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_91(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__91(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_22(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_DOUBLE(p1, 1);
    V8CALLARG_EXPECT_FUNCTION(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__22(&resWrapper, &p0, p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_21(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_DOUBLE(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__21(&resWrapper, &p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_46(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_ARRAYBUFFER_DATA(p1, 1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p2, 2);
    V8CALLARG_EXPECT_DOUBLE(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__46(&resWrapper, &p0, p1, &p2, p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_45(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p2, 2);
    V8CALLARG_EXPECT_DOUBLE(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__45(&resWrapper, &p0, &p1, &p2, p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_24(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p1, 1);
    V8CALLARG_EXPECT_FUNCTION(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__24(&resWrapper, &p0, &p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_23(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__23(&resWrapper, &p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_26(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_DOUBLE(p1, 1);
    V8CALLARG_EXPECT_DOUBLE(p2, 2);
    V8CALLARG_EXPECT_FUNCTION(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__26(&resWrapper, &p0, p1, p2, p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_25(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_DOUBLE(p1, 1);
    V8CALLARG_EXPECT_DOUBLE(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__25(&resWrapper, &p0, p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_30(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_FUNCTION(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__30(&resWrapper, &p0, &p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_29(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__29(&resWrapper, &p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_18(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_FUNCTION(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__18(&resWrapper, &p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_17(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnInt resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__17(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_BOOL(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_32(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_FUNCTION(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__32(&resWrapper, &p0, &p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_31(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__31(&resWrapper, &p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_38(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_BOOL(p1, 1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p2, 2);
    V8CALLARG_EXPECT_FUNCTION(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__38(&resWrapper, &p0, p1, &p2, p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_37(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_BOOL(p1, 1);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__37(&resWrapper, &p0, p1, &p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_40(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_FUNCTION(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__40(&resWrapper, &p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_39(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__39(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_42(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__42(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(v8Iso, resWrapper.size);
    memcpy(buffer->GetBackingStore()->Data(), res, resWrapper.size);
    callInfo.GetReturnValue().Set(buffer);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_41(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__41(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_47(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__47(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_48(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__48(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_43(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__43(&resWrapper, &p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_44(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_BOOL(p1, 1);
    V8CALLARG_EXPECT_BOOL(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__44(&resWrapper, &p0, p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_20(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_BOOL(p1, 1);
    V8CALLARG_EXPECT_FUNCTION(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__20(&resWrapper, &p0, p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_19(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_BOOL(p1, 1);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__19(&resWrapper, &p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_34(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_FUNCTION(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__34(&resWrapper, &p0, &p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_33(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__33(&resWrapper, &p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_28(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p1, 1);
    V8CALLARG_EXPECT_FUNCTION(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__28(&resWrapper, &p0, &p1, p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_27(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__27(&resWrapper, &p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_36(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);
    V8CALLARG_EXPECT_FUNCTION(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__36(&resWrapper, &p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_35(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__35(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_14(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__14(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_15(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__15(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_16(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__16(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_5(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__5(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_7(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__7(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_11(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_GOSTRING(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__11(&resWrapper, &p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_3(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__3(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_4(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__4(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_8(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__8(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_12(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnLong resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__12(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_2(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_DOUBLE(p1, 1);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__2(&resWrapper, p0, p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_13(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_FUNCTION(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__13(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_9(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnLong resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__9(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_10(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnLong resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__10(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_6(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX


	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__6(&resWrapper);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_50(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p1, 1);

	ProgpFunctionReturnInt resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__50(&resWrapper, p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_BOOL(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_51(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnLong resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__51(&resWrapper, p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_DOUBLE(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_66(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(2);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__66(&resWrapper, p0, &p1);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_65(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__65(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_67(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__67(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_58(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__58(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_56(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__56(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_57(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__57(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_55(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__55(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_60(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__60(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_59(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__59(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_61(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_DOUBLE(p2, 2);
    V8CALLARG_EXPECT_FUNCTION(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__61(&resWrapper, p0, &p1, p2, p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_64(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__64(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_62(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(5);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_DOUBLE(p2, 2);
    V8CALLARG_EXPECT_GOSTRING(p3, 3);
    V8CALLARG_EXPECT_FUNCTION(p4, 4);

	ProgpFunctionReturnVoid resWrapper{};

    progp_IncreaseContextRef();
    resWrapper.isAsync = true;
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__62(&resWrapper, p0, &p1, p2, &p3, p4);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_69(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_GOSTRING(p2, 2);
    V8CALLARG_EXPECT_V8OBJECT_TOSTRING(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__69(&resWrapper, p0, &p1, &p2, &p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_68(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(3);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_GOSTRING(p2, 2);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__68(&resWrapper, p0, &p1, &p2);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_54(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnString resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__54(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    callInfo.GetReturnValue().Set(V8VALUE_FROM_GOSTRING(res));
	PROGP_V8FUNCTION_AFTER
}

void v8Function_63(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnArrayBuffer resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__63(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    auto res = resWrapper.value;
    v8::Local<v8::Value> v8Res;
    V8VALUE_FROM_GOCUSTOM(v8Res, res, resWrapper.size);
    callInfo.GetReturnValue().Set(v8Res);
	PROGP_V8FUNCTION_AFTER
}

void v8Function_53(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_DOUBLE(p1, 1);
    V8CALLARG_EXPECT_GOSTRING(p2, 2);
    V8CALLARG_EXPECT_GOSTRING(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__53(&resWrapper, p0, p1, &p2, &p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_49(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(1);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__49(&resWrapper, p0);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void v8Function_52(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
	PROGP_V8FUNCTION_BEFORE_PROGPCTX

    V8CALLARG_EXPECT_ARGCOUNT(4);
    V8CALLARG_EXPECT_DOUBLE(p0, 0);
    V8CALLARG_EXPECT_GOSTRING(p1, 1);
    V8CALLARG_EXPECT_GOSTRING(p2, 2);
    V8CALLARG_EXPECT_FUNCTION(p3, 3);

	ProgpFunctionReturnVoid resWrapper{};
	resWrapper.currentEvent = progpCtx->event;
	progpCgoBinding__52(&resWrapper, p0, &p1, &p2, p3);
	
    if (resWrapper.errorMessage!=nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
		delete(resWrapper.errorMessage);
        throw std::runtime_error(msg.c_str());
    } else if (resWrapper.constErrorMessage!= nullptr) {
		auto msg = std::string(resWrapper.errorMessage);
        throw std::runtime_error(resWrapper.errorMessage);
    }

    callInfo.GetReturnValue().SetUndefined();
	PROGP_V8FUNCTION_AFTER
}

void exposeGoFunctionsToV8(const std::string& group, v8::Local<v8::Object> v8Host) {
    PROGP_BIND_FUNCTION("global", "progpCallAfterMs", (f_progp_v8_function)v8Function_1);
    PROGP_BIND_FUNCTION("global", "testCallbackWithArrayBuffer", (f_progp_v8_function)v8Function_86);
    PROGP_BIND_FUNCTION("global", "testCallbackWithBool", (f_progp_v8_function)v8Function_82);
    PROGP_BIND_FUNCTION("global", "testCallbackWithDouble", (f_progp_v8_function)v8Function_83);
    PROGP_BIND_FUNCTION("global", "testCallbackWithError", (f_progp_v8_function)v8Function_84);
    PROGP_BIND_FUNCTION("global", "testCallbackWithString", (f_progp_v8_function)v8Function_81);
    PROGP_BIND_FUNCTION("global", "testCallbackWithStringBuffer", (f_progp_v8_function)v8Function_87);
    PROGP_BIND_FUNCTION("global", "testCallbackWithoutValues", (f_progp_v8_function)v8Function_85);
    PROGP_BIND_FUNCTION("global", "testAsync", (f_progp_v8_function)v8Function_88);
    PROGP_BIND_FUNCTION("global", "testBool", (f_progp_v8_function)v8Function_76);
    PROGP_BIND_FUNCTION("global", "testByteArray", (f_progp_v8_function)v8Function_80);
    PROGP_BIND_FUNCTION("global", "testEventFunction", (f_progp_v8_function)v8Function_94);
    PROGP_BIND_FUNCTION("global", "testFloat32", (f_progp_v8_function)v8Function_72);
    PROGP_BIND_FUNCTION("global", "testFloat64", (f_progp_v8_function)v8Function_71);
    PROGP_BIND_FUNCTION("global", "testInt", (f_progp_v8_function)v8Function_75);
    PROGP_BIND_FUNCTION("global", "testInt32", (f_progp_v8_function)v8Function_73);
    PROGP_BIND_FUNCTION("global", "testInt64", (f_progp_v8_function)v8Function_74);
    PROGP_BIND_FUNCTION("global", "testReceiveSharedResource", (f_progp_v8_function)v8Function_92);
    PROGP_BIND_FUNCTION("global", "testReturnSharedResource", (f_progp_v8_function)v8Function_93);
    PROGP_BIND_FUNCTION("global", "testString", (f_progp_v8_function)v8Function_70);
    PROGP_BIND_FUNCTION("global", "testStringArray", (f_progp_v8_function)v8Function_79);
    PROGP_BIND_FUNCTION("global", "testStructPtr", (f_progp_v8_function)v8Function_78);
    PROGP_BIND_FUNCTION("global", "testStructRef", (f_progp_v8_function)v8Function_77);
    PROGP_BIND_FUNCTION("global", "testThrowError", (f_progp_v8_function)v8Function_89);
    PROGP_BIND_FUNCTION("global", "testThrowErrorP1", (f_progp_v8_function)v8Function_90);
    PROGP_BIND_FUNCTION("global", "testThrowErrorP2", (f_progp_v8_function)v8Function_91);
    PROGP_BIND_FUNCTION("nodejsModFS", "accessAsync", (f_progp_v8_function)v8Function_22);
    PROGP_BIND_FUNCTION("nodejsModFS", "accessSync", (f_progp_v8_function)v8Function_21);
    PROGP_BIND_FUNCTION("nodejsModFS", "appendFileSyncBytes", (f_progp_v8_function)v8Function_46);
    PROGP_BIND_FUNCTION("nodejsModFS", "appendFileSyncText", (f_progp_v8_function)v8Function_45);
    PROGP_BIND_FUNCTION("nodejsModFS", "chmodAsync", (f_progp_v8_function)v8Function_24);
    PROGP_BIND_FUNCTION("nodejsModFS", "chmodSync", (f_progp_v8_function)v8Function_23);
    PROGP_BIND_FUNCTION("nodejsModFS", "chownAsync", (f_progp_v8_function)v8Function_26);
    PROGP_BIND_FUNCTION("nodejsModFS", "chownSync", (f_progp_v8_function)v8Function_25);
    PROGP_BIND_FUNCTION("nodejsModFS", "copyFileAsync", (f_progp_v8_function)v8Function_30);
    PROGP_BIND_FUNCTION("nodejsModFS", "copyFileSync", (f_progp_v8_function)v8Function_29);
    PROGP_BIND_FUNCTION("nodejsModFS", "existsASync", (f_progp_v8_function)v8Function_18);
    PROGP_BIND_FUNCTION("nodejsModFS", "existsSync", (f_progp_v8_function)v8Function_17);
    PROGP_BIND_FUNCTION("nodejsModFS", "linkAsync", (f_progp_v8_function)v8Function_32);
    PROGP_BIND_FUNCTION("nodejsModFS", "linkSync", (f_progp_v8_function)v8Function_31);
    PROGP_BIND_FUNCTION("nodejsModFS", "mkdirAsync", (f_progp_v8_function)v8Function_38);
    PROGP_BIND_FUNCTION("nodejsModFS", "mkdirSync", (f_progp_v8_function)v8Function_37);
    PROGP_BIND_FUNCTION("nodejsModFS", "mkdtempAsync", (f_progp_v8_function)v8Function_40);
    PROGP_BIND_FUNCTION("nodejsModFS", "mkdtempSync", (f_progp_v8_function)v8Function_39);
    PROGP_BIND_FUNCTION("nodejsModFS", "readFileBytesSync", (f_progp_v8_function)v8Function_42);
    PROGP_BIND_FUNCTION("nodejsModFS", "readFileUtf8Sync", (f_progp_v8_function)v8Function_41);
    PROGP_BIND_FUNCTION("nodejsModFS", "readlinkSync", (f_progp_v8_function)v8Function_47);
    PROGP_BIND_FUNCTION("nodejsModFS", "realpath", (f_progp_v8_function)v8Function_48);
    PROGP_BIND_FUNCTION("nodejsModFS", "renameSync", (f_progp_v8_function)v8Function_43);
    PROGP_BIND_FUNCTION("nodejsModFS", "rmSync", (f_progp_v8_function)v8Function_44);
    PROGP_BIND_FUNCTION("nodejsModFS", "statAsync", (f_progp_v8_function)v8Function_20);
    PROGP_BIND_FUNCTION("nodejsModFS", "statSync", (f_progp_v8_function)v8Function_19);
    PROGP_BIND_FUNCTION("nodejsModFS", "symlinkAsync", (f_progp_v8_function)v8Function_34);
    PROGP_BIND_FUNCTION("nodejsModFS", "symlinkSync", (f_progp_v8_function)v8Function_33);
    PROGP_BIND_FUNCTION("nodejsModFS", "truncateAsync", (f_progp_v8_function)v8Function_28);
    PROGP_BIND_FUNCTION("nodejsModFS", "truncateSync", (f_progp_v8_function)v8Function_27);
    PROGP_BIND_FUNCTION("nodejsModFS", "unlinkAsync", (f_progp_v8_function)v8Function_36);
    PROGP_BIND_FUNCTION("nodejsModFS", "unlinkSync", (f_progp_v8_function)v8Function_35);
    PROGP_BIND_FUNCTION("nodejsModOS", "homeDir", (f_progp_v8_function)v8Function_14);
    PROGP_BIND_FUNCTION("nodejsModOS", "hostName", (f_progp_v8_function)v8Function_15);
    PROGP_BIND_FUNCTION("nodejsModOS", "tempDir", (f_progp_v8_function)v8Function_16);
    PROGP_BIND_FUNCTION("nodejsModProcess", "arch", (f_progp_v8_function)v8Function_5);
    PROGP_BIND_FUNCTION("nodejsModProcess", "argv", (f_progp_v8_function)v8Function_7);
    PROGP_BIND_FUNCTION("nodejsModProcess", "chdir", (f_progp_v8_function)v8Function_11);
    PROGP_BIND_FUNCTION("nodejsModProcess", "cwd", (f_progp_v8_function)v8Function_3);
    PROGP_BIND_FUNCTION("nodejsModProcess", "env", (f_progp_v8_function)v8Function_4);
    PROGP_BIND_FUNCTION("nodejsModProcess", "exit", (f_progp_v8_function)v8Function_8);
    PROGP_BIND_FUNCTION("nodejsModProcess", "getuid", (f_progp_v8_function)v8Function_12);
    PROGP_BIND_FUNCTION("nodejsModProcess", "kill", (f_progp_v8_function)v8Function_2);
    PROGP_BIND_FUNCTION("nodejsModProcess", "nextTick", (f_progp_v8_function)v8Function_13);
    PROGP_BIND_FUNCTION("nodejsModProcess", "pid", (f_progp_v8_function)v8Function_9);
    PROGP_BIND_FUNCTION("nodejsModProcess", "ppid", (f_progp_v8_function)v8Function_10);
    PROGP_BIND_FUNCTION("nodejsModProcess", "platform", (f_progp_v8_function)v8Function_6);
    PROGP_BIND_FUNCTION("progpjsModHttp", "configureServer", (f_progp_v8_function)v8Function_50);
    PROGP_BIND_FUNCTION("progpjsModHttp", "getHost", (f_progp_v8_function)v8Function_51);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestCookie", (f_progp_v8_function)v8Function_66);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestCookies", (f_progp_v8_function)v8Function_65);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestHeaders", (f_progp_v8_function)v8Function_67);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestHost", (f_progp_v8_function)v8Function_58);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestIP", (f_progp_v8_function)v8Function_56);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestMethod", (f_progp_v8_function)v8Function_57);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestPath", (f_progp_v8_function)v8Function_55);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestPostArgs", (f_progp_v8_function)v8Function_60);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestQueryArgs", (f_progp_v8_function)v8Function_59);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestReadFormFile", (f_progp_v8_function)v8Function_61);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestRemainingSegments", (f_progp_v8_function)v8Function_64);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestSaveFormFile", (f_progp_v8_function)v8Function_62);
    PROGP_BIND_FUNCTION("progpjsModHttp", "responseSetCookie", (f_progp_v8_function)v8Function_69);
    PROGP_BIND_FUNCTION("progpjsModHttp", "responseSetHeader", (f_progp_v8_function)v8Function_68);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestURI", (f_progp_v8_function)v8Function_54);
    PROGP_BIND_FUNCTION("progpjsModHttp", "requestWildcards", (f_progp_v8_function)v8Function_63);
    PROGP_BIND_FUNCTION("progpjsModHttp", "returnString", (f_progp_v8_function)v8Function_53);
    PROGP_BIND_FUNCTION("progpjsModHttp", "startServer", (f_progp_v8_function)v8Function_49);
    PROGP_BIND_FUNCTION("progpjsModHttp", "VERB_withFunction", (f_progp_v8_function)v8Function_52);
}

#endif // PROGP_STANDALONE
