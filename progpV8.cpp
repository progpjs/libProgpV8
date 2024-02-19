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

#include "libplatform/libplatform.h"
#include "v8.h"
#include <memory>

#include <iostream>
#include <utility>
#include <thread>
#include <cstdlib>
#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

std::unique_ptr<v8::Platform> gV8Platform;
v8::ArrayBuffer::Allocator *gArrayBufferAllocator;
f_progp_v8_functions_provider gV8FunctionProvider = nullptr;
f_progp_v8_dynamicFunctions_provider gV8DynamicFunctionProvider = nullptr;
f_progp_v8_function_allowedFunctionChecker gV8AllowedFunctionChecker = nullptr;

ProgpContext gProgpDbgCtx;

void progp_DeclareGlobalFunctions(ProgpContext progpCtx);
void onJavascriptError(ProgpContext progpCtx, const v8::Local<v8::Context> &v8Ctx, v8::Local<v8::Message>& message);

//region Engine

void onProcessRejectedPromise(v8::PromiseRejectMessage reject_message) {
    auto v8Iso = reject_message.GetPromise()->GetIsolate();
    auto v8Ctx = v8Iso->GetCurrentContext();
    ProgpContext progpCtx = (ProgpContext) v8Ctx->GetEmbedderData(0).As<v8::External>()->Value();

    auto messageValue = reject_message.GetValue();

    if (!messageValue.IsEmpty()) {
        v8::Local<v8::Message> errorMessage = v8::Exception::CreateMessage(v8Iso, messageValue);
        onJavascriptError(progpCtx, v8Ctx, errorMessage);
    }
}

extern "C"
void progp_DisposeContext(ProgpContext progpCtx) {
    delete(progpCtx->event);

    if (progpCtx->v8Iso == v8::Isolate::GetCurrent()) {
        progpCtx->v8Iso->Exit();
    }
    progpCtx->v8Iso->Dispose();
    delete(progpCtx);
}

extern "C"
ProgpContext progp_CreateNewContext(uintptr_t data) {
    auto progpCtx = new s_progp_context();
    progpCtx->data = data;

    // Always having a event allow generalizing some mechanisms.
    // It's why we always bind an event to a ProgpContext.
    //
    progpCtx->event = new s_progp_event();
    progpCtx->event->id = 0;
    progpCtx->event->refCount = 0;
    progpCtx->event->previousEvent = nullptr;
    progpCtx->event->contextData = data;

    // Create the v8-isolate.
    {
        v8::Isolate::CreateParams params;
        params.array_buffer_allocator = gArrayBufferAllocator;

        progpCtx->v8Iso = v8::Isolate::New(params);

        // Allows having the stacktrace for the errors.
        progpCtx->v8Iso->SetCaptureStackTraceForUncaughtExceptions(true);

        // Allows knowing when a promise is rejected and not caught.
        progpCtx->v8Iso->SetPromiseRejectCallback(onProcessRejectedPromise);

        // Allow freeing memory when running out.
        progpCtx->v8Iso->LowMemoryNotification();
    }

    // Create the v8-context.
    {
        V8ISO_ACCESS(progpCtx);

        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(v8Iso);
        auto v8Ctx = v8::Context::New(v8Iso, nullptr, global);
        progpCtx->v8Ctx.Reset(v8Iso, v8Ctx);

        v8Ctx->SetEmbedderData(0, v8::External::New(v8Iso, progpCtx));
    }

    return progpCtx;
}

extern "C"
void progp_InitializeContext(ProgpContext progpCtx) {
    progp_DeclareGlobalFunctions(progpCtx);

    if (gProgpDbgCtx== nullptr) {
        gProgpDbgCtx = progpCtx;
    }
}

extern "C"
void progp_StartupEngine() {
    gV8Platform = v8::platform::NewDefaultPlatform();

    v8::V8::InitializePlatform(gV8Platform.get());
    v8::V8::Initialize();

    gArrayBufferAllocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

#ifndef PROGP_STANDALONE
    cgoInitialize();
#endif
}

extern "C"
const char* progp_GetV8EngineVersion() {
    return v8::V8::GetVersion();
}

//endregion

//region Executing scripts

f_progp_onNoMoreTasksForContext g_onNoMoreTask = nullptr;
f_progp_eventFinished g_onEventFinished = nullptr;

extern "C"
void progp_IncreaseContextRef(ProgpContext progpCtx) {
    //std::lock_guard autoUnlock(gContextRefCountMutex);

    // Here we are thread safe since caller use
    // a funnel doing that only one thread can speak to the VM.
    progpCtx->refCount++;

    if (progpCtx->event!= nullptr) {
        progpCtx->event->refCount++;
    }
}

extern "C"
void progp_DecreaseContextRef(ProgpContext progpCtx) {
    if (progpCtx->event!= nullptr) {
        progpCtx->event->refCount--;

        if (progpCtx->event->refCount==0) {
            auto evt = progpCtx->event;
            progpCtx->event = evt->previousEvent;
            if (g_onEventFinished!=nullptr) g_onEventFinished(evt->id);

            delete(evt);
        }
    }

    progpCtx->refCount--;

    if (progpCtx->refCount==0) {
        if (g_onNoMoreTask != nullptr) g_onNoMoreTask(progpCtx);
    }
}

extern "C"
bool progp_ExecuteScript(ProgpContext progpCtx, const char* scriptContent, const char* scriptOrigin, uintptr_t eventId) {
    progpCtx->event->id = eventId;
    progp_IncreaseContextRef(progpCtx);
    V8CTX_ACCESS();

    v8::ScriptOrigin v8ScriptOrigin(v8Iso, v8::String::NewFromUtf8(v8Iso, scriptOrigin).ToLocalChecked());
    v8::Local<v8::String> v8ScriptSourceAsText = v8::String::NewFromUtf8(v8Iso, scriptContent).ToLocalChecked();

    v8::TryCatch tryCatch(v8Iso);
    v8::Local<v8::Script> script;

    if (!v8::Script::Compile(v8Ctx, v8ScriptSourceAsText, &v8ScriptOrigin).ToLocal(&script)) {
        auto error = tryCatch.Message();
        onJavascriptError(progpCtx, v8Ctx, error);
        progp_DecreaseContextRef(progpCtx);
        return false;
    }

    v8::Local<v8::Value> result;
    //
    if (!script->Run(v8Ctx).ToLocal(&result)) {
        auto error = tryCatch.Message();
        onJavascriptError(progpCtx, v8Ctx, error);
        progp_DecreaseContextRef(progpCtx);
        return false;
    }

    progp_DecreaseContextRef(progpCtx);
    return true;
}

