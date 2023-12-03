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
