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

//******************************************************************************

int32_t BBAA9988i32()
{
    uint32_t src = 0xbbaa9988;
    int32_t dst;
    std::memcpy(&dst, &src, sizeof(dst));
    return dst;
}

int64_t FFEEDDCCBBAA9988i64()
{
    uint64_t src = 0xffeeddccbbaa9988;
    int64_t dst;
    std::memcpy(&dst, &src, sizeof(dst));
    return dst;
}

TEST_CASE("TryReadU8/ReadU8")
{
    LexIO::VectorStream buffer = {0x88};

    uint8_t test;
    REQUIRE(LexIO::TryReadU8(test, buffer) == true);
    REQUIRE(test == 0x88);
    REQUIRE(LexIO::TryReadU8(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE_THROWS(LexIO::ReadU8(buffer));
}

TEST_CASE("TryWriteU8/WriteU8")
{
    {
        LexIO::ArrayStream<sizeof(uint8_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint8_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteU8(buffer, 0x88) == true);
        REQUIRE(cbuffer.Container()[0] == 0x88);
        REQUIRE(LexIO::TryWriteU8(buffer, 0x88) == false);
    }

    {
        LexIO::ArrayStream<1> buffer;
        const LexIO::ArrayStream<1> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteU8(buffer, 0x88));
        REQUIRE(cbuffer.Container()[0] == 0x88);
        REQUIRE_THROWS(LexIO::WriteU8(buffer, 0x88));
    }
}

TEST_CASE("TryRead8/Read8")
{
    LexIO::VectorStream buffer = {0x88};

    int8_t test;
    REQUIRE(LexIO::TryRead8(test, buffer) == true);
    REQUIRE(test == -120);
    REQUIRE(LexIO::TryRead8(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read8(buffer) == -120);
    REQUIRE_THROWS(LexIO::Read8(buffer));
}

TEST_CASE("TryWrite8/Write8")
{
    {
        LexIO::ArrayStream<sizeof(int8_t)> buffer;
        const LexIO::ArrayStream<sizeof(int8_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWrite8(buffer, -120) == true);
        REQUIRE(cbuffer.Container()[0] == 0x88);
        REQUIRE(LexIO::TryWrite8(buffer, -120) == false);
    }

    {
        LexIO::ArrayStream<sizeof(int8_t)> buffer;
        const LexIO::ArrayStream<sizeof(int8_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::Write8(buffer, -120));
        REQUIRE(cbuffer.Container()[0] == 0x88);
        REQUIRE_THROWS(LexIO::Write8(buffer, -120));
    }
}

TEST_CASE("TryReadU16LE/ReadU16LE")
{
    LexIO::VectorStream buffer({0x88, 0x99});

    uint16_t test;
    REQUIRE(LexIO::TryReadU16LE(test, buffer) == true);
    REQUIRE(test == 0x9988);
    REQUIRE(LexIO::TryReadU16LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadU16LE(buffer) == 0x9988);
    REQUIRE_THROWS(LexIO::ReadU16LE(buffer));
}

TEST_CASE("TryWriteU16LE/WriteU16LE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint16_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint16_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteU16LE(buffer, 0x9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(LexIO::TryWriteU16LE(buffer, 0x9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint16_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint16_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteU16LE(buffer, 0x9988));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE_THROWS(LexIO::WriteU16LE(buffer, 0x9988));
    }
}

TEST_CASE("TryReadU16BE/ReadU16BE")
{
    LexIO::VectorStream buffer({0x88, 0x99});

    uint16_t test;
    REQUIRE(LexIO::TryReadU16BE(test, buffer) == true);
    REQUIRE(test == 0x8899);
    REQUIRE(LexIO::TryReadU16BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadU16BE(buffer) == 0x8899);
    REQUIRE_THROWS(LexIO::ReadU16BE(buffer));
}

TEST_CASE("TryWriteU16BE/WriteU16BE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint16_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint16_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteU16BE(buffer, 0x9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWriteU16BE(buffer, 0x9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint16_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint16_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteU16BE(buffer, 0x9988));
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::WriteU16BE(buffer, 0x9988));
    }
}