//endregion

//region Managing errors

f_progp_javascriptErrorListener gJavascriptErrorListener = nullptr;

void disposeErrorMessage(s_progp_v8_errorMessage *msg) {
    if (msg->error!= nullptr) free(msg->error);
    if (msg->sourceMapUrl!= nullptr) free(msg->sourceMapUrl);
    if (msg->resourceName!= nullptr) free(msg->resourceName);

    if (msg->stackTraceFrameArray!= nullptr) {
        int frameCount = msg->stackTraceFrameCount;
        auto array = msg->stackTraceFrameArray;
        msg->stackTraceFrameArray = nullptr;

        for (int i=0;i<frameCount;i++) {
            auto frame = &array[i];
            if (frame->function!= nullptr) free(frame->function);
            if (frame->source!= nullptr) free(frame->source);
        }
    }

    delete(msg);
}

static s_progp_v8_errorMessage* createErrorMessage(ProgpContext progpCtx, const v8::Local<v8::Context> &v8Ctx, v8::Local<v8::Message>& message) {
    auto res = new s_progp_v8_errorMessage();

    res->error = strdup(V8VALUE_TO_CSTRING(message->Get()));
    res->errorLevel = message->ErrorLevel();

    res->startColumn = message->GetStartColumn();
    res->endColumn = message->GetEndColumn();

    res->startPosition = message->GetStartPosition();
    res->endPosition = message->GetEndPosition();

    auto v8ScriptOrigin = message->GetScriptOrigin();

    if (!v8ScriptOrigin.SourceMapUrl().IsEmpty()) {
        res->sourceMapUrl = strdup(V8VALUE_TO_CSTRING(v8ScriptOrigin.SourceMapUrl()->ToString(v8Ctx).ToLocalChecked()));
    }

    if (!v8ScriptOrigin.ResourceName().IsEmpty()) {
        res->resourceName = strdup(V8VALUE_TO_CSTRING(v8ScriptOrigin.ResourceName()->ToString(v8Ctx).ToLocalChecked()));
    }

    auto v8StackTrace = message->GetStackTrace();

    if (!v8StackTrace.IsEmpty()) {
        auto stackTraceFrameCount = v8StackTrace->GetFrameCount();

        auto frames = (s_progp_v8_stackTraceFrame*)malloc(sizeof(s_progp_v8_stackTraceFrame) * stackTraceFrameCount);
        res->stackTraceFrameArray = frames;
        res->stackTraceFrameCount = stackTraceFrameCount;
        res->stackTraceFrameSize = sizeof(s_progp_v8_stackTraceFrame);

        for (auto frameOffset = 0 ; frameOffset<stackTraceFrameCount ; frameOffset++) {
            auto v8StackFrame = v8StackTrace->GetFrame(progpCtx->v8Iso, frameOffset);
            auto frame = &frames[frameOffset];

            frame->line = v8StackFrame->GetLineNumber();
            frame->column = v8StackFrame->GetColumn();

            auto v8Function = v8StackFrame->GetFunctionName();
            if (!v8Function.IsEmpty()) frame->function = strdup(V8VALUE_TO_CSTRING(v8Function->ToString(v8Ctx).ToLocalChecked()));

            auto v8Source = v8StackFrame->GetScriptNameOrSourceURL();
            if (!v8Source.IsEmpty()) frame->source = strdup(V8VALUE_TO_CSTRING(v8Source->ToString(v8Ctx).ToLocalChecked()));
        }
    }

    return res;
}

void onJavascriptError(ProgpContext progpCtx, const v8::Local<v8::Context> &v8Ctx, v8::Local<v8::Message>& message) {
    //PROGP_DEBUG("Calling onJavascriptError from " << caller);

    if (gJavascriptErrorListener != nullptr) {
        auto msg = createErrorMessage(progpCtx, v8Ctx, message);
        gJavascriptErrorListener(progpCtx, msg);
        disposeErrorMessage(msg);
    }
}

void progp_PrintErrorMessage(s_progp_v8_errorMessage* msg) {
    PROGP_PRINT(msg->error);

    for (int i=0;i<msg->stackTraceFrameCount;i++) {
        auto frame = msg->stackTraceFrameArray[i];

        if (frame.function!= nullptr) {
            PROGP_PRINT(frame.source << ":" << frame.line << ":" << frame.column << " - " << frame.function);
        }
    }
}

//endregion

//region Functions: declaring news functions

void progp_AddFunctionToObject(ProgpContext progpCtx, const char* groupName, const v8::Local<v8::Object> &v8Object, const char* functionName, f_progp_v8_function functionRef) {
    // Detect if the function is allowed.
    //
    if (gV8AllowedFunctionChecker!= nullptr) {
        std::string securityGroup;
        if (!gV8AllowedFunctionChecker((char*)securityGroup.c_str(), (char*)groupName, (char*)functionName)) return;
    }

    V8CTX_ACCESS();

    auto v8FunctionTemplate = v8::FunctionTemplate::New(v8Iso, functionRef);

    v8::Local<v8::String> v8PropName = v8::String::NewFromUtf8(v8Iso, functionName,
                                                               v8::NewStringType::kNormal).ToLocalChecked();

    auto v8Function = v8FunctionTemplate->GetFunction(v8Ctx).ToLocalChecked();
    v8Object->Set(v8Ctx, v8PropName, v8Function).IsJust();

    v8FunctionTemplate.Clear();
}

const v8::Local<v8::Object>* gCurrentFunctionGroup;

void progp_CreateFunctionGroup(ProgpContext progpCtx, const std::string& group, const v8::Local<v8::Object> &v8Object) {
    progp_CreateFunctionGroup_Internal(progpCtx, group, v8Object);

    if (gV8FunctionProvider!= nullptr) {
        gV8FunctionProvider(progpCtx, group, v8Object);
    }

    if (gV8DynamicFunctionProvider!= nullptr) {
        gCurrentFunctionGroup = &v8Object;

        // Will call progp_DeclareDynamicFunction.
        gV8DynamicFunctionProvider(progpCtx, (char*)group.c_str());

        gCurrentFunctionGroup = nullptr;
    }
}

