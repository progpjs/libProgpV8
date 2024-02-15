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

#ifndef PROGP_V8_H
#define PROGP_V8_H

#include "./progpAPI.h"
#include <string>
#include "v8.h"
#include <v8-inspector.h>

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

//region MACROS

#define PROGP_PRINT(m) std::cout << m << std::endl
#define PROGP_DEBUG(m) std::cout << "[C-PROGP_DEBUG] - " << m << std::endl
#define PROGP_LOG_ERROR(FROM, WHAT) std::cout << "ERROR - " << FROM << " - " << WHAT << std::endl

#define V8ISO_ACCESS() \
    auto v8Iso = gUnsafe_v8Iso; \
    v8::Locker locker(v8Iso); \
    v8::Isolate::Scope isolate_scope(v8Iso); \
    v8::HandleScope handle_scope(v8Iso)

#define V8CTX_ACCESS() \
    V8ISO_ACCESS(); \
    auto v8Ctx = gUnsafe_v8Ctx->Get(v8Iso); \
    v8::Context::Scope contextScope(v8Ctx)

#define PROGP_V8FUNCTION_BEFORE_V8CTX \
    auto v8Iso = callInfo.GetIsolate(); \
    auto v8Ctx = v8Iso->GetCurrentContext(); \
    v8::Locker locker(v8Iso); \
    v8::Isolate::Scope isolate_scope(v8Iso); \
    v8::HandleScope handle_scope(v8Iso); \
    v8::Context::Scope contextScope(v8Ctx); \
    try { \

#define PROGP_V8FUNCTION_BEFORE \
    auto v8Iso = callInfo.GetIsolate(); \
    v8::Locker locker(v8Iso); \
    v8::Isolate::Scope isolate_scope(v8Iso); \
    try {

#define PROGP_V8FUNCTION_AFTER \
    } \
    catch (std::exception &e) { \
        auto message = e.what(); \
        v8Iso->ThrowError(v8::String::NewFromUtf8(v8Iso, message, v8::NewStringType::kNormal, (int) strlen(message)).ToLocalChecked()); \
    }

#define PROGP_BIND_FUNCTION(GROUP, NAME, FCT_REF) \
    if (group==GROUP) progp_AddFunctionToObject(GROUP, v8Host, NAME, FCT_REF)

//endregion

//region Function Types

typedef void (*f_progp_v8_function)(const v8::FunctionCallbackInfo<v8::Value> &callInfo);
typedef void (*f_progp_v8_functions_provider)(const std::string &group, const v8::Local<v8::Object> v8Object);
typedef void (*f_progp_v8_dynamicFunctions_provider)(char* group);
typedef int (*f_progp_v8_function_allowedFunctionChecker)(char* securityGroup, char* functionGroup, char* functionName);

//endregion

//region C Structs

struct s_progp_v8_function {
    v8::Persistent<v8::Function> ref;
};

typedef struct s_progp_v8_eventData {
    /**
     * The V8 object use by the javascript.
     */
    v8::Persistent<v8::Value> data;

    /**
     * Is called when the event exit.
     */
    s_progp_v8_function* onEventExit;
} s_progp_v8_eventData;

//endregion

//region C++ <> V8 value

//region C++ to V8 value

#define DOUBLE_TO_V8VALUE(VALUE) v8::Number::New(v8Iso, VALUE)
#define LONG_TO_V8VALUE(VALUE) v8::Number::New(v8Iso, VALUE)
#define BOOL_TO_V8VALUE(VALUE) v8::Boolean::New(v8Iso, VALUE)

#define CSTRING_TO_V8VALUE(VALUE) v8::String::NewFromOneByte(gUnsafe_v8Iso, reinterpret_cast<const uint8_t *>(VALUE), v8::NewStringType::kNormal, (int)strlen(VALUE)).ToLocalChecked()
#define CSTRING_TO_V8VALUE_SIZE(VALUE, SIZE) v8::String::NewFromOneByte(v8Iso, reinterpret_cast<const uint8_t *>(VALUE), v8::NewStringType::kNormal, (int)SIZE).ToLocalChecked()

//endregion

//region V8 value to C++

const char* progpV8Value_GetTypeName(v8::Local<v8::Value> &value);

#define V8VALUE_TO_UTF8STRING(V8_STRING)        (v8::String::Utf8Value(v8Iso, V8_STRING))
#define V8VALUE_TO_CSTRING(V8_STRING)           (*(V8VALUE_TO_UTF8STRING(V8_STRING)))

#define V8VALUE_TO_INT32(VALUE)                 VALUE.Get(iso)->Int32Value(context).ToChecked()
#define V8VALUE_FROM_INT32(VALUE)               v8::Integer::New(v8Iso, VALUE)

#define V8VALUE_TO_INT(VALUE)                   V8VALUE_TO_INT32(VALUE)
#define V8VALUE_FROM_INT(VALUE)                 V8VALUE_FROM_INT32(VALUE)

#define V8VALUE_TO_DOUBLE(VALUE)                VALUE.Get(v8Iso)->NumberValue(context).ToChecked()
#define V8VALUE_FROM_DOUBLE(VALUE)              v8::Number::New(v8Iso, VALUE)

#define V8VALUE_FROM_CHARPTR(VALUE)              v8::String::NewFromUtf8(v8Iso, (const char*)VALUE).ToLocalChecked()
#define V8VALUE_FROM_SIZEDSTRING(VALUE, SIZE)    v8::String::NewFromUtf8(v8Iso, VALUE, v8::NewStringType::kNormal, (int)SIZE).ToLocalChecked()

#define V8VALUE_FROM_BOOL(VALUE)                v8::Boolean::New(v8Iso, VALUE)

#define V8VALUE_FROM_GOSTRING(VALUE)            v8::String::NewFromUtf8(v8Iso, ((s_progp_goString*)VALUE)->p, v8::NewStringType::kNormal, (int)((s_progp_goString*)VALUE)->n).ToLocalChecked()

#define V8VALUE_FROM_GOCUSTOM(OUT, VALUE, SIZE) \
    { \
        auto mayBeJson = v8::JSON::Parse(v8Ctx, CSTRING_TO_V8VALUE_SIZE((const char*)VALUE, SIZE)); \
        if (mayBeJson.IsEmpty()) OUT = Undefined(v8Iso); \
        else OUT = mayBeJson.ToLocalChecked(); \
    }

//endregion

//region Call Arguments Conversion

#define progp_v8ObjectToGoString(target, v8Object) \
    v8::Local<v8::String> gap_##target;\
    v8::Local<v8::String> json_##target;\
    if (!v8::JSON::Stringify(v8Ctx, v8Object, gap_##target).ToLocal(&json_##target)) throw std::runtime_error("invalid object, can't convert to json"); \
    auto asString_##target = v8::String::Utf8Value(v8Iso, json_##target); \
    auto target = s_progp_goStringOut{*asString_##target, (int)strlen(*asString_##target)}

#define V8CALLARG_EXPECT_ARGCOUNT(count)                                        \
    if (callInfo.Length()<count) {                                              \
        throw std::runtime_error("invalid parameter count");                    \
    }

#define V8CALLARG_EXPECT_ANYVALUE(target, argNum) \
     auto target = progpAnyValue_fromV8Value(v8Iso, v8Ctx, callInfo[argNum])

#define V8CALLARG_EXPECT_V8OBJECT(target, argNum)                               \
    if (!callInfo[argNum]->IsObject()) {                                        \
        throw std::runtime_error("invalid param type, expect object");          \
    }                                                                           \
    auto target = callInfo[argNum].As<v8::Object>()

#define V8CALLARG_EXPECT_V8OBJECT_TOSTRING(target, argNum)                      \
    auto tmp_##target = callInfo[argNum].As<v8::Object>();                      \
    progp_v8ObjectToGoString(target, tmp_##target)

#define V8CALLARG_EXPECT_BOOL(target, argNum)                                   \
    if (!callInfo[argNum]->IsBoolean()) {                                       \
        throw std::runtime_error("invalid param type, expect boolean");         \
    }                                                                           \
    auto target = callInfo[argNum]->BooleanValue(v8Iso)

// Note: int, long, and float doesn't exist. Only double exists with v8.
//
#define V8CALLARG_EXPECT_DOUBLE(target, argNum)                                 \
    if (!callInfo[argNum]->IsNumber()) {                                        \
        throw std::runtime_error("invalid param type, expect float number");    \
    }                                                                           \
    auto target = callInfo[argNum]->NumberValue(v8Ctx).ToChecked()

#define V8CALLARG_EXPECT_UTF8STRING(target, argNum)                             \
    if (!callInfo[argNum]->IsString()) {                                        \
        throw std::runtime_error("invalid param type, expect string");          \
    }                                                                           \
    auto target = V8VALUE_TO_UTF8STRING(callInfo[argNum])

#define V8CALLARG_EXPECT_CSTRING(target, argNum)                                \
    if (!callInfo[argNum]->IsString()) {                                        \
        throw std::runtime_error("invalid param type, expect string");          \
    }                                                                           \
    auto tmp_##target = V8VALUE_TO_UTF8STRING(callInfo[argNum]);                \
    const char* target = *tmp_##target

#define V8CALLARG_EXPECT_GOSTRING(target, argNum)                                \
    if (!callInfo[argNum]->IsString()) {                                         \
        throw std::runtime_error("invalid param type, expect string");           \
    }                                                                            \
    auto tmp_##target = V8VALUE_TO_UTF8STRING(callInfo[argNum]);                 \
    s_progp_goStringOut target = {*tmp_##target, (int)strlen(*tmp_##target)}

#define V8CALLARG_EXPECT_V8STRING(target, argNum)                                \
    if (!callInfo[argNum]->IsString()) {                                        \
        throw std::runtime_error("invalid param type, expect string");          \
    }                                                                           \
    auto target = callInfo[argNum].As<v8::String>()

#define V8CALLARG_EXPECT_ARRAYBUFFER(target, argNum)                            \
    if (!callInfo[argNum]->IsArrayBuffer()) {                                   \
        throw std::runtime_error("invalid param type, expect ArrayBuffer");     \
    }                                                                           \
    auto target = callInfo[argNum].As<v8::ArrayBuffer>()

#define V8CALLARG_EXPECT_ARRAYBUFFER_DATA(target, argNum)                       \
    if (!callInfo[argNum]->IsArrayBuffer()) {                                   \
        throw std::runtime_error("invalid param type, expect ArrayBuffer");     \
    }                                                                           \
    s_progp_v8_buffer tmp_##target;                                             \
    auto buffer_##target = (callInfo[argNum].As<v8::ArrayBuffer>());            \
    tmp_##target.buffer = buffer_##target->Data();                              \
    tmp_##target.length = (int)buffer_##target->ByteLength();                   \
    auto target = &tmp_##target

#define V8CALLARG_EXPECT_EXTERNAL(target, argNum) \
   void* target = nullptr; \
   if (callInfo[argNum]->IsExternal()) { target = callInfo[argNum].As<v8::External>()->Value(); } \
   else if (!callInfo[argNum]->IsUndefined() && !callInfo[argNum]->IsNull()) \
        throw std::runtime_error("invalid param type, expect External") \


#define V8CALLARG_EXPECT_FUNCTION(target, argNum)                               \
    if (!callInfo[argNum]->IsFunction()) {                                      \
        throw std::runtime_error("invalid param type, expect Function");        \
    }                                                                           \
    auto target = progpFunctions_NewPointer(callInfo[argNum].As<v8::Function>())

//endregion

//endregion

//region V8 Debugger

#define DEBUGGER_ERROR(MSG) std::cerr << "Debugger Error: " << MSG << std::endl;

#if PROGP_PRINT_DEBUGGER_MESSAGE
#define PROGP_PRINT_DEBUGGER_MSG(MSG) PROGP_PRINT("C - DBG - " << MSG);
#else
#define PROGP_PRINT_DEBUGGER_MSG(MSG)
#endif

class ProgpDbgInternals_IoNetwork;
class ProgpDbgInternals_V8InspectorChannelImpl;
class ProgpDbgInternals_V8InspectorClientImpl;

typedef struct s_debuggerMessageList {
    s_debuggerMessageList* next{};
    std::string message;
} s_debuggerMessageList;

/**
 * Manage the communication with Chrome Inspector.
 * Receive his messages, and send him messages.
 */
class ProgpDbgInternals_IoNetwork {
public:
    ProgpDbgInternals_IoNetwork(std::string guid, std::string filePath);

    void sendMessageToChromeInspector(const std::string &message);

    /**
     * Return true is the debugger is connected and ready
     * which mean that the client can execute script with
     * the context of the debugger.
     */
    [[nodiscard]] bool isDebuggerReady() const;

    static void printOpenChromeInspectorMessage();

private:
    friend ProgpDbgInternals_V8InspectorClientImpl;

    /**
     * Is called by the constructor inside a new thread in order
     * process the incoming network calls.
     */
    void startListeningNetwork();

    /**
     * An infinite loop which for each incoming message (call from readNextWebSocketMessage)
     * add this message to the queue of message that will be consumed.
     * See: progpDbgInternals_WaitAndSendMessages
     */
    void loopAddIncomingMessagesToDebuggerQueue();

    /**
     * Is call when the http connexion is closed.
     * Which mean generally than Chrome Inspector is closed.
     */
    void declareDebuggerSessionStopped();

    /**
     * Process an http message.
     * There is simple http messages and websocket message.
     * Here process the simple http messages.
     */
    http::message_generator onSimpleHttpRequest(http::request<http::string_body> &req);

    /**
     * Wait for the next websocket message.
     * Will block the thread.
     */
    std::string readNextWebSocketMessage();

    /**
     * Is call when the Chrome Inspector is connected.
     * When called, he isn't ready but here he begin to speak with us.
     */
    void onChromeInspectorConnected();

    /**
     * Is call by the inspector when the debugger is ready.
     */
    void declareDebuggerReady();

    int _port;
    bool _isNowAWebSocket{};
    std::string _guid;
    std::string _filePath;
    bool _isDebuggerReady{};
    bool _isDebuggerSessionStopped{};
    websocket::stream<tcp::socket>* _ws{};
    ProgpDbgInternals_V8InspectorClientImpl* _inspector{};
};

/**
 * Is created by ProgpDbgInternals_V8InspectorClientImpl.
 * His role is to receive the messages from the V8 Engine himself (to Chrome Inspector)
 * and being the binding between the session and the engine.
 *
 * When the engine must send a message or a notification then it calls "sendResponse" or "sendNotification".
 * Here messages are part from the dialog between the engine and the inspector.
 * While notifications are alerts printed to the human user.
 */
class ProgpDbgInternals_V8InspectorChannelImpl final : public v8_inspector::V8Inspector::Channel {
public:
    ProgpDbgInternals_V8InspectorChannelImpl(v8::Isolate* v8Iso, ProgpDbgInternals_V8InspectorClientImpl* inspector);

    /**
     * Is called when the V8 engine must send a message to Chrome Inspector.
     * Here it's the first point where V8Engine call.
     */
    void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) override;

    /**
     * Is called when the V8 engine must send a notification to Chrome Inspector.
     * Here it's the first point where V8Engine call.
     *
     * A notification is something like a warning send from the inspector to the user reading message.
     * It must be printed in the code.
     */
    void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;

    void flushProtocolNotifications() override;

private:
    v8::Isolate* _v8Iso;
    ProgpDbgInternals_V8InspectorClientImpl* _inspector;
};

/**
 * Is created when the user connect to Chrome Inspector in his navigator.
 * It's manager the link between Chrome Inspector and the V8 Engine.
 * Here is Chrome Inspector --> to --> the v8 Engine.
 *
 * More than one client can be created, but only one is living at a time.
 * For example if the inspector is disconnected, then an new ProgpDbgInternals_V8InspectorClientImpl
 * must be created when connecting again.
 */
class ProgpDbgInternals_V8InspectorClientImpl final : public v8_inspector::V8InspectorClient {
public:
    ProgpDbgInternals_V8InspectorClientImpl(v8::Platform *platform, ProgpDbgInternals_IoNetwork* network);

    void onChromeInspectorConnected();

    /**
     * Is call when a message is received from Chrome Inspector.
     * Will send him to the v8 engine himself.
     */
    void onMessageFromChromeInspector(const v8_inspector::StringView &message_view);

    /**
     * Is call when the debugger enter the paused state.
     */
    void runMessageLoopOnPause(int contextGroupId) override;

    /**
     * Call when the debugger quit from the paused state.
     */
    void quitMessageLoopOnPause() override;

    /**
     * Called by the network stack when a message is received.
     * Will be add to a chained list and consumed from a special thread.
     */
    void addMessageFromChromeInspector(std::string message);

    void resumeFromPausedState();

private:
    friend ProgpDbgInternals_V8InspectorChannelImpl;
    friend ProgpDbgInternals_IoNetwork;

    /**
     * Is called when the network is disconnected.
     * Will allows the current script to continue executing.
     */
    void onNetworkDisconnected();

    /**
     * Return the v8 context to use with the debugger.
     *
     * @param contextGroupId
     *      It's an idd which identify the current session.
     *      Is used when more than one context can be debugger.
     *
     * @return
     */
    v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

    //region Message loop

    /**
     * Declare that a message has been received from Chrome Inspector.
     */
    void processMessageFromChromeInspector(const std::string &message);

    s_debuggerMessageList* getNextMessageFromChromeInspector();
    bool processNextMessageIfAvailable();
    void loopWaitAndSendMessages();

    /**
     * The head of the list of messages.
     * The network add message and the inspector consume them.
     */
    s_debuggerMessageList* _messageListHead{};

    s_debuggerMessageList* _messageListTail{};
    std::mutex _messageListMutex{};

    //endregion

    std::mutex _ctxPtrMutex;
    bool _isInPause{};
    int _contextGroupId;
    v8::Platform *_platform;
    bool _runNestedLoop = false;

    ProgpDbgInternals_IoNetwork* _network;
    std::unique_ptr<v8_inspector::V8Inspector> _inspector;
    std::unique_ptr<v8_inspector::V8InspectorSession> _v8InspectorSession;
    std::unique_ptr<ProgpDbgInternals_V8InspectorChannelImpl> _channel;
};

//endregion

//region Config

void progpConfig_SetJavascriptErrorListener(f_progp_javascriptErrorListener listener);
void progpConfig_SetJavascriptFunctionProvider(f_progp_v8_functions_provider handler);
void progpConfig_OnDebuggerExitedListener(f_progp_noParamNoReturn listener);
void progpConfig_OnNoMoreTask(f_progp_noParamNoReturn listener);
void progpConfig_SetDraftFunctionListener(f_draftFunctionListener listener);
void progpConfig_SetDynamicFunctionProvider(f_progp_v8_dynamicFunctions_provider handler);
void progpConfig_SetAllowedFunctionChecker(f_progp_v8_function_allowedFunctionChecker handler);

//endregion

//region Golang binding

#ifndef PROGP_STANDALONE

void exposeGoFunctionsToV8(const std::string& group, v8::Local<v8::Object> v8Host);

#endif

//endregion

//region AnyValue

#define PROGP_ANY_VALUE_UNDEFINED 0
#define PROGP_ANY_VALUE_NULL 1
#define PROGP_ANY_VALUE_INVALID 2
#define PROGP_ANY_VALUE_NUMBER 3
#define PROGP_ANY_VALUE_STRING 4
#define PROGP_ANY_VALUE_BOOLEAN 5
#define PROGP_ANY_VALUE_ARRAYBUFFER 6
#define PROGP_ANY_VALUE_FUNCTION 7
#define PROGP_ANY_VALUE_JSON 8


v8::Local<v8::Value> progp_AnyValueToV8Value(const v8::Local<v8::Context> &v8Ctx, const s_progp_anyValue &anyValue);
s_progp_anyValue progp_AnyValueFromV8Value(const v8::Local<v8::Context> &v8Ctx, const v8::Local<v8::Value> &v8Value);
void progp_handleDraftFunction(const v8::FunctionCallbackInfo<v8::Value> &callInfo);

//endregion

void progp_AddFunction(const char* functionName, f_progp_v8_function functionRef);
void progp_AddFunctionToObject(const char* groupName, const v8::Local<v8::Object> &v8Object, const char* functionName, f_progp_v8_function functionRef);

void progp_CreateFunctionGroup(const std::string& group, const v8::Local<v8::Object> &v8Object);
void progp_CreateFunctionGroup_Internal(const std::string& group, v8::Local<v8::Object> v8Host);

s_progp_v8_function* progpFunctions_NewPointer(const v8::Local<v8::Function> &v8Function);

void progp_PrintErrorMessage(s_progp_v8_errorMessage* msg);

#ifdef PROGP_ADD_EXTERNAL_FUNCTIONS
void progp_DeclareAllFunctions_External();
#endif

#endif //PROGP_V8_H
