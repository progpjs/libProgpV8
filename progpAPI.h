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
#ifndef PROGP_V8_API_H
#define PROGP_V8_API_H

// Keep it as-is and don't replace by cstddef
// since currently go compiler don't support cstddef.
//
#include <stddef.h> // NOLINT(*-deprecated-headers)

#include <stdint.h> // NOLINT(*-deprecated-headers)

#if __cplusplus
extern "C" {
#else
#define bool int
#endif

//region C Structs + sizeOf

typedef struct s_progp_v8_stackTraceFrame {
    int line;
    int column;
    char* function;
    char* source;
} s_progp_v8_stackTraceFrame;

typedef struct s_progp_v8_errorMessage {
    char* error;
    int errorLevel;

    int startColumn;
    int endColumn;

    int startPosition;
    int endPosition;

    char* sourceMapUrl;
    char* resourceName;

    int stackTraceFrameCount;
    int stackTraceFrameSize;
    s_progp_v8_stackTraceFrame *stackTraceFrameArray;
} s_progp_v8_errorMessage;

typedef struct { const char *p; ptrdiff_t n; } s_progp_goString;

typedef struct { const char *p; int n; } s_progp_goStringOut;

typedef struct s_progp_v8_function s_progp_v8_function;
typedef s_progp_v8_function* ProgpV8FunctionPtr;

typedef struct s_progp_context s_progp_context;
typedef s_progp_context* ProgpContext;

typedef struct s_progp_event s_progp_event;

typedef struct s_progp_event {
    uintptr_t id;
    int refCount;
    s_progp_event* previousEvent;
    uintptr_t contextData;
} s_progp_event;

typedef s_progp_event* ProgpEvent;

typedef struct s_progp_anyValue {
    int valueType;
    double number;
    void* voidPtr;
    int size;

    // Used when returning a value in order to
    // declare an error. The value is automatically
    // deleted after consumed.
    //
    char* errorMessage;

    // mustFree is used from the Go side.
    // Allows knowing if the value voidPtr must be free.
    //
    int mustFree;
} s_progp_anyValue;

typedef struct s_progp_v8_buffer {
    void* buffer;
    int length;
} s_progp_v8_buffer;
//
typedef s_progp_v8_buffer *ProgpV8BufferPtr;

int progp_GetSizeOfAnyValueStruct();

//endregion

//region Function types

typedef void (*f_progp_noParamNoReturn)();
typedef void (*f_progp_onNoMoreTasksForContext)(ProgpContext progpCtx);
typedef void (*f_progp_eventFinished)(uintptr_t eventId);
typedef void (*f_progp_javascriptErrorListener)(ProgpContext progpCtx, s_progp_v8_errorMessage* error);
typedef s_progp_anyValue (*f_draftFunctionListener)(ProgpContext progpCtx, char* functionName, s_progp_anyValue* anyValueArray, int valueCount, ProgpEvent currentEvent);

//endregion

//region Functions return type

#define ProgpFunctionReturn char *errorMessage; \
const char *constErrorMessage;  \
s_progp_event *currentEvent;\
void* eventDisposer;  \
bool isAsync;  \
int contextId;

typedef struct ProgpFunctionReturnVoid {
    ProgpFunctionReturn
} ProgpFunctionReturnVoid;

typedef struct ProgpFunctionReturnDouble {
    ProgpFunctionReturn
    double value;
} ProgpFunctionReturnDouble;

typedef struct ProgpFunctionReturnInt {
    ProgpFunctionReturn
    int value;
} ProgpFunctionReturnInt;

typedef struct ProgpFunctionReturnLong {
    ProgpFunctionReturn
    long value;
} ProgpFunctionReturnLong;

typedef struct ProgpFunctionReturnString {
    ProgpFunctionReturn
    // Here it's a ref on a GoString.
    void* value;
} ProgpFunctionReturnString;

typedef struct ProgpFunctionReturnArrayBuffer {
    ProgpFunctionReturn
    void* value;
    int size;
} ProgpFunctionReturnArrayBuffer;

//endregion

//region Calling function from Golang

#define FCT_CALLBACK_PARAMS s_progp_v8_function* functionRef, bool mustDecreaseTaskCount, bool mustDisposeFunction, ProgpEvent eventToRestore, uintptr_t resourceContainerId

void progp_CallFunctionWithUndefined(FCT_CALLBACK_PARAMS);
void progp_CallFunctionWithErrorP1(FCT_CALLBACK_PARAMS, const char* str, size_t strLen);
void progp_CallFunctionWithStringP2(FCT_CALLBACK_PARAMS, const char* str, size_t strLen);
void progp_CallFunctionWithArrayBufferP2(FCT_CALLBACK_PARAMS, const void* buffer, size_t bufferSize);
void progp_CallFunctionWithDoubleP2(FCT_CALLBACK_PARAMS, double value);
void progp_CallFunctionWithBoolP2(FCT_CALLBACK_PARAMS, bool value);

//endregion

//region Calling Golang function

void progp_DeclareDynamicFunction(ProgpContext progpCtx, const char* functionName);

//endregion

//region Golang binding

#ifndef PROGP_STANDALONE

void cgoInitialize();

#endif

//endregion

void progp_StartupEngine();
ProgpContext progp_CreateNewContext(uintptr_t data);
void progp_DisposeContext(ProgpContext progpCtx);

void progp_IncreaseContextRef(ProgpContext progpCtx);
void progp_DecreaseContextRef(ProgpContext progpCtx);

const char* progp_GetV8EngineVersion();

bool progp_ExecuteScript(ProgpContext progpCtx, const char* scriptContent, const char* scriptOrigin, uintptr_t eventId);

void progp_WaitDebuggerReady();

void* progp_CopyGoBuffer(void* buffer, int size);

#if __cplusplus
}
#endif

#endif //PROGP_V8_API_H