extern "C"
void progp_DeclareDynamicFunction(ProgpContext progpCtx, const char* functionName) {
    if (gCurrentFunctionGroup== nullptr) {
        return;
    }

    V8CTX_ACCESS();

    auto vFctName = CSTRING_TO_V8VALUE(functionName);
    auto v8FunctionTemplate = v8::FunctionTemplate::New(v8Iso, progp_handleDraftFunction, vFctName);

    v8::Local<v8::String> v8PropName = v8::String::NewFromUtf8(v8Iso, functionName,
                                                               v8::NewStringType::kNormal).ToLocalChecked();

    auto v8Function = v8FunctionTemplate->GetFunction(v8Ctx).ToLocalChecked();

    (*gCurrentFunctionGroup)->Set(v8Ctx, v8PropName, v8Function).IsJust();

    v8FunctionTemplate.Clear();
}

void progp_DeclareGlobalFunctions(ProgpContext progpCtx) {
    V8CTX_ACCESS();

    progp_CreateFunctionGroup(progpCtx, "global", v8Ctx->Global());
}

//endregion

//region Functions: ref to javascript function

s_progp_v8_function* progpFunctions_NewPointer(ProgpContext progpCtx, const v8::Local<v8::Function> &v8Function) {
    auto res = new s_progp_v8_function();
    res->progpCtx = progpCtx;
    res->ref.Reset(progpCtx->v8Iso, v8Function);
    return res;
}

//endregion

//region Functions: call from external / callbacks

#define FCT_CALLBACK_BEFORE \
    auto progpCtx = functionRef->progpCtx; \
    V8CTX_ACCESS(); \
    if (eventToRestore!=nullptr) progpCtx->event = eventToRestore; \
    if (resourceContainerId!=0) useNewEvent(progpCtx, resourceContainerId); \
    v8::TryCatch tryCatch(v8Iso);

#define FCT_CALLBACK_AFTER \
    if (isEmpty) { \
        if (tryCatch.HasCaught()) { \
            auto catchError = tryCatch.Message(); \
            onJavascriptError(progpCtx, v8Ctx, catchError); \
        } \
    } \
    \
    if (mustDisposeFunction) delete(functionRef); \
    if (resourceContainerId!=0) progp_DecreaseContextRef(progpCtx); \
    if (mustDecreaseTaskCount) progp_DecreaseContextRef(progpCtx);

inline void useNewEvent(ProgpContext progpCtx, uintptr_t resourceContainerId) {
    auto newEvent = new s_progp_event();
    newEvent->id = resourceContainerId;
    newEvent->refCount = 0;
    newEvent->previousEvent = nullptr;
    newEvent->contextData = progpCtx->data;

    newEvent->previousEvent = progpCtx->event;
    progpCtx->event = newEvent;

    // Require, without that the ref counter is 0
    // and will never go from 1 to 0.
    //
    progp_IncreaseContextRef(progpCtx);
}

extern "C"
void progp_CallFunctionWithStringP2(FCT_CALLBACK_PARAMS, const char* str, size_t strLen) {
    FCT_CALLBACK_BEFORE

    v8::Local<v8::Value> argArray[2];
    argArray[0] = v8::Undefined(v8Iso);
    argArray[1] = v8::String::NewFromUtf8(v8Iso, str, v8::NewStringType::kNormal, (int)strLen).ToLocalChecked();

    auto isEmpty = functionRef->ref.Get(v8Iso)->Call(v8Ctx, v8Ctx->Global(), 2, argArray).IsEmpty();

    FCT_CALLBACK_AFTER
}

extern "C"
void progp_CallFunctionWithArrayBufferP2(FCT_CALLBACK_PARAMS, const void* buffer, size_t bufferSize) {
    FCT_CALLBACK_BEFORE

    auto v8BackingStore = std::shared_ptr(v8::ArrayBuffer::NewBackingStore(v8Iso, bufferSize));
    memcpy(v8BackingStore->Data(), buffer, bufferSize);

    // Required for buffer allocation.
    v8Ctx->Enter();

    v8::Local<v8::Value> argArray[2];
    argArray[0] = v8::Undefined(v8Iso);
    argArray[1] = v8::ArrayBuffer::New(v8Iso, v8BackingStore);

    auto isEmpty = functionRef->ref.Get(v8Iso)->Call(v8Ctx, v8Ctx->Global(), 2, argArray).IsEmpty();

    FCT_CALLBACK_AFTER
}

extern "C"
void progp_CallFunctionWithErrorP1(FCT_CALLBACK_PARAMS, const char* str, size_t strLen) {
    FCT_CALLBACK_BEFORE

    v8::Local<v8::Value> argArray[1];
    argArray[0] = v8::String::NewFromUtf8(v8Iso, str, v8::NewStringType::kNormal, (int)strLen).ToLocalChecked();

    auto isEmpty = functionRef->ref.Get(v8Iso)->Call(v8Ctx, v8Ctx->Global(), 1, argArray).IsEmpty();

    FCT_CALLBACK_AFTER
}

extern "C"
void progp_CallFunctionWithDoubleP1(FCT_CALLBACK_PARAMS, double value) {
    FCT_CALLBACK_BEFORE

    v8::Local<v8::Value> argArray[1];
    argArray[0] = DOUBLE_TO_V8VALUE(value);

    auto isEmpty = functionRef->ref.Get(v8Iso)->Call(v8Ctx, v8Ctx->Global(), 1, argArray).IsEmpty();

    FCT_CALLBACK_AFTER
}

extern "C"
void progp_CallFunctionWithDoubleP2(FCT_CALLBACK_PARAMS, double value) {
    FCT_CALLBACK_BEFORE

    v8::Local<v8::Value> argArray[2];
    argArray[0] = v8::Undefined(v8Iso);
    argArray[1] = DOUBLE_TO_V8VALUE(value);

    auto isEmpty = functionRef->ref.Get(v8Iso)->Call(v8Ctx, v8Ctx->Global(), 2, argArray).IsEmpty();

    FCT_CALLBACK_AFTER
}

