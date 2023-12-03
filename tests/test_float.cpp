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
#include <cmath>

TEST_CASE("TryReadFloatLE/ReadFloatLE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});

    int exp = 0;
    float test, x;
    REQUIRE(LexIO::TryReadFloatLE(test, buffer) == true);
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE(LexIO::TryReadFloatLE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE_NOTHROW(test = LexIO::ReadFloatLE(buffer));
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE_THROWS(LexIO::ReadFloatLE(buffer));
}

TEST_CASE("TryWriteFloatLE/WriteFloatLE")
{
    const float test = std::ldexp(-0.666405201f, -7);

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(float)> buffer;
        const LexIO::ArrayStream<sizeof(float)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteFloatLE(buffer, test) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(LexIO::TryWriteFloatLE(buffer, test) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(float)> buffer;
        const LexIO::ArrayStream<sizeof(float)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteFloatLE(buffer, test));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE_THROWS(LexIO::WriteFloatLE(buffer, test));
    }
}

TEST_CASE("TryReadFloatBE/ReadFloatBE")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});

    int exp = 0;
    float test, x;
    REQUIRE(LexIO::TryReadFloatBE(test, buffer) == true);
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE(LexIO::TryReadFloatBE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE_NOTHROW(test = LexIO::ReadFloatBE(buffer));
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE_THROWS(LexIO::ReadFloatBE(buffer));
}

TEST_CASE("TryWriteFloatBE/WriteFloatBE")
{
    const float test = std::ldexp(-0.666405201f, -7);

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(float)> buffer;
        const LexIO::ArrayStream<sizeof(float)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteFloatBE(buffer, test) == true);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWriteFloatBE(buffer, test) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(float)> buffer;
        const LexIO::ArrayStream<sizeof(float)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteFloatBE(buffer, test));
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::WriteFloatBE(buffer, test));
    }
}

TEST_CASE("ReadDoubleLE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    const double check = LexIO::ReadDoubleLE(buffer);

    int exp;
    const double x = std::frexp(check, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
}

TEST_CASE("WriteDoubleLE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteDoubleLE(buffer, std::ldexp(-0.96457516339869276, 1024));
    LexIO::Rewind(buffer);

    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xcc);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0xee);
    REQUIRE(LexIO::ReadU8(buffer) == 0xff);
}

TEST_CASE("ReadDoubleBE")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});
    const double check = LexIO::ReadDoubleBE(buffer);

    int exp;
    const double x = std::frexp(check, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
}

TEST_CASE("WriteDoubleBE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteDoubleBE(buffer, std::ldexp(-0.96457516339869276, 1024));
    LexIO::Rewind(buffer);

    REQUIRE(LexIO::ReadU8(buffer) == 0xff);
    REQUIRE(LexIO::ReadU8(buffer) == 0xee);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0xcc);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}