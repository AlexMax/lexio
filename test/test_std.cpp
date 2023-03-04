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

using VectorBuffer = LexIO::StdBuffer<std::vector<uint8_t>>;

template <typename T>
void CommonTest(T &buf)
{
    // Test reading.
    LexIO::WriteU8(buf, 192);
    REQUIRE(LexIO::Tell(buf) == 1);
    LexIO::WriteU16LE(buf, 1993);
    REQUIRE(LexIO::Tell(buf) == 3);
    LexIO::WriteU32LE(buf, 20200602);
    REQUIRE(LexIO::Tell(buf) == 7);
    LexIO::WriteU64LE(buf, uint64_t(UINT32_MAX) + 20200613);
    REQUIRE(LexIO::Tell(buf) == 15);
    REQUIRE(LexIO::Length(buf) == 15);

    // Test writing.
    LexIO::Seek(buf, LexIO::WhenceStart(0));
    REQUIRE(LexIO::Tell(buf) == 0);
    REQUIRE(LexIO::ReadU8(buf) == 192);
    REQUIRE(LexIO::ReadU16LE(buf) == 1993);
    REQUIRE(LexIO::ReadU32LE(buf) == 20200602);
    REQUIRE(LexIO::ReadU64LE(buf) == uint64_t(UINT32_MAX) + 20200613);
    REQUIRE(LexIO::Tell(buf) == 15);

    // Test seeking.
    LexIO::Seek(buf, LexIO::WhenceStart(2));
    REQUIRE(LexIO::Tell(buf) == 2);
    LexIO::Seek(buf, LexIO::WhenceCurrent(2));
    REQUIRE(LexIO::Tell(buf) == 4);
    LexIO::Seek(buf, LexIO::WhenceCurrent(-2));
    REQUIRE(LexIO::Tell(buf) == 2);
    LexIO::Seek(buf, LexIO::WhenceEnd(2));
    REQUIRE(LexIO::Tell(buf) == 13);

    // Test overwriting data.
    LexIO::WriteU16LE(buf, 1993);
    REQUIRE(LexIO::Tell(buf) == 15);
    REQUIRE(LexIO::Length(buf) == 15);
    LexIO::Seek(buf, LexIO::WhenceCurrent(-2));
    REQUIRE(LexIO::Tell(buf) == 13);
    REQUIRE(LexIO::Length(buf) == 15);
    REQUIRE(LexIO::ReadU16LE(buf) == 1993);
}

constexpr size_t TEST_SIZE = 15;

TEST_CASE("Test StdBuffer with std::vector", "[std]")
{
    VectorBuffer buf;
    CommonTest(buf);
    REQUIRE_NOTHROW(LexIO::WriteU8(buf, 0));
    REQUIRE(LexIO::Tell(buf) == TEST_SIZE + 1);
}

TEST_CASE("Test StaticStdBuffer with std::array", "[std]")
{
    using ArrayBuffer = LexIO::StaticStdBuffer<std::array<uint8_t, TEST_SIZE>>;
    ArrayBuffer buf;
    CommonTest(buf);
    REQUIRE_THROWS(LexIO::WriteU8(buf, 0));
    REQUIRE(LexIO::Tell(buf) == TEST_SIZE);
}

TEST_CASE("Test FixedStdBuffer with pre-allocated size", "[std]")
{
    using FixedVectorBuffer = LexIO::FixedStdBuffer<std::vector<uint8_t>>;
    FixedVectorBuffer buf(TEST_SIZE);
    CommonTest(buf);
    REQUIRE_THROWS(LexIO::WriteU8(buf, 0));
    REQUIRE(LexIO::Tell(buf) == TEST_SIZE);
}