extern "C"
void progp_CallFunctionWithBoolP2(FCT_CALLBACK_PARAMS, bool value) {
    FCT_CALLBACK_BEFORE

    v8::Local<v8::Value> argArray[2];
    argArray[0] = v8::Undefined(v8Iso);
    argArray[1] = BOOL_TO_V8VALUE(value);

    auto isEmpty = functionRef->ref.Get(v8Iso)->Call(v8Ctx, v8Ctx->Global(), 2, argArray).IsEmpty();

    FCT_CALLBACK_AFTER
}

extern "C"
void progp_CallFunctionWithUndefined(FCT_CALLBACK_PARAMS) {
    FCT_CALLBACK_BEFORE

    auto isEmpty = functionRef->ref.Get(v8Iso)->Call(v8Ctx, v8Ctx->Global(), 0, nullptr).IsEmpty();

    FCT_CALLBACK_AFTER
}

//endregion

//region Others

void* progp_CopyGoBuffer(void* buffer, int size) {
    auto newBuffer = malloc(size);
    memcpy(newBuffer, buffer, size);
    return newBuffer;
}

const char* progpV8Value_GetTypeName(v8::Local<v8::Value> &value) {
    if (value->IsUndefined()) return "Undefined";
    if (value->IsNull()) return "Null";
    if (value->IsString()) return "String";
    if (value->IsStringObject()) return "StringObject";
    if (value->IsFloat64Array()) return "Float64Array";
    if (value->IsFloat32Array()) return "Float32Array";
    if (value->IsMapIterator()) return "MapIterator";
    if (value->IsMap()) return "Map";
    if (value->IsDate()) return "Date";
    if (value->IsBooleanObject()) return "BooleanObject";
    if (value->IsBoolean()) return "Boolean";
    if (value->IsBigIntObject()) return "BigIntObject";
    if (value->IsBigInt64Array()) return "BigInt64Array";
    if (value->IsAsyncFunction()) return "AsyncFunction";
    if (value->IsArrayBufferView()) return "ArrayBufferView";
    if (value->IsArgumentsObject()) return "ArgumentsObject";
    if (value->IsUint8Array()) return "Uint8Array";
    if (value->IsExternal()) return "External";
    if (value->IsFunction()) return "Function";
    if (value->IsArray()) return "Array";
    if (value->IsArrayBuffer()) return "ArrayBuffer";
    if (value->IsBigInt()) return "BigInt";
    if (value->IsInt32()) return "Int32";
    if (value->IsNumber()) return "Number";
    return "???";
}

extern "C"
int progp_GetSizeOfAnyValueStruct() {
    return sizeof(s_progp_anyValue);
}


//endregion

//region V8 Debugger

//region Don't expose, for v8 debugger use

unsigned int randomChar() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

std::string buildGuid(int len) {
    std::stringstream ss;

    for (auto i = 0; i < len; i++) {
        const auto rc = randomChar();
        std::stringstream hexStream;
        hexStream << std::hex << rc;
        auto hex = hexStream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }

    return ss.str();
}

static std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;

    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }

    return str;
}

static inline std::string convertToString(v8::Isolate *isolate, const v8_inspector::StringView stringView) {
    int length = static_cast<int>(stringView.length());

    v8::Local<v8::String> message = (
            stringView.is8Bit()
            ? v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t *>(stringView.characters8()),
                                         v8::NewStringType::kNormal, length)
            : v8::String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t *>(stringView.characters16()),
                                         v8::NewStringType::kNormal, length)
    ).ToLocalChecked();

    v8::String::Utf8Value result(isolate, message);
    return *result;
}

static inline v8_inspector::StringView convertToStringView(const std::string &str) {
    auto *stringView = reinterpret_cast<const uint8_t *>(str.c_str());
    return {stringView, str.length()};
}

static inline v8::Local<v8::Object> parseJson(const v8::Local<v8::Context> &context, const std::string &json) {
    auto iso = context->GetIsolate();

    v8::MaybeLocal<v8::Value> v = v8::JSON::Parse(
            context, v8::String::NewFromUtf8(iso, &json[0],
                                             v8::NewStringType::kNormal).ToLocalChecked());

    if (v.IsEmpty()) return {};
    return v.ToLocalChecked()->ToObject(context).ToLocalChecked();
}

