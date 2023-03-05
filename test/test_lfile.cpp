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

#include "test_nonstd.h"

#include "lexio.h"

#include <catch2/catch_all.hpp>

//------------------------------------------------------------------------------

TEST_CASE("Test file opened in read mode", "[lfile]")
{
    constexpr const char *firstLine = "The quick brown fox";

    auto file = LexIO::LOpen("../test/test_file.txt", LexIO::LOpenMode::read);

    // Test reading.
    uint8_t readBuffer[32];
    LexIO::RawRead(file, readBuffer);
    size_t testLen = strlen(firstLine);
    readBuffer[testLen] = '\0';
    REQUIRE(!strcmp((const char *)&readBuffer[0], firstLine));

    // Test writing.
    const uint8_t writeBuffer[32] = {0x00};
    REQUIRE_THROWS(LexIO::RawWrite(file, writeBuffer));

    // Test seeking.
    size_t pos = LexIO::Seek(file, LexIO::WhenceStart(2));
    REQUIRE(pos == 2);

    pos = LexIO::Seek(file, LexIO::WhenceCurrent(6));
    REQUIRE(pos == 8);

    pos = LexIO::Seek(file, LexIO::WhenceEnd(2));
    REQUIRE(pos == 47);
}
