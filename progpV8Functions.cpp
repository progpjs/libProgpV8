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

#include "progpV8.h"
#include "v8.h"

#include <iostream>
#include <memory>

void JsProgpPrint(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
    PROGP_V8FUNCTION_BEFORE_PROGPCTX

        int argsCount = callInfo.Length();

        for (int i = 0; i < argsCount; i++) {
            auto v = callInfo[i];
            if (v.IsEmpty()) continue;
            if (v->IsExternal()) continue;

            if (v->IsObject() || v->IsArray()) {
                v8::Local<v8::String> gap_p0;
                v8::Local<v8::String> asString;

                if (v8::JSON::Stringify(v8Ctx, v, gap_p0).ToLocal(&asString)) {
                    v = asString;
                }
            }

            // Will throw an exception if a special type.
            try { std::cout << V8VALUE_TO_CSTRING(v); }
            catch (std::exception &e) {}
        }

        std::cout << std::endl;
        callInfo.GetReturnValue().SetUndefined();

    PROGP_V8FUNCTION_AFTER
}

void JsProgpGetModule(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
    PROGP_V8FUNCTION_BEFORE_PROGPCTX

        V8CALLARG_EXPECT_CSTRING(modName, 0);

        auto v8Object = v8::Object::New(v8Iso);
        progp_CreateFunctionGroup(progpCtx, modName, v8Object);
        callInfo.GetReturnValue().Set(v8Object);

    PROGP_V8FUNCTION_AFTER
}

void JsProgpStringToBuffer(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
    PROGP_V8FUNCTION_BEFORE

        V8CALLARG_EXPECT_ARGCOUNT(1)

        if (!callInfo[0]->IsString()) {
            throw std::runtime_error("invalid param type, expect string");
        }

        auto arg0 = callInfo[0].As<v8::String>();

        v8::Local<v8::ArrayBuffer> v8Buffer = v8::ArrayBuffer::New(v8Iso, arg0->Length());
        arg0->WriteUtf8(v8Iso, (char*)v8Buffer->GetBackingStore()->Data());

        callInfo.GetReturnValue().Set(v8Buffer);

    PROGP_V8FUNCTION_AFTER
}

void JsProgpBufferToString(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
    PROGP_V8FUNCTION_BEFORE

        V8CALLARG_EXPECT_ARGCOUNT(1)

        V8CALLARG_EXPECT_ARRAYBUFFER(arg0, 0);
        auto res = v8::String::NewFromUtf8(v8Iso, (char*)arg0->Data(), v8::NewStringType::kNormal, (int)arg0->ByteLength()).ToLocalChecked();
        callInfo.GetReturnValue().Set(res);

    PROGP_V8FUNCTION_AFTER
}

void progp_CreateFunctionGroup_Internal(ProgpContext progpCtx, const std::string& group, v8::Local<v8::Object> v8Host) {
    PROGP_BIND_FUNCTION("global", "progpPrint", JsProgpPrint);
    PROGP_BIND_FUNCTION("global", "progpGetModule", JsProgpGetModule);
    PROGP_BIND_FUNCTION("global", "progpStringToBuffer", JsProgpStringToBuffer);
    PROGP_BIND_FUNCTION("global", "progpBufferToString", JsProgpBufferToString);
}