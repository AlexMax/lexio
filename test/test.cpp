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

#include "lexio.h"

#include <catch2/catch_all.hpp>

//------------------------------------------------------------------------------

#pragma region "Read/Write Tests"

using VectorBuffer = LexIO::StdBuffer<std::vector<uint8_t>>;

TEST_CASE("Read UInt8", "[read]")
{
    VectorBuffer buffer = {0x88};
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Write UInt8", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU8(buffer, 0x88);
    REQUIRE(buffer.Data()[0] == 0x88);
}

TEST_CASE("Read U16LE", "[read]")
{
    VectorBuffer buffer({0x88, 0x99});
    REQUIRE(LexIO::ReadU16LE(buffer) == 0x9988);
}

TEST_CASE("Write UInt16LE", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU16LE(buffer, 0x9988);
    REQUIRE(buffer.Data()[0] == 0x88);
    REQUIRE(buffer.Data()[1] == 0x99);
}

TEST_CASE("Read UInt32", "[read]")
{
    VectorBuffer buffer({0x88, 0x99, 0xaa, 0xbb});
    REQUIRE(LexIO::ReadU32LE(buffer) == 0xbbaa9988);
}

TEST_CASE("Write UInt32LE", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU32LE(buffer, 0xbbaa9988);
    REQUIRE(buffer.Data()[0] == 0x88);
    REQUIRE(buffer.Data()[1] == 0x99);
    REQUIRE(buffer.Data()[2] == 0xaa);
    REQUIRE(buffer.Data()[3] == 0xbb);
}

TEST_CASE("Read UInt64", "[read]")
{
    VectorBuffer buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    REQUIRE(LexIO::ReadU64LE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("Write UInt64LE", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988);
    REQUIRE(buffer.Data()[0] == 0x88);
    REQUIRE(buffer.Data()[1] == 0x99);
    REQUIRE(buffer.Data()[2] == 0xaa);
    REQUIRE(buffer.Data()[3] == 0xbb);
    REQUIRE(buffer.Data()[4] == 0xcc);
    REQUIRE(buffer.Data()[5] == 0xdd);
    REQUIRE(buffer.Data()[6] == 0xee);
    REQUIRE(buffer.Data()[7] == 0xff);
}

#pragma endregion

//------------------------------------------------------------------------------

#pragma region "Sequence Buffer Tests"

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>

TEST_CASE("LexIO::StdBuffer", "[seqbuf]")
{
    SECTION("Test StaticStdBuffer with std::array")
    {
        using ArrayBuffer = LexIO::StaticStdBuffer<std::array<uint8_t, 4>>;
        ArrayBuffer buf;
    }

    SECTION("Test FixedStdBuffer with pre-allocated size")
    {
        using FixedVectorBuffer = LexIO::FixedStdBuffer<std::vector<uint8_t>>;
        FixedVectorBuffer(size_t(4));
    }
}

#pragma endregion

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int result = Catch::Session().run(argc, argv);
    return result;
}
