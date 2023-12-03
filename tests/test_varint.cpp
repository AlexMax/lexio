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

TEST_CASE("TryReadUVarint32/ReadUVarint32")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});

    uint32_t test;
    REQUIRE(LexIO::TryReadUVarint32(test, buffer) == true);
    REQUIRE(test == 0xbbaa9988);
    REQUIRE(LexIO::TryReadUVarint32(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadUVarint32(buffer) == 0xbbaa9988);
    REQUIRE_THROWS(LexIO::ReadUVarint32(buffer));
}

TEST_CASE("TryWriteUVarint32/WriteUVarint32")
{
    constexpr size_t COUNT = LexIO::UVarint32Bytes(0xbbaa9988);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteUVarint32(buffer, 0xbbaa9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0x0b);
        REQUIRE(LexIO::TryWriteUVarint32(buffer, 0xbbaa9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteUVarint32(buffer, 0xbbaa9988));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0x0b);
        REQUIRE_THROWS(LexIO::WriteUVarint32(buffer, 0xbbaa9988));
    }
}

//******************************************************************************

TEST_CASE("TryReadVarint32/ReadVarint32")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});

    int32_t test;
    REQUIRE(LexIO::TryReadVarint32(test, buffer) == true);
    REQUIRE(test == -1146447480);
    REQUIRE(LexIO::TryReadVarint32(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadVarint32(buffer) == -1146447480);
    REQUIRE_THROWS(LexIO::ReadVarint32(buffer));
}

TEST_CASE("TryWriteVarint32/WriteVarint32")
{
    constexpr size_t COUNT = LexIO::Varint32Bytes(-1146447480);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteVarint32(buffer, -1146447480) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0x0b);
        REQUIRE(LexIO::TryWriteVarint32(buffer, -1146447480) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteVarint32(buffer, -1146447480));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0x0b);
        REQUIRE_THROWS(LexIO::WriteVarint32(buffer, -1146447480));
    }
}

//******************************************************************************

TEST_CASE("TryReadSVarint32/ReadSVarint32")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});

    int32_t test;
    REQUIRE(LexIO::TryReadSVarint32(test, buffer) == true);
    REQUIRE(test == 1574259908);
    REQUIRE(LexIO::TryReadSVarint32(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadSVarint32(buffer) == 1574259908);
    REQUIRE_THROWS(LexIO::ReadSVarint32(buffer));
}

TEST_CASE("TryWriteSVarint32/WriteSVarint32")
{
    constexpr size_t COUNT = LexIO::SVarint32Bytes(1574259908);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteSVarint32(buffer, 1574259908) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0x0b);
        REQUIRE(LexIO::TryWriteSVarint32(buffer, 1574259908) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteSVarint32(buffer, 1574259908));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0x0b);
        REQUIRE_THROWS(LexIO::WriteSVarint32(buffer, 1574259908));
    }
}

//******************************************************************************

TEST_CASE("TryReadUVarint64/ReadUVarint64")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});

    uint64_t test;
    REQUIRE(LexIO::TryReadUVarint64(test, buffer) == true);
    REQUIRE(test == 0xffeeddccbbaa9988);
    REQUIRE(LexIO::TryReadUVarint64(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadUVarint64(buffer) == 0xffeeddccbbaa9988);
    REQUIRE_THROWS(LexIO::ReadUVarint64(buffer));
}

TEST_CASE("TryWriteUVarint64/WriteUVarint64")
{
    constexpr size_t COUNT = LexIO::UVarint64Bytes(0xffeeddccbbaa9988);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteUVarint64(buffer, 0xffeeddccbbaa9988) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcb);
        REQUIRE(cbuffer.Container()[i++] == 0xb9);
        REQUIRE(cbuffer.Container()[i++] == 0xb7);
        REQUIRE(cbuffer.Container()[i++] == 0xf7);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0x01);
        REQUIRE(LexIO::TryWriteUVarint64(buffer, 0xffeeddccbbaa9988) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteUVarint64(buffer, 0xffeeddccbbaa9988));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcb);
        REQUIRE(cbuffer.Container()[i++] == 0xb9);
        REQUIRE(cbuffer.Container()[i++] == 0xb7);
        REQUIRE(cbuffer.Container()[i++] == 0xf7);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0x01);
        REQUIRE_THROWS(LexIO::WriteUVarint64(buffer, 0xffeeddccbbaa9988));
    }
}

