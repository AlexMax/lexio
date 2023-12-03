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

TEST_CASE("TryReadFloat32LE/ReadFloat32LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});

    int exp = 0;
    LexIO::float32_t test, x;
    REQUIRE(LexIO::TryReadFloat32LE(test, buffer) == true);
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE(LexIO::TryReadFloat32LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE_NOTHROW(test = LexIO::ReadFloat32LE(buffer));
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE_THROWS(LexIO::ReadFloat32LE(buffer));
}

TEST_CASE("TryWriteFloat32LE/WriteFloat32LE")
{
    const LexIO::float32_t test = std::ldexp(-0.666405201f, -7);

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float32_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float32_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteFloat32LE(buffer, test) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(LexIO::TryWriteFloat32LE(buffer, test) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float32_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float32_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteFloat32LE(buffer, test));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE_THROWS(LexIO::WriteFloat32LE(buffer, test));
    }
}

TEST_CASE("TryReadFloat32BE/ReadFloat32BE")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});

    int exp = 0;
    LexIO::float32_t test, x;
    REQUIRE(LexIO::TryReadFloat32BE(test, buffer) == true);
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE(LexIO::TryReadFloat32BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE_NOTHROW(test = LexIO::ReadFloat32BE(buffer));
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
    REQUIRE_THROWS(LexIO::ReadFloat32BE(buffer));
}

TEST_CASE("TryWriteFloat32BE/WriteFloat32BE")
{
    const LexIO::float32_t test = std::ldexp(-0.666405201f, -7);

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float32_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float32_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteFloat32BE(buffer, test) == true);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWriteFloat32BE(buffer, test) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float32_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float32_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteFloat32BE(buffer, test));
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::WriteFloat32BE(buffer, test));
    }
}

TEST_CASE("TryReadFloat64LE/ReadFloat64LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    int exp = 0;
    LexIO::float64_t test, x;
    REQUIRE(LexIO::TryReadFloat64LE(test, buffer) == true);
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
    REQUIRE(LexIO::TryReadFloat64LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE_NOTHROW(test = LexIO::ReadFloat64LE(buffer));
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
    REQUIRE_THROWS(LexIO::ReadFloat64LE(buffer));
}

TEST_CASE("TryWriteFloat64LE/WriteFloat64LE")
{
    const LexIO::float64_t test = std::ldexp(-0.96457516339869276, 1024);

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float64_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float64_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteFloat64LE(buffer, test) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(LexIO::TryWriteFloat64LE(buffer, test) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float64_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float64_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteFloat64LE(buffer, test));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE_THROWS(LexIO::WriteFloat64LE(buffer, test));
    }
}

TEST_CASE("TryReadFloat64BE/ReadFloat64BE")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});

    int exp = 0;
    LexIO::float64_t test, x;
    REQUIRE(LexIO::TryReadFloat64BE(test, buffer) == true);
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
    REQUIRE(LexIO::TryReadFloat64BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE_NOTHROW(test = LexIO::ReadFloat64BE(buffer));
    x = std::frexp(test, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
    REQUIRE_THROWS(LexIO::ReadFloat64BE(buffer));
}

TEST_CASE("TryWriteFloat64LE/WriteFloat64BE")
{
    const LexIO::float64_t test = std::ldexp(-0.96457516339869276, 1024);

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float64_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float64_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteFloat64BE(buffer, test) == true);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWriteFloat64BE(buffer, test) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(LexIO::float64_t)> buffer;
        const LexIO::ArrayStream<sizeof(LexIO::float64_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteFloat64BE(buffer, test));
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::WriteFloat64BE(buffer, test));
    }
}
