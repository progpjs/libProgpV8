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

#ifndef PROGP_V8_TOOLS_H
#define PROGP_V8_TOOLS_H

#include "./progpV8.h"

#include <string>
#include <iostream>
#include <utility>
#include <filesystem>
#include <memory>
#include <string>
#include <execinfo.h>
#include <map>

void progpTools_pressKey(const char* msg);
std::string progpTools_readFileContent(const std::string &filename);
std::string progpTools_GetCurrentDirectory();

#endif //PROGP_V8_TOOLS_H
