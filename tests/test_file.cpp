//
// Copyright 2023 Lexi Mayfield
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "./test.h"
#include "catch2/catch_all.hpp"

//******************************************************************************

TEST_CASE("Test file opened in read mode")
{
    constexpr const char *firstLine = "The quick brown fox";

    auto file = LexIO::Open(LEXIO_TEST_DIR "/test_file.txt", LexIO::OpenMode::read);

    // Test reading.
    uint8_t readBuffer[32];
    LexIO::Read(readBuffer, file);
    size_t testLen = strlen(firstLine);
    readBuffer[testLen] = '\0';                                                  // NOLINT
    REQUIRE(!strcmp(reinterpret_cast<const char *>(&readBuffer[0]), firstLine)); // NOLINT

    // Test writing.
    const uint8_t writeBuffer[32] = {0x00};
    REQUIRE_THROWS(LexIO::Write(file, writeBuffer));

    // Test seeking.
    size_t pos = LexIO::Seek(file, 2, LexIO::seek::start);
    REQUIRE(pos == 2);

    pos = LexIO::Seek(file, 6, LexIO::seek::current);
    REQUIRE(pos == 8);

    pos = LexIO::Seek(file, 2, LexIO::seek::end);
    REQUIRE((pos == 47 || pos == 49)); // File contains two newlines.
}
