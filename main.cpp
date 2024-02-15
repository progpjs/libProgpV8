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
#ifdef PROGP_STANDALONE

#include "./progpV8.h"
#include "./progpTools.h"

s_progp_anyValue draftFunctionListener(char* functionName, s_progp_anyValue* anyValueArray, int valueCount, ProgpEvent currentEvent)  {
    auto text = std::string("hello function ") + functionName+ "!";

    s_progp_anyValue res;
    res.valueType = PROGP_ANY_VALUE_STRING;
    res.voidPtr = (void*) strdup(text.c_str());
    res.mustFree = true;
    res.size = text.size();

    return res;
}

void onJavascriptError(s_progp_v8_errorMessage* error) {
    progp_PrintErrorMessage(error);
}

void JsTest(const v8::FunctionCallbackInfo<v8::Value> &callInfo) {
    progp_handleDraftFunction(callInfo);
}

void createTestFunctions(const std::string &group, v8::Local<v8::Object> v8Host) {
    if (group=="global") {
        PROGP_BIND_FUNCTION("global", "test", JsTest);
    }
}

int main() {
    progpConfig_SetJavascriptErrorListener(onJavascriptError);
    progpConfig_SetJavascriptFunctionProvider(createTestFunctions);
    progpConfig_SetDraftFunctionListener(draftFunctionListener);

    progp_StartupEngine();

    //progp_DeclareDynamicFunction("draftFunction");

    //progp_WaitDebuggerReady();

    auto filePath = progpTools_GetCurrentDirectory() + "/stdin.js";
    auto script = progpTools_readFileContent(filePath);
    progp_ExecuteScript(script.c_str(), filePath.c_str(), 0);

    return 0;
}

#endif // PROGP_STANDALONE