TEST_CASE("TryRead16LE/Read16LE")
{
    LexIO::VectorStream buffer({0x88, 0x99});

    int16_t test;
    REQUIRE(LexIO::TryRead16LE(test, buffer) == true);
    REQUIRE(test == -26232);
    REQUIRE(LexIO::TryRead16LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read16LE(buffer) == -26232);
    REQUIRE_THROWS(LexIO::Read16LE(buffer));
}

TEST_CASE("TryWrite16LE/Write16LE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int16_t)> buffer;
        const LexIO::ArrayStream<sizeof(int16_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWrite16LE(buffer, -26232) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(LexIO::TryWrite16LE(buffer, -26232) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int16_t)> buffer;
        const LexIO::ArrayStream<sizeof(int16_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::Write16LE(buffer, -26232));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE_THROWS(LexIO::Write16LE(buffer, -26232));
    }
}

TEST_CASE("TryRead16BE/Read16BE")
{
    LexIO::VectorStream buffer({0x99, 0x88});

    int16_t test;
    REQUIRE(LexIO::TryRead16BE(test, buffer) == true);
    REQUIRE(test == -26232);
    REQUIRE(LexIO::TryRead16BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read16BE(buffer) == -26232);
    REQUIRE_THROWS(LexIO::Read16BE(buffer));
}

TEST_CASE("TryWrite16BE/Write16BE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int16_t)> buffer;
        const LexIO::ArrayStream<sizeof(int16_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWrite16BE(buffer, -26232) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWrite16BE(buffer, -26232) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int16_t)> buffer;
        const LexIO::ArrayStream<sizeof(int16_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::Write16BE(buffer, -26232));
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::Write16BE(buffer, -26232));
    }
}

TEST_CASE("TryReadU32LE/ReadU32LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});

    uint32_t test;
    REQUIRE(LexIO::TryReadU32LE(test, buffer) == true);
    REQUIRE(test == 0xbbaa9988);
    REQUIRE(LexIO::TryReadU32LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadU32LE(buffer) == 0xbbaa9988);
    REQUIRE_THROWS(LexIO::ReadU32LE(buffer));
}

TEST_CASE("TryWriteU32LE/WriteU32LE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint32_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint32_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteU32LE(buffer, 0xbbaa9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(LexIO::TryWriteU32LE(buffer, 0xbbaa9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint32_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint32_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteU32LE(buffer, 0xbbaa9988));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE_THROWS(LexIO::WriteU32LE(buffer, 0xbbaa9988));
    }
}

TEST_CASE("TryReadU32BE/ReadU32BE")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});

    uint32_t test;
    REQUIRE(LexIO::TryReadU32BE(test, buffer) == true);
    REQUIRE(test == 0xbbaa9988);
    REQUIRE(LexIO::TryReadU32BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadU32BE(buffer) == 0xbbaa9988);
    REQUIRE_THROWS(LexIO::ReadU32BE(buffer));
}

TEST_CASE("TryWriteU32BE/WriteU32BE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint32_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint32_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteU32BE(buffer, 0xbbaa9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWriteU32BE(buffer, 0xbbaa9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint32_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint32_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteU32BE(buffer, 0xbbaa9988));
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::WriteU32BE(buffer, 0xbbaa9988));
    }
}

TEST_CASE("TryRead32LE/Read32LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});

    int32_t test;
    REQUIRE(LexIO::TryRead32LE(test, buffer) == true);
    REQUIRE(test == -1146447480);
    REQUIRE(LexIO::TryRead32LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read32LE(buffer) == -1146447480);
    REQUIRE_THROWS(LexIO::Read32LE(buffer));
}

