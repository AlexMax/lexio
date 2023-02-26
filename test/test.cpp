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

TEST_CASE("Read UInt8", "[read]")
{
    std::array<uint8_t, 1> data = {0x88};
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Write UInt8", "[write]")
{
    std::array<uint8_t, 1> data;
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    LexIO::WriteU8(buffer, 0x88);
    REQUIRE(data[0] == 0x88);
}

TEST_CASE("Read U16LE", "[read]")
{
    std::array<uint8_t, 2> data = {0x88, 0x99};
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    REQUIRE(LexIO::ReadU16LE(buffer) == 0x9988);
}

TEST_CASE("Write UInt16LE", "[write]")
{
    std::array<uint8_t, 2> data;
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    LexIO::WriteU16LE(buffer, 0x9988);
    REQUIRE(data[0] == 0x88);
    REQUIRE(data[1] == 0x99);
}

TEST_CASE("Read UInt32", "[read]")
{
    std::array<uint8_t, 4> data = {0x88, 0x99, 0xaa, 0xbb};
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    REQUIRE(LexIO::ReadU32LE(buffer) == 0xbbaa9988);
}

TEST_CASE("Write UInt32LE", "[write]")
{
    std::array<uint8_t, 4> data;
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    LexIO::WriteU32LE(buffer, 0xbbaa9988);
    REQUIRE(data[0] == 0x88);
    REQUIRE(data[1] == 0x99);
    REQUIRE(data[2] == 0xaa);
    REQUIRE(data[3] == 0xbb);
}

TEST_CASE("Read UInt64", "[read]")
{
    std::array<uint8_t, 8> data = {0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    REQUIRE(LexIO::ReadU64LE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("Write UInt64LE", "[write]")
{
    std::array<uint8_t, 8> data;
    LexIO::SpanBuffer buffer(std::span<uint8_t>(data.begin(), data.end()));
    LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988);
    REQUIRE(data[0] == 0x88);
    REQUIRE(data[1] == 0x99);
    REQUIRE(data[2] == 0xaa);
    REQUIRE(data[3] == 0xbb);
    REQUIRE(data[4] == 0xcc);
    REQUIRE(data[5] == 0xdd);
    REQUIRE(data[6] == 0xee);
    REQUIRE(data[7] == 0xff);
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
    std::array<uint8_t, 4> data = {0x88, 0x99, 0xaa, 0xbb};
    std::span<uint8_t> dataSpan(data.data(), data.size());

    SECTION("Test std::span")
    {
        LexIO::SpanBuffer buf(dataSpan);
    }

    SECTION("Test std::array")
    {
        using ArrayBuffer = LexIO::StdBuffer<std::array<uint8_t, 4>>;

        //ArrayBuffer buf(data.begin(), data.end());
    }

    SECTION("Test std::vector")
    {
        using VectorBuffer = LexIO::StdBuffer<std::vector<uint8_t>>;
        using VectorBufferResize = LexIO::StdBufferResize<std::vector<uint8_t>>;

        //VectorBuffer buf(data.begin(), data.end();
        //VectorBufferResize buf;
    }
}

#pragma endregion

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int result = Catch::Session().run(argc, argv);
    return result;
}
