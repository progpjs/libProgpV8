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

#include "./progpTools.h"

#include <string>
#include <iostream>
#include <memory>
#include <fstream>

void progpTools_pressKey(const char* msg) {
    PROGP_PRINT("Press key " << msg);
    std::cin.get();
}

std::string progpTools_readFileContent(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return "";

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

std::string progpTools_GetCurrentDirectory() {
    return std::filesystem::current_path();
}