static inline std::string getPropertyFromJson(v8::Isolate *isolate,
                                              const v8::Local<v8::Object> &jsonObject,
                                              const std::string &propertyName) {

    v8::Local<v8::Value> property = jsonObject->Get(isolate->GetCurrentContext(),
                                                    v8::String::NewFromUtf8(isolate, propertyName.c_str(),
                                                                            v8::NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();

    v8::String::Utf8Value utf8Value(isolate, property);
    return *utf8Value;
}

inline void pauseMs(int durationInMs) {
    std::this_thread::sleep_for(std::chrono::milliseconds(durationInMs));
}

//endregion

const int Size_1Ko = 1024;
const int Size_1Mo = Size_1Ko * 1024;

// The max size of the packets sends to the debugger.
// Must be huge since the debugger sends huge packets and doesn't support chunking...
//
const int gDebuggerIoBufferSize = Size_1Mo * 60;

ProgpDbgInternals_IoNetwork *g_debuggerInt_ioNetwork;

f_progp_noParamNoReturn g_onDebuggerExitedCallback = nullptr;

extern "C"
void progp_WaitDebuggerReady() {
    if (g_debuggerInt_ioNetwork == nullptr) {
        g_debuggerInt_ioNetwork = new ProgpDbgInternals_IoNetwork(buildGuid(64), "file://progpv8");
    }

    ProgpDbgInternals_IoNetwork::printOpenChromeInspectorMessage();

    for (;;) {
        if (g_debuggerInt_ioNetwork->isDebuggerReady()) {
            break;
        }

        pauseMs(100);
    }
}

void progpDbgInternals_OnDebuggerReady() {
    PROGP_PRINT("Debugger: Chrome Inspector is ready");
}

void progpDbgInternals_DeclareDebuggerExited() {
    if (g_onDebuggerExitedCallback!= nullptr) g_onDebuggerExitedCallback();
    else DEBUGGER_ERROR("Debugger client has been closed.")
}

//region Class --> ProgpDbgInternals_IoNetwork

ProgpDbgInternals_IoNetwork::ProgpDbgInternals_IoNetwork(std::string guid, std::string filePath)
{
    _port = 9229;
    _guid = std::move(guid);
    _filePath = std::move(filePath);

    std::thread([this] { return this->startListeningNetwork(); }).detach();
}

http::message_generator ProgpDbgInternals_IoNetwork::onSimpleHttpRequest(http::request<http::string_body> &req)
{
    auto const bad_request =
            [&req](beast::string_view why)
            {
                http::response<http::string_body> res{http::status::bad_request, req.version()};
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "text/html");
                res.keep_alive(req.keep_alive());
                res.body() = std::string(why);
                res.prepare_payload();
                return res;
            };

    auto const send_response = [&req](beast::string_view msg) {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(msg);
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    //
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head)
        return bad_request("Unknown HTTP-method");

    auto target = req.target();

    // Request path must be absolute and not contain "..".
    if(target.empty() || target[0] != '/' || req.target().find("..") != beast::string_view::npos) return bad_request("Illegal request-target");

    if (target=="/json/version") {
        return send_response(R"({"Browser":"progpjs/1.0.0","Protocol-Version":"1.3","V8-Version":"11.4.183.1"})");
    }

    // Note: target was "/json" and is now "/json/list?for_tab".
    //
    if (target.starts_with("/json")) {
        std::string s = R"([{"description":"progpjs","devtoolsFrontendUrl":"devtools://devtools/bundled/js_app.html?ws=127.0.0.1:%PORT%/ws/%GUID%&experiments=true&v8only=true","faviconUrl":"https://progpjs.com/favicon.ico","id":"%GUID%","title":"progpjs - main [pid: %PID%]","type":"node","url":"%FILEPATH%","webSocketDebuggerUrl":"ws://127.0.0.1:%PORT%/ws/%GUID%"}])";

        s = replaceAll(s, std::string("%GUID%"), _guid);
        s = replaceAll(s, std::string("%PID%"), std::to_string(5555));
        s = replaceAll(s, std::string("%PORT%"), std::to_string(_port));
        s = replaceAll(s, std::string("%FILEPATH%"), _filePath);

        return send_response(s);
    }

#if PROGP_PRINT_DEBUGGER_MESSAGE
    PROGP_DEBUG("Unknown debugger request: " << target.data());
#endif

    return bad_request("Unknown request");
}

void ProgpDbgInternals_IoNetwork::sendMessageToChromeInspector(const std::string &message) {
    if (_isDebuggerSessionStopped) return;
    PROGP_PRINT_DEBUGGER_MSG("E2I >>>>> " << message)

    try {
        boost::beast::multi_buffer b;
        boost::beast::ostream(b) << message;

        _ws->text(_ws->got_text());
        auto data = b.data();

        if (message.size()>gDebuggerIoBufferSize) PROGP_PRINT("Warning, message to debugger exceed the max size. See: gDebuggerIoBufferSize");
        _ws->write(data);

    } catch (beast::system_error const &se) {
        if (se.code() != websocket::error::closed) {
            DEBUGGER_ERROR(se.code().message())
        }
    } catch (std::exception const &e) {
        DEBUGGER_ERROR(e.what())
    }
}

bool ProgpDbgInternals_IoNetwork::isDebuggerReady() const {
    return _isDebuggerReady;
}

void ProgpDbgInternals_IoNetwork::printOpenChromeInspectorMessage() {
    PROGP_PRINT("");
    PROGP_PRINT("=== ProgpJS Debugger started =========================================");
    PROGP_PRINT("=      Open the following link in your Chrome/Chromium browser:      =");
    PROGP_PRINT("=      chrome://inspect/#devices                                     =");
    PROGP_PRINT("======================================================================");
}

void ProgpDbgInternals_IoNetwork::loopAddIncomingMessagesToDebuggerQueue() {
    _isDebuggerSessionStopped = false;

    while (true) {
        try {
            if (_isDebuggerSessionStopped) break;
            auto message = readNextWebSocketMessage();
            if (_isDebuggerSessionStopped) break;

            if (!message.empty() && (_inspector!= nullptr)) _inspector->addMessageFromChromeInspector(message);
        }
        catch (const std::exception &e) {
            declareDebuggerSessionStopped();
        }
    }
}

void ProgpDbgInternals_IoNetwork::declareDebuggerSessionStopped() {
    if (!_isNowAWebSocket) return;
    if (_isDebuggerSessionStopped) return;

    _isDebuggerSessionStopped = true;

    if (_inspector!= nullptr) _inspector->onNetworkDisconnected();
}

void ProgpDbgInternals_IoNetwork::startListeningNetwork() {
    auto const address = net::ip::make_address("127.0.0.1");
    boost::beast::flat_buffer buffer;

    _isDebuggerSessionStopped = true;

    try {
        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, static_cast<unsigned short>(_port)}};

        while (true) {
            _isDebuggerReady = false;
            _isNowAWebSocket = false;

            // The connexion socket.
            // Will be initialized by the acceptor.
            //
            // There is one socket per message, since each client connects
            // to a different internal port (the same external port, but a different internal port).
            //
            // It's why the socket is closed after each http-message that is received.
            //
            tcp::socket socket{ioc};

            try {
                // Initialize the socket and
                // wait until a client connects to this socket.
                // Block waiting the client.
                //
                acceptor.accept(socket);

                // Read the request send by the client (here Chrome Inspector).
                // Here buffer is only used to store the temporary data.
                //
                http::request<http::string_body> req;
                http::read(socket, buffer, req);

                // The target is what is after "http://localhost/???".
                // It's the "???" part.
                //
                auto reqTarget = req.target();

                // Here the difficulty is that two things are mixed:
                // - Simple HTTP calls.
                // - And webSocket calls.
                //
                // Its starts by simples http call where Chrome Inspector asks us the list of the sessions.
                // Once the user clicks on a session through a hypertext link (or if Chrome Inspector auto-connect),
                // then the connexion turns into a web-socket connexion.
                //
                // We have known that it's beginning the web-socket dialogue a target beginning by "/ws/"+;
                //
                if (reqTarget.starts_with("/ws/")) {
                    // >>> Began a web-socket dialogue.

                    _isNowAWebSocket = true;

                    _ws = new websocket::stream<tcp::socket>(std::move(socket));

                    // Required. Without that track, the messages are too long.
                    // The default is 4kb. Here grow to 8Mb. Which means scripts size can exceed 8Mb.
                    _ws->write_buffer_bytes(gDebuggerIoBufferSize);

                    _ws->accept(req);

                    // Create a client which will consume the messages from the Chrome Inspector.
                    onChromeInspectorConnected();

                    // An infinite loop waiting for messages and adding them to the inspector client message queue.
                    this->loopAddIncomingMessagesToDebuggerQueue();

                    // Close the connexion.

                    if (_ws!= nullptr) {
                        beast::error_code ec;
                        ec = _ws->next_layer().shutdown(tcp::socket::shutdown_both, ec);

                        delete (_ws);
                        _ws = nullptr;

                        if (ec) {
                            // 57 - Socket is not connected
                            if (ec.value()!=57) {
                                DEBUGGER_ERROR("Code " << ec.value() << " - " << ec.what())
                            }
                        }
                    }
                } else {
                    // >>> A standard HTTP call.

                    // Process the message and give a response.
                    auto msg = onSimpleHttpRequest(req);

                    // Send this response.
                    //
                    beast::error_code ec;
                    beast::write(socket, std::move(msg), ec);
                    //
                    if (ec) {
                        DEBUGGER_ERROR("Code " << ec.value() << " - " << ec.what())
                    }

                    // Close the connexion with the client.
                    // Here it's always one shoot.
                    //
                    ec = socket.shutdown(tcp::socket::shutdown_send, ec);
                    //
                    if (ec) {
                        // 57 - Socket is not connected
                        if (ec.value()!=57) {
                            DEBUGGER_ERROR("Code " << ec.value() << " - " << ec.what())
                        }
                    }
                }
            }
            catch (beast::system_error const &se) {
                if (se.code() != websocket::error::closed) {
                    DEBUGGER_ERROR("Code " << se.code().value() << " - " << se.code().message())
                    declareDebuggerSessionStopped();
                }
            }
            catch (const std::exception &e) {
                DEBUGGER_ERROR(e.what())
                declareDebuggerSessionStopped();
            }
        }
    }
    catch (beast::system_error const &se) {
        if (se.code().message()=="Permission denied") {
            DEBUGGER_ERROR("The port " << _port << " is already used, can't launch debugger.")
        } else {
            DEBUGGER_ERROR(se.code().message() << " - " << se.what())
        }

        declareDebuggerSessionStopped();
    }
    catch (const std::exception &e) {
        DEBUGGER_ERROR(e.what())
        declareDebuggerSessionStopped();
    }
}