TEST_CASE("TryWrite32LE/Write32LE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int32_t)> buffer;
        const LexIO::ArrayStream<sizeof(int32_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWrite32LE(buffer, -1146447480) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(LexIO::TryWrite32LE(buffer, -1146447480) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int32_t)> buffer;
        const LexIO::ArrayStream<sizeof(int32_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::Write32LE(buffer, -1146447480));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE_THROWS(LexIO::Write32LE(buffer, -1146447480));
    }
}

TEST_CASE("TryRead32BE/Read32BE")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});

    int32_t test;
    REQUIRE(LexIO::TryRead32BE(test, buffer) == true);
    REQUIRE(test == -1146447480);
    REQUIRE(LexIO::TryRead32BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read32BE(buffer) == -1146447480);
    REQUIRE_THROWS(LexIO::Read32BE(buffer));
}

TEST_CASE("TryWrite32BE/Write32BE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int32_t)> buffer;
        const LexIO::ArrayStream<sizeof(int32_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWrite32BE(buffer, -1146447480) == true);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWrite32BE(buffer, -1146447480) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int32_t)> buffer;
        const LexIO::ArrayStream<sizeof(int32_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::Write32BE(buffer, -1146447480));
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::Write32BE(buffer, -1146447480));
    }
}

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

TEST_CASE("TryReadU64LE/ReadU64LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    uint64_t test;
    REQUIRE(LexIO::TryReadU64LE(test, buffer) == true);
    REQUIRE(test == 0xffeeddccbbaa9988);
    REQUIRE(LexIO::TryReadU64LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadU64LE(buffer) == 0xffeeddccbbaa9988);
    REQUIRE_THROWS(LexIO::ReadU64LE(buffer));
}

TEST_CASE("TryWriteU64LE/WriteU64LE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint64_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint64_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteU64LE(buffer, 0xffeeddccbbaa9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(LexIO::TryWriteU64LE(buffer, 0xffeeddccbbaa9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint64_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint64_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE_THROWS(LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988));
    }
}

TEST_CASE("TryReadU64BE/ReadU64BE")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});

    uint64_t test;
    REQUIRE(LexIO::TryReadU64BE(test, buffer) == true);
    REQUIRE(test == 0xffeeddccbbaa9988);
    REQUIRE(LexIO::TryReadU64BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadU64BE(buffer) == 0xffeeddccbbaa9988);
    REQUIRE_THROWS(LexIO::ReadU64BE(buffer));
}

TEST_CASE("TryWriteU64BE/WriteU64BE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint64_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint64_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteU64BE(buffer, 0xffeeddccbbaa9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWriteU64BE(buffer, 0xffeeddccbbaa9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint64_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint64_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteU64BE(buffer, 0xffeeddccbbaa9988));
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::WriteU64BE(buffer, 0xffeeddccbbaa9988));
    }
}

TEST_CASE("TryRead64LE/Read64LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    int64_t test;
    REQUIRE(LexIO::TryRead64LE(test, buffer) == true);
    REQUIRE(test == -4822678189205112);
    REQUIRE(LexIO::TryRead64LE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read64LE(buffer) == -4822678189205112);
    REQUIRE_THROWS(LexIO::Read64LE(buffer));
}

TEST_CASE("TryWrite64LE/Write64LE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint64_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint64_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWrite64LE(buffer, -4822678189205112) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(LexIO::TryWrite64LE(buffer, -4822678189205112) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(uint64_t)> buffer;
        const LexIO::ArrayStream<sizeof(uint64_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::Write64LE(buffer, -4822678189205112));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE_THROWS(LexIO::Write64LE(buffer, -4822678189205112));
    }
}

TEST_CASE("TryRead64BE/Read64BE")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});

    int64_t test;
    REQUIRE(LexIO::TryRead64BE(test, buffer) == true);
    REQUIRE(test == -4822678189205112);
    REQUIRE(LexIO::TryRead64BE(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read64BE(buffer) == -4822678189205112);
    REQUIRE_THROWS(LexIO::Read64BE(buffer));
}

