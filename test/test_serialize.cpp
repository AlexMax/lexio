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

TEST_CASE("Read UInt8", "[serialize]")
{
    VectorBuffer buffer = {0x88};
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Write UInt8", "[serialize]")
{
    VectorBuffer buffer;
    LexIO::WriteU8(buffer, 0x88);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Read U16LE", "[serialize]")
{
    VectorBuffer buffer({0x88, 0x99});
    REQUIRE(LexIO::ReadU16LE(buffer) == 0x9988);
}

TEST_CASE("Write UInt16LE", "[serialize]")
{
    VectorBuffer buffer;
    LexIO::WriteU16LE(buffer, 0x9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
}

TEST_CASE("Read UInt32", "[serialize]")
{
    VectorBuffer buffer({0x88, 0x99, 0xaa, 0xbb});
    REQUIRE(LexIO::ReadU32LE(buffer) == 0xbbaa9988);
}

TEST_CASE("Write UInt32LE", "[serialize]")
{
    VectorBuffer buffer;
    LexIO::WriteU32LE(buffer, 0xbbaa9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
}

TEST_CASE("Read UInt64", "[serialize]")
{
    VectorBuffer buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    REQUIRE(LexIO::ReadU64LE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("Write UInt64LE", "[serialize]")
{
    VectorBuffer buffer;
    LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988);
    LexIO::Seek(buffer, LexIO::WhenceStart(0));

    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xcc);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0xee);
    REQUIRE(LexIO::ReadU8(buffer) == 0xff);
}