void ProgpDbgInternals_IoNetwork::onChromeInspectorConnected() {
    if (_inspector== nullptr) {
        _inspector = new ProgpDbgInternals_V8InspectorClientImpl(gV8Platform.get(), this);
    }

    _inspector->onChromeInspectorConnected();
}

void ProgpDbgInternals_IoNetwork::declareDebuggerReady() {
    _isDebuggerReady = true;
}

std::string ProgpDbgInternals_IoNetwork::readNextWebSocketMessage() {
    beast::flat_buffer buffer;

    try {
        beast::error_code ec;
        _ws->read(buffer, ec);

        if (ec) {
            declareDebuggerSessionStopped();
            return "";
        }
    }
    catch (...)
    {
        declareDebuggerSessionStopped();
    }

    return boost::beast::buffers_to_string(buffer.data());
}

//endregion

//region Class --> ProgpDbgInternals_V8InspectorClientImpl

int gDbgInt_nextContextGroupId = 1;

ProgpDbgInternals_V8InspectorClientImpl::ProgpDbgInternals_V8InspectorClientImpl(v8::Platform* platform, ProgpDbgInternals_IoNetwork* network) {
    _platform = platform;
    _network = network;

    // The id of this context group.
    // Used when more than on thing is debugger at the same time.
    //
    _contextGroupId = gDbgInt_nextContextGroupId;
    gDbgInt_nextContextGroupId++;

    // The inspector is what process the message of Chrome Inspector.
    //_inspector.release(); .... to delete
    //
    _inspector = v8_inspector::V8Inspector::create(gProgpDbgCtx->v8Iso, this);

    // Allow avoiding recursing on message processing loop.
    _runNestedLoop = false;

    _isInPause = true;

    this->loopWaitAndSendMessages();
}

void ProgpDbgInternals_V8InspectorClientImpl::onNetworkDisconnected() {
    progpDbgInternals_DeclareDebuggerExited();

    // Avoid the closing the debugger block the script.
    //
    _v8InspectorSession->setSkipAllPauses(true);
    _v8InspectorSession->resume();
}

void ProgpDbgInternals_V8InspectorClientImpl::onChromeInspectorConnected() {
    if (_v8InspectorSession== nullptr) {
        // The channel allows receiving messages from V8 Engine.
        _channel = std::make_unique<ProgpDbgInternals_V8InspectorChannelImpl>(gProgpDbgCtx->v8Iso, this);

        // Bind the inspector and the channel.
        // _v8InspectorSession.release(); .... to delete
        //
        _v8InspectorSession = _inspector->connect(
                _contextGroupId, _channel.get(), v8_inspector::StringView(),
                v8_inspector::V8Inspector::ClientTrustLevel::kFullyTrusted
        );
    }
    else {
        _v8InspectorSession->setSkipAllPauses(false);
    }

    // Give a name to the context.
    // Allows discerning it in the messages.
    //
    std::string ctxName("progpjs-");
    ctxName += std::to_string(_contextGroupId);

    auto progpCtx = gProgpDbgCtx;
    V8CTX_ACCESS();

    _inspector->contextCreated(v8_inspector::V8ContextInfo(v8Ctx, _contextGroupId, convertToStringView(ctxName)));

    _runNestedLoop = false;
    _isInPause = false;
}

void ProgpDbgInternals_V8InspectorClientImpl::resumeFromPausedState() {
    _v8InspectorSession->setSkipAllPauses(false);
    _v8InspectorSession->resume();
}

void ProgpDbgInternals_V8InspectorClientImpl::onMessageFromChromeInspector(const v8_inspector::StringView &message_view) {
    _v8InspectorSession->dispatchProtocolMessage(message_view);
}

void ProgpDbgInternals_V8InspectorClientImpl::runMessageLoopOnPause(int contextGroupId) {
    // Avoid recurring.
    if (_runNestedLoop) return;

    _isInPause = true;
    _runNestedLoop = true;

    auto progpCtx = gProgpDbgCtx;

    // _onWaitFrontendMessageOnPause will wait a network message and call ProgpDebugger::onMessage
    //
    while (_isInPause) {
        if (!processNextMessageIfAvailable()) pauseMs(50);

        {
            std::lock_guard autoLock(_ctxPtrMutex);
            while (v8::platform::PumpMessageLoop(_platform, progpCtx->v8Iso)) {}
        }
    }

    _runNestedLoop = false;
    _isInPause = false;
}