TEST_CASE("TryWrite64BE/Write64BE")
{
    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int64_t)> buffer;
        const LexIO::ArrayStream<sizeof(int64_t)> &cbuffer = buffer;

        REQUIRE(LexIO::TryWrite64BE(buffer, -4822678189205112) == true);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(LexIO::TryWrite64BE(buffer, -4822678189205112) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<sizeof(int64_t)> buffer;
        const LexIO::ArrayStream<sizeof(int64_t)> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::Write64BE(buffer, -4822678189205112));
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0xee);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcc);
        REQUIRE(cbuffer.Container()[i++] == 0xbb);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0x99);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE_THROWS(LexIO::Write64BE(buffer, -4822678189205112));
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

TEST_CASE("ReadUVarint32")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});
    REQUIRE(LexIO::ReadUVarint32(buffer) == 0xbbaa9988);
}

TEST_CASE("WriteUVarint32")
{
    LexIO::VectorStream buffer;
    LexIO::WriteUVarint32(buffer, 0xbbaa9988);
    LexIO::Rewind(buffer);

    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0xb3);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0x0b);
    REQUIRE_THROWS(LexIO::ReadU8(buffer));
}

TEST_CASE("ReadVarint32")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});
    REQUIRE(LexIO::ReadVarint32(buffer) == BBAA9988i32());
}

TEST_CASE("WriteVarint32")
{
    LexIO::VectorStream buffer;
    LexIO::WriteVarint32(buffer, BBAA9988i32());
    LexIO::Rewind(buffer);

    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x88);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xb3);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xaa);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xdd);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x0b);
    REQUIRE_THROWS(LexIO::ReadU8(buffer));
}

TEST_CASE("ReadSVarint32")
{
    LexIO::VectorStream buffer({0xef, 0x99, 0xab, 0xc5, 0x08});
    REQUIRE(LexIO::ReadSVarint32(buffer) == BBAA9988i32());
}

TEST_CASE("WriteSVarint32")
{
    LexIO::VectorStream buffer;
    LexIO::WriteSVarint32(buffer, BBAA9988i32());
    LexIO::Rewind(buffer);

    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xef);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x99);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xab);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xc5);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x08);
    REQUIRE_THROWS(LexIO::ReadU8(buffer));
}

TEST_CASE("ReadUVarint64")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});
    REQUIRE(LexIO::ReadUVarint64(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("WriteUVarint64")
{
    LexIO::VectorStream buffer;
    LexIO::WriteUVarint64(buffer, 0xffeeddccbbaa9988);
    LexIO::Rewind(buffer);

    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0xb3);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0xcb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xb9);
    REQUIRE(LexIO::ReadU8(buffer) == 0xb7);
    REQUIRE(LexIO::ReadU8(buffer) == 0xf7);
    REQUIRE(LexIO::ReadU8(buffer) == 0xff);
    REQUIRE(LexIO::ReadU8(buffer) == 0x01);
    REQUIRE_THROWS(LexIO::ReadU8(buffer));
}

TEST_CASE("ReadVarint64")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});
    REQUIRE(LexIO::ReadVarint64(buffer) == FFEEDDCCBBAA9988i64());
}

TEST_CASE("WriteVarint64")
{
    LexIO::VectorStream buffer;
    LexIO::WriteVarint64(buffer, FFEEDDCCBBAA9988i64());
    LexIO::Rewind(buffer);

    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x88);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xb3);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xaa);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xdd);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xcb);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xb9);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xb7);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xf7);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xff);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x01);
    REQUIRE_THROWS(LexIO::ReadU8(buffer));
}

TEST_CASE("ReadSVarint64")
{
    LexIO::VectorStream buffer({0xef, 0x99, 0xab, 0xc5, 0xe8, 0x8c, 0x91, 0x11});
    REQUIRE(LexIO::ReadSVarint64(buffer) == FFEEDDCCBBAA9988i64());
}