//******************************************************************************

TEST_CASE("TryReadVarint64/ReadVarint64")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});

    int64_t test;
    REQUIRE(LexIO::TryReadVarint64(test, buffer) == true);
    REQUIRE(test == -4822678189205112);
    REQUIRE(LexIO::TryReadVarint64(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadVarint64(buffer) == -4822678189205112);
    REQUIRE_THROWS(LexIO::ReadVarint64(buffer));
}

TEST_CASE("TryWriteVarint64/WriteVarint64")
{
    constexpr size_t COUNT = LexIO::Varint64Bytes(-4822678189205112);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteVarint64(buffer, -4822678189205112) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcb);
        REQUIRE(cbuffer.Container()[i++] == 0xb9);
        REQUIRE(cbuffer.Container()[i++] == 0xb7);
        REQUIRE(cbuffer.Container()[i++] == 0xf7);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0x01);
        REQUIRE(LexIO::TryWriteVarint64(buffer, -4822678189205112) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteVarint64(buffer, -4822678189205112));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcb);
        REQUIRE(cbuffer.Container()[i++] == 0xb9);
        REQUIRE(cbuffer.Container()[i++] == 0xb7);
        REQUIRE(cbuffer.Container()[i++] == 0xf7);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0x01);
        REQUIRE_THROWS(LexIO::WriteVarint64(buffer, -4822678189205112));
    }
}

//******************************************************************************

TEST_CASE("TryReadSVarint64/ReadSVarint64")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});

    int64_t test;
    REQUIRE(LexIO::TryReadSVarint64(test, buffer) == true);
    REQUIRE(test == 9220960697760173252);
    REQUIRE(LexIO::TryReadSVarint64(test, buffer) == false);

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::ReadSVarint64(buffer) == 9220960697760173252);
    REQUIRE_THROWS(LexIO::ReadSVarint64(buffer));
}

//******************************************************************************

TEST_CASE("TryWriteSVarint64/WriteSVarint64")
{
    constexpr size_t COUNT = LexIO::SVarint64Bytes(9220960697760173252);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE(LexIO::TryWriteSVarint64(buffer, 9220960697760173252) == true);
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcb);
        REQUIRE(cbuffer.Container()[i++] == 0xb9);
        REQUIRE(cbuffer.Container()[i++] == 0xb7);
        REQUIRE(cbuffer.Container()[i++] == 0xf7);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0x01);
        REQUIRE(LexIO::TryWriteSVarint64(buffer, 9220960697760173252) == false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        REQUIRE_NOTHROW(LexIO::WriteSVarint64(buffer, 9220960697760173252));
        REQUIRE(cbuffer.Container()[i++] == 0x88);
        REQUIRE(cbuffer.Container()[i++] == 0xb3);
        REQUIRE(cbuffer.Container()[i++] == 0xaa);
        REQUIRE(cbuffer.Container()[i++] == 0xdd);
        REQUIRE(cbuffer.Container()[i++] == 0xcb);
        REQUIRE(cbuffer.Container()[i++] == 0xb9);
        REQUIRE(cbuffer.Container()[i++] == 0xb7);
        REQUIRE(cbuffer.Container()[i++] == 0xf7);
        REQUIRE(cbuffer.Container()[i++] == 0xff);
        REQUIRE(cbuffer.Container()[i++] == 0x01);
        REQUIRE_THROWS(LexIO::WriteSVarint64(buffer, 9220960697760173252));
    }
}