void ProgpDbgInternals_V8InspectorClientImpl::quitMessageLoopOnPause() {
    _isInPause = false;
}

v8::Local<v8::Context> ProgpDbgInternals_V8InspectorClientImpl::ensureDefaultContextInGroup(int contextGroupId) {
    auto progpCtx = gProgpDbgCtx;
    V8CTX_ACCESS();
    return v8Ctx;
}

void ProgpDbgInternals_V8InspectorClientImpl::processMessageFromChromeInspector(const std::string &message) {
    std::lock_guard autoLock(_ctxPtrMutex);
    auto progpCtx = gProgpDbgCtx;
    V8CTX_ACCESS();

    v8_inspector::StringView protocolMessage = convertToStringView(message);
    v8::Local<v8::Object> jsonObject = parseJson(v8Ctx, message);

    std::string method;

    if (!jsonObject.IsEmpty()) {
        method = getPropertyFromJson(v8Iso, jsonObject, "method");
    }

    if (method == "Runtime.runIfWaitingForDebugger") {
        PROGP_PRINT_DEBUGGER_MSG("RECEIVE signal : Debugger Ready")

        // Required if we asked the script to wait the debugger.
        // It's not the case here, but we added this line in case of future changes.
        //
        runIfWaitingForDebugger(1);

        _network->declareDebuggerReady();
        progpDbgInternals_OnDebuggerReady();
    }

    onMessageFromChromeInspector(protocolMessage);
    PROGP_PRINT_DEBUGGER_MSG("I2E <<<<< " << message)
}

void ProgpDbgInternals_V8InspectorClientImpl::addMessageFromChromeInspector(std::string message) {
    auto entry = new(s_debuggerMessageList);
    entry->message = std::move(message);

    std::unique_lock lock(_messageListMutex);

    if (_messageListTail != nullptr) {
        _messageListTail->next = entry;
    }

    _messageListTail = entry;

    if (_messageListHead == nullptr) {
        _messageListHead = entry;
    }

    lock.unlock();
}

/**
 * Return the next message in stack, or null if no message.
 * ! The caller must delete the message once consumed !
 */
s_debuggerMessageList* ProgpDbgInternals_V8InspectorClientImpl::getNextMessageFromChromeInspector() {
    std::lock_guard autoUnlock(_messageListMutex);

    if (_messageListHead == nullptr) return nullptr;

    auto item = _messageListHead;
    _messageListHead = item->next;

    if (_messageListHead == nullptr) {
        _messageListTail = nullptr;
    }

    return item;
}

bool ProgpDbgInternals_V8InspectorClientImpl::processNextMessageIfAvailable() {
    auto item = getNextMessageFromChromeInspector();

    if (item!= nullptr) {
        auto message = item->message;
        delete (item);
        processMessageFromChromeInspector(message);

        return true;
    }

    return false;
}

/**
 * The network stack fill a queue with messages.
 * Here we take items from this queue and send the to the debugger.
 * It's done in his own thread in order to avoid been locked.
 */
void ProgpDbgInternals_V8InspectorClientImpl::loopWaitAndSendMessages() {
    // Will send the messages using a thread which don't block the network.
    //
    std::thread([this]() {
        pauseMs(50);

        // Avoid endless loop warning.
        while (gProgpDbgCtx->v8Iso!= nullptr) {
            // If paused, then it's processed by ProgpDbgInternals_IoNetwork::processNextMessageIfAvailable.
            if (!_isInPause) {
                auto item = getNextMessageFromChromeInspector();

                if (item!= nullptr) {
                    auto message = item->message;
                    delete (item);
                    processMessageFromChromeInspector(message);
                }
            }

            // Slow down the loop.
            pauseMs(100);
        }
    }).detach();
}

//endregion

//region Class --> ProgpDbgInternals_V8InspectorChannelImpl

ProgpDbgInternals_V8InspectorChannelImpl::ProgpDbgInternals_V8InspectorChannelImpl(v8::Isolate* v8Iso, ProgpDbgInternals_V8InspectorClientImpl* inspector) {
    _v8Iso = v8Iso;
    _inspector = inspector;
}

void ProgpDbgInternals_V8InspectorChannelImpl::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) {
    auto progpCtx = gProgpDbgCtx;
    V8CTX_ACCESS();

    g_debuggerInt_ioNetwork->sendMessageToChromeInspector(convertToString(v8Iso, message->string()));
}

void ProgpDbgInternals_V8InspectorChannelImpl::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) {
    auto progpCtx = gProgpDbgCtx;
    V8CTX_ACCESS();

    g_debuggerInt_ioNetwork->sendMessageToChromeInspector( convertToString(v8Iso, message->string()));
}

void ProgpDbgInternals_V8InspectorChannelImpl::flushProtocolNotifications() {
}

//endregion

//endregion

//region AnyValue

s_progp_anyValue gAnyValue[32];
f_draftFunctionListener gDraftFunctionListener = nullptr;