TEST_CASE("WriteSVarint64")
{
    LexIO::VectorStream buffer;
    LexIO::WriteSVarint64(buffer, FFEEDDCCBBAA9988i64());
    LexIO::Rewind(buffer);

    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xef);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x99);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xab);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xc5);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0xe8);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x8c);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x91);
    REQUIRE(int(LexIO::ReadU8(buffer)) == 0x11);
    REQUIRE_THROWS(LexIO::ReadU8(buffer));
}

template <typename T, std::size_t N>
constexpr std::size_t CountOf(T const (&)[N]) noexcept
{
    return N;
}

static LexIO::VectorStream GetBuffer()
{
    const uint8_t text[] = "The quick brown fox\njumps over the lazy dog.\n";
    LexIO::ConstByteSpanT textSpan{&text[0], CountOf(text) - 1};

    LexIO::VectorStream rvo;
    rvo.LexWrite(textSpan);
    rvo.LexSeek(LexIO::SeekPos(0, LexIO::seek::start));
    return rvo;
}

TEST_CASE("ReadBytes")
{
    LexIO::VectorStream buffer = GetBuffer();

    std::array<uint8_t, 10> data;
    data.fill(0x00);

    REQUIRE_NOTHROW(LexIO::ReadBytes(data.begin(), data.end() - 1, buffer));
    char *dataChar = LexIO::Detail::BitCast<char *>(&(*data.begin()));
    REQUIRE(strlen(dataChar) == 9);
    REQUIRE(!strcmp(dataChar, "The quick"));
}

TEST_CASE("WriteBytes")
{
    LexIO::VectorStream buffer;
    std::string src{"The quick"};
    std::array<uint8_t, 10> data;
    data.fill(0x00);
    std::memcpy(data.data(), src.data(), src.size());

    REQUIRE_NOTHROW(LexIO::WriteBytes(buffer, data.begin(), data.end()));

    std::array<uint8_t, 10> check;
    check.fill(0x00);
    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read(check, buffer) == 10);
    REQUIRE(memcmp(check.data(), data.data(), check.size()) == 0);
}

TEST_CASE("ReadData")
{
    LexIO::VectorStream buffer = GetBuffer();

    void *data = calloc(10, 1);
    REQUIRE_NOTHROW(LexIO::ReadData(data, 9, buffer));
    char *dataChar = static_cast<char *>(data);
    REQUIRE(strlen(dataChar) == 9);
    REQUIRE(!strcmp(dataChar, "The quick"));
    free(data);
}

TEST_CASE("WriteData")
{
    LexIO::VectorStream buffer;
    std::string data{"The quick"};
    const void *dataVoid = static_cast<const void *>(data.data());
    std::array<uint8_t, 10> check;
    memset(check.data(), 0x00, check.size());

    REQUIRE_NOTHROW(LexIO::WriteData(buffer, dataVoid, data.length()));

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read(check, buffer) == 9);

    char *checkChar = reinterpret_cast<char *>(check.data());
    REQUIRE(strcmp(checkChar, data.c_str()) == 0);
}

TEST_CASE("ReadString")
{
    LexIO::VectorStream buffer = GetBuffer();

    std::string data;
    data.resize(9);
    REQUIRE_NOTHROW(LexIO::ReadString(data.begin(), data.end(), buffer));
    REQUIRE(data == "The quick");
}

TEST_CASE("WriteString")
{
    LexIO::VectorStream buffer;
    std::string data{"The quick"};
    std::array<uint8_t, 10> check;
    memset(check.data(), 0x00, check.size());

    REQUIRE_NOTHROW(LexIO::WriteString(buffer, data.cbegin(), data.cend()));

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read(check, buffer) == 9);

    char *checkChar = reinterpret_cast<char *>(check.data());
    REQUIRE(strcmp(checkChar, data.c_str()) == 0);
}
