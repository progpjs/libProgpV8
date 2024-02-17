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
#ifndef PROGP_STANDALONE

#include "./progpV8.h"
#include "_cgo_export.h"

#include <iostream>

void handleErrorMessage(ProgpContext progpCtx, s_progp_v8_errorMessage* error) {
    cppCallOnJavascriptError((void*)progpCtx->data, error);
}

void handleDynamicFunctionProvider(ProgpContext progpCtx, char *groupName) {
    cppOnDynamicFunctionProviderRequested((void*)progpCtx->data, groupName);
}

s_progp_anyValue handleOnDynamicFunctionCalled(ProgpContext progpCtx, char* functionName, s_progp_anyValue* anyValueArray, int valueCount, ProgpEvent currentEvent) {
    return cppOnDynamicFunctionCalled((void*)progpCtx->data, functionName, anyValueArray, valueCount, currentEvent);
}

void handle_oneNoMoreTask(ProgpContext progpCtx) {
    cppCallOnNoMoreTask((void*)progpCtx->data);
}

// Executed by Go.
void cgoInitialize() {
    progpConfig_SetJavascriptFunctionProvider(exposeGoFunctionsToV8);
    progpConfig_SetJavascriptErrorListener(handleErrorMessage);
    progpConfig_OnDebuggerExitedListener(cppCallOnDebuggerExited);
    progpConfig_OnNoMoreTask(handle_oneNoMoreTask);
    progpConfig_SetDraftFunctionListener(handleOnDynamicFunctionCalled);
    progpConfig_SetDynamicFunctionProvider(handleDynamicFunctionProvider);
    progpConfig_SetAllowedFunctionChecker(cppCheckAllowedFunction);
    progpConfig_OnEventFinished(cppOnEventFinished);
}

#endif // !PROGP_STANDALONE