s_progp_anyValue progp_AnyValueFromV8Value(ProgpContext progpCtx, const v8::Local<v8::Context> &v8Ctx, const v8::Local<v8::Value> &v8Value) {
    s_progp_anyValue res;
    res.mustFree = false;

    if (v8Value.IsEmpty()) {
        res.valueType = PROGP_ANY_VALUE_UNDEFINED;
        return res;
    }

    if (v8Value->IsNull()) {
        res.valueType = PROGP_ANY_VALUE_NULL;
    }

    if (v8Value->IsNumber()) {
        res.valueType = PROGP_ANY_VALUE_NUMBER;
        res.number = v8Value.As<v8::Number>()->Value();
        return res;
    }

    if (v8Value->IsBoolean()) {
        res.valueType = PROGP_ANY_VALUE_BOOLEAN;
        if (v8Value.As<v8::Boolean>()->IsTrue()) res.size = 1; else res.size = 0;
        return res;
    }

    if (v8Value->IsString()) {
        res.valueType = PROGP_ANY_VALUE_STRING;
        auto asString = v8::String::Utf8Value(progpCtx->v8Iso, v8Value.As<v8::String>());

        // Here the value must be copied since asString is deleted once quitting this scope.
        // I don't know how to avoid in this use case.
        //
        res.voidPtr = strdup(*asString);
        res.mustFree = true;
        return res;
    }

    if (v8Value->IsArrayBuffer()) {
        res.valueType = PROGP_ANY_VALUE_ARRAYBUFFER;
        auto asBuffer = v8Value.As<v8::ArrayBuffer>();
        res.size = (int)asBuffer->ByteLength();
        res.voidPtr = (char*)asBuffer->Data();
        return res;
    }

    if (v8Value->IsFunction()) {
        res.valueType = PROGP_ANY_VALUE_FUNCTION;
        res.voidPtr = progpFunctions_NewPointer(progpCtx, v8Value.As<v8::Function>());
        res.mustFree = false; // <- the deleting will be managed by the Go wrapper
        return res;
    }

    if (v8Value->IsObject() || v8Value->IsArray()) {
        // gap is the indentation.
        v8::Local<v8::String> gap;
        v8::Local<v8::String> json;
        //
        if (v8::JSON::Stringify(v8Ctx, v8Value, gap).ToLocal(&json)) {
            res.valueType = PROGP_ANY_VALUE_JSON;
            auto asString = v8::String::Utf8Value(progpCtx->v8Iso, json);
            res.voidPtr = strdup(*asString);
            res.size = (int)strlen((char*)res.voidPtr);
            return res;
        }
    }

    res.valueType = PROGP_ANY_VALUE_INVALID;
    return res;
}

v8::Local<v8::Value> progp_AnyValueToV8Value(ProgpContext progpCtx, const v8::Local<v8::Context> &v8Ctx, const s_progp_anyValue &anyValue) {
    switch (anyValue.valueType) {
        case PROGP_ANY_VALUE_UNDEFINED:
        case PROGP_ANY_VALUE_INVALID:
            return v8::Undefined(progpCtx->v8Iso);

        case PROGP_ANY_VALUE_NULL:
            return v8::Null(progpCtx->v8Iso);

        case PROGP_ANY_VALUE_NUMBER:
            return v8::Number::New(progpCtx->v8Iso, anyValue.number);

        case PROGP_ANY_VALUE_BOOLEAN:
            return v8::Boolean::New(progpCtx->v8Iso, anyValue.size == 1);

        case PROGP_ANY_VALUE_STRING: {
            v8::Local<v8::String> r = CSTRING_TO_V8VALUE((char*)anyValue.voidPtr);
            if (anyValue.mustFree) delete ((char*)anyValue.voidPtr);
            return r;
        }

        case PROGP_ANY_VALUE_ARRAYBUFFER: {
            auto v8BackingStore = std::shared_ptr(v8::ArrayBuffer::NewBackingStore(progpCtx->v8Iso, anyValue.size));
            memcpy(v8BackingStore->Data(), anyValue.voidPtr, anyValue.size);
            return v8::ArrayBuffer::New(progpCtx->v8Iso, v8BackingStore);
        }

        case PROGP_ANY_VALUE_JSON: {
            auto asString = CSTRING_TO_V8VALUE((char*)anyValue.voidPtr);
            auto mayBeJson = v8::JSON::Parse(v8Ctx, asString);

            if (mayBeJson.IsEmpty()) {
                return v8::Undefined(progpCtx->v8Iso);
            }

            return mayBeJson.ToLocalChecked();
        }
    }

    return v8::Undefined(progpCtx->v8Iso);
}

void progp_handleDraftFunction(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
    PROGP_V8FUNCTION_BEFORE_PROGPCTX

        if (gDraftFunctionListener==nullptr) {
            callInfo.GetReturnValue().SetUndefined();
            return;
        }

        auto data = callInfo.Data();
        auto utf8FunctionName = V8VALUE_TO_UTF8STRING(data);
        auto functionName = *utf8FunctionName;

        auto argCount = callInfo.Length();

        for (int i=0;i<argCount;i++) {
            auto value = progp_AnyValueFromV8Value(progpCtx, v8Ctx, callInfo[i]);
            gAnyValue[i] = value;
        }

        auto res = gDraftFunctionListener(progpCtx, functionName, gAnyValue, argCount, progpCtx->event);

        if (res.errorMessage!= nullptr) {
            auto v8String = CSTRING_TO_V8VALUE(res.errorMessage);
            delete(res.errorMessage);
            v8Iso->ThrowError(v8String);
            return;
        }

        auto asV8Value = progp_AnyValueToV8Value(progpCtx, v8Ctx, res);

        callInfo.GetReturnValue().Set(asV8Value);

    PROGP_V8FUNCTION_AFTER
}

//endregion

//region Others

void printConsole(char* text) {
    PROGP_PRINT(text);
}

f_progp_printHandler gPrintHandler = printConsole;

void progp_PrintString(const char* text) {
    gPrintHandler((char*)text);
}

//endregion

//region Config

void progpConfig_SetJavascriptErrorListener(f_progp_javascriptErrorListener listener) {
    gJavascriptErrorListener = listener;
}

void progpConfig_SetJavascriptFunctionProvider(f_progp_v8_functions_provider handler) {
    gV8FunctionProvider = handler;
}

void progpConfig_SetDynamicFunctionProvider(f_progp_v8_dynamicFunctions_provider handler) {
    gV8DynamicFunctionProvider = handler;
}

void progpConfig_SetAllowedFunctionChecker(f_progp_v8_function_allowedFunctionChecker handler) {
    gV8AllowedFunctionChecker = handler;
}

void progpConfig_OnDebuggerExitedListener(f_progp_noParamNoReturn listener) {
    g_onDebuggerExitedCallback = listener;
}

void progpConfig_OnNoMoreTask(f_progp_onNoMoreTasksForContext listener) {
    g_onNoMoreTask = listener;
}

void progpConfig_OnEventFinished(f_progp_eventFinished listener) {
    g_onEventFinished = listener;
}

void progpConfig_SetDraftFunctionListener(f_draftFunctionListener listener) {
    gDraftFunctionListener = listener;
}

f_progp_printHandler progpConfig_GetPrintHandler() {
    return gPrintHandler;
}

void progpConfig_SetPrintHandler(f_progp_printHandler handler) {
    gPrintHandler = handler;
}

//endregion