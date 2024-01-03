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

#include "lexio/serialize/varint.hpp"

#include "./test.h"

//******************************************************************************

TEST(Varint, TryReadUVarint32_ReadUVarint32)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});

    uint32_t test;
    EXPECT_EQ(LexIO::TryReadUVarint32(test, buffer), true);
    EXPECT_EQ(test, 0xbbaa9988);
    EXPECT_EQ(LexIO::TryReadUVarint32(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadUVarint32(buffer), 0xbbaa9988);
    EXPECT_ANY_THROW(LexIO::ReadUVarint32(buffer));
}

TEST(Varint, TryReadUVarint32_ReadUVarint32_TooBig)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x8b, 0x00});

    uint32_t test;
    EXPECT_EQ(LexIO::TryReadUVarint32(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_ANY_THROW(LexIO::ReadUVarint32(buffer));
}

TEST(Varint, TryWriteUVarint32_WriteUVarint32)
{
    constexpr size_t COUNT = LexIO::UVarint32Bytes(0xbbaa9988);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_EQ(LexIO::TryWriteUVarint32(buffer, 0xbbaa9988), true);
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0x0b);
        EXPECT_EQ(LexIO::TryWriteUVarint32(buffer, 0xbbaa9988), false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_NO_THROW(LexIO::WriteUVarint32(buffer, 0xbbaa9988));
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0x0b);
        EXPECT_ANY_THROW(LexIO::WriteUVarint32(buffer, 0xbbaa9988));
    }
}

//******************************************************************************

TEST(Varint, TryReadVarint32_ReadVarint32)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});

    int32_t test;
    EXPECT_EQ(LexIO::TryReadVarint32(test, buffer), true);
    EXPECT_EQ(test, -1146447480);
    EXPECT_EQ(LexIO::TryReadVarint32(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadVarint32(buffer), -1146447480);
    EXPECT_ANY_THROW(LexIO::ReadVarint32(buffer));
}

TEST(Varint, TryWriteVarint32_WriteVarint32)
{
    constexpr size_t COUNT = LexIO::Varint32Bytes(-1146447480);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_EQ(LexIO::TryWriteVarint32(buffer, -1146447480), true);
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0x0b);
        EXPECT_EQ(LexIO::TryWriteVarint32(buffer, -1146447480), false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_NO_THROW(LexIO::WriteVarint32(buffer, -1146447480));
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0x0b);
        EXPECT_ANY_THROW(LexIO::WriteVarint32(buffer, -1146447480));
    }
}

//******************************************************************************

TEST(Varint, TryReadSVarint32_ReadSVarint32)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0x0b});

    int32_t test;
    EXPECT_EQ(LexIO::TryReadSVarint32(test, buffer), true);
    EXPECT_EQ(test, 1574259908);
    EXPECT_EQ(LexIO::TryReadSVarint32(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadSVarint32(buffer), 1574259908);
    EXPECT_ANY_THROW(LexIO::ReadSVarint32(buffer));
}

TEST(Varint, TryWriteSVarint32_WriteSVarint32)
{
    constexpr size_t COUNT = LexIO::SVarint32Bytes(1574259908);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_EQ(LexIO::TryWriteSVarint32(buffer, 1574259908), true);
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0x0b);
        EXPECT_EQ(LexIO::TryWriteSVarint32(buffer, 1574259908), false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_NO_THROW(LexIO::WriteSVarint32(buffer, 1574259908));
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0x0b);
        EXPECT_ANY_THROW(LexIO::WriteSVarint32(buffer, 1574259908));
    }
}

//******************************************************************************

TEST(Varint, TryReadUVarint64_ReadUVarint64)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});

    uint64_t test;
    EXPECT_EQ(LexIO::TryReadUVarint64(test, buffer), true);
    EXPECT_EQ(test, 0xffeeddccbbaa9988);
    EXPECT_EQ(LexIO::TryReadUVarint64(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadUVarint64(buffer), 0xffeeddccbbaa9988);
    EXPECT_ANY_THROW(LexIO::ReadUVarint64(buffer));
}

TEST(Varint, TryReadUVarint64_ReadUVarint64_TooBig)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x81, 0x00});

    uint64_t test;
    EXPECT_EQ(LexIO::TryReadUVarint64(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_ANY_THROW(LexIO::ReadUVarint64(buffer));
}

TEST(Varint, TryWriteUVarint64_WriteUVarint64)
{
    constexpr size_t COUNT = LexIO::UVarint64Bytes(0xffeeddccbbaa9988);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_EQ(LexIO::TryWriteUVarint64(buffer, 0xffeeddccbbaa9988), true);
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0xcb);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb9);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xf7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xff);
        EXPECT_EQ(cbuffer.Container()[i++], 0x01);
        EXPECT_EQ(LexIO::TryWriteUVarint64(buffer, 0xffeeddccbbaa9988), false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_NO_THROW(LexIO::WriteUVarint64(buffer, 0xffeeddccbbaa9988));
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0xcb);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb9);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xf7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xff);
        EXPECT_EQ(cbuffer.Container()[i++], 0x01);
        EXPECT_ANY_THROW(LexIO::WriteUVarint64(buffer, 0xffeeddccbbaa9988));
    }
}

//******************************************************************************

TEST(Varint, TryReadVarint64_ReadVarint64)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});

    int64_t test;
    EXPECT_EQ(LexIO::TryReadVarint64(test, buffer), true);
    EXPECT_EQ(test, -4822678189205112);
    EXPECT_EQ(LexIO::TryReadVarint64(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadVarint64(buffer), -4822678189205112);
    EXPECT_ANY_THROW(LexIO::ReadVarint64(buffer));
}

TEST(Varint, TryWriteVarint64_WriteVarint64)
{
    constexpr size_t COUNT = LexIO::Varint64Bytes(-4822678189205112);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_EQ(LexIO::TryWriteVarint64(buffer, -4822678189205112), true);
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0xcb);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb9);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xf7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xff);
        EXPECT_EQ(cbuffer.Container()[i++], 0x01);
        EXPECT_EQ(LexIO::TryWriteVarint64(buffer, -4822678189205112), false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_NO_THROW(LexIO::WriteVarint64(buffer, -4822678189205112));
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0xcb);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb9);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xf7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xff);
        EXPECT_EQ(cbuffer.Container()[i++], 0x01);
        EXPECT_ANY_THROW(LexIO::WriteVarint64(buffer, -4822678189205112));
    }
}

//******************************************************************************

TEST(Varint, TryReadSVarint64_ReadSVarint64)
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});

    int64_t test;
    EXPECT_EQ(LexIO::TryReadSVarint64(test, buffer), true);
    EXPECT_EQ(test, 9220960697760173252);
    EXPECT_EQ(LexIO::TryReadSVarint64(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadSVarint64(buffer), 9220960697760173252);
    EXPECT_ANY_THROW(LexIO::ReadSVarint64(buffer));
}

//******************************************************************************

TEST(Varint, TryWriteSVarint64_WriteSVarint64)
{
    constexpr size_t COUNT = LexIO::SVarint64Bytes(9220960697760173252);

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_EQ(LexIO::TryWriteSVarint64(buffer, 9220960697760173252), true);
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0xcb);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb9);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xf7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xff);
        EXPECT_EQ(cbuffer.Container()[i++], 0x01);
        EXPECT_EQ(LexIO::TryWriteSVarint64(buffer, 9220960697760173252), false);
    }

    {
        size_t i = 0;
        LexIO::ArrayStream<COUNT> buffer;
        const LexIO::ArrayStream<COUNT> &cbuffer = buffer;

        EXPECT_NO_THROW(LexIO::WriteSVarint64(buffer, 9220960697760173252));
        EXPECT_EQ(cbuffer.Container()[i++], 0x88);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb3);
        EXPECT_EQ(cbuffer.Container()[i++], 0xaa);
        EXPECT_EQ(cbuffer.Container()[i++], 0xdd);
        EXPECT_EQ(cbuffer.Container()[i++], 0xcb);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb9);
        EXPECT_EQ(cbuffer.Container()[i++], 0xb7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xf7);
        EXPECT_EQ(cbuffer.Container()[i++], 0xff);
        EXPECT_EQ(cbuffer.Container()[i++], 0x01);
        EXPECT_ANY_THROW(LexIO::WriteSVarint64(buffer, 9220960697760173252));
    }
}

TEST(Varint, UVarint32Bytes)
{
    EXPECT_EQ(1, LexIO::UVarint32Bytes(0));
    EXPECT_EQ(1, LexIO::UVarint32Bytes(0x7f));
    EXPECT_EQ(2, LexIO::UVarint32Bytes(0x80));
    EXPECT_EQ(2, LexIO::UVarint32Bytes(0x3fff));
    EXPECT_EQ(3, LexIO::UVarint32Bytes(0x4000));
    EXPECT_EQ(3, LexIO::UVarint32Bytes(0x1fffff));
    EXPECT_EQ(4, LexIO::UVarint32Bytes(0x200000));
    EXPECT_EQ(4, LexIO::UVarint32Bytes(0xfffffff));
    EXPECT_EQ(5, LexIO::UVarint32Bytes(0x10000000));
    EXPECT_EQ(5, LexIO::UVarint32Bytes(0xffffffff));
}

TEST(Varint, Varint32Bytes)
{
    EXPECT_EQ(1, LexIO::Varint32Bytes(0));
    EXPECT_EQ(1, LexIO::Varint32Bytes(0x7f));
    EXPECT_EQ(2, LexIO::Varint32Bytes(0x80));
    EXPECT_EQ(2, LexIO::Varint32Bytes(0x3fff));
    EXPECT_EQ(3, LexIO::Varint32Bytes(0x4000));
    EXPECT_EQ(3, LexIO::Varint32Bytes(0x1fffff));
    EXPECT_EQ(4, LexIO::Varint32Bytes(0x200000));
    EXPECT_EQ(4, LexIO::Varint32Bytes(0xfffffff));
    EXPECT_EQ(5, LexIO::Varint32Bytes(0x10000000));
    EXPECT_EQ(5, LexIO::Varint32Bytes(0x7fffffff));
    EXPECT_EQ(5, LexIO::Varint32Bytes(0 - 0x1));
    EXPECT_EQ(5, LexIO::Varint32Bytes(0 - 0x80000000));
}

TEST(Varint, SVarint32Bytes)
{
    EXPECT_EQ(1, LexIO::SVarint32Bytes(0));
    EXPECT_EQ(1, LexIO::SVarint32Bytes(1));
    EXPECT_EQ(1, LexIO::SVarint32Bytes(-1));
    EXPECT_EQ(1, LexIO::SVarint32Bytes(0 - 0x40));
    EXPECT_EQ(2, LexIO::SVarint32Bytes(0x40));
    EXPECT_EQ(2, LexIO::SVarint32Bytes(0 - 0x2000));
    EXPECT_EQ(3, LexIO::SVarint32Bytes(0x2000));
    EXPECT_EQ(3, LexIO::SVarint32Bytes(0 - 0x100000));
    EXPECT_EQ(4, LexIO::SVarint32Bytes(0x100000));
    EXPECT_EQ(4, LexIO::SVarint32Bytes(0 - 0x8000000));
    EXPECT_EQ(5, LexIO::SVarint32Bytes(0x8000000));
    EXPECT_EQ(5, LexIO::SVarint32Bytes(0 - 0x80000000));
    EXPECT_EQ(5, LexIO::SVarint32Bytes(0x7fffffff));
}

TEST(Varint, UVarint64Bytes)
{
    EXPECT_EQ(1, LexIO::UVarint64Bytes(0));
    EXPECT_EQ(1, LexIO::UVarint64Bytes(0x7f));
    EXPECT_EQ(2, LexIO::UVarint64Bytes(0x80));
    EXPECT_EQ(2, LexIO::UVarint64Bytes(0x3fff));
    EXPECT_EQ(3, LexIO::UVarint64Bytes(0x4000));
    EXPECT_EQ(3, LexIO::UVarint64Bytes(0x1fffff));
    EXPECT_EQ(4, LexIO::UVarint64Bytes(0x200000));
    EXPECT_EQ(4, LexIO::UVarint64Bytes(0xfffffff));
    EXPECT_EQ(5, LexIO::UVarint64Bytes(0x10000000));
    EXPECT_EQ(5, LexIO::UVarint64Bytes(0x7ffffffff));
    EXPECT_EQ(6, LexIO::UVarint64Bytes(0x800000000));
    EXPECT_EQ(6, LexIO::UVarint64Bytes(0x3ffffffffff));
    EXPECT_EQ(7, LexIO::UVarint64Bytes(0x40000000000));
    EXPECT_EQ(7, LexIO::UVarint64Bytes(0x1ffffffffffff));
    EXPECT_EQ(8, LexIO::UVarint64Bytes(0x2000000000000));
    EXPECT_EQ(8, LexIO::UVarint64Bytes(0xffffffffffffff));
    EXPECT_EQ(9, LexIO::UVarint64Bytes(0x100000000000000));
    EXPECT_EQ(9, LexIO::UVarint64Bytes(0x7fffffffffffffff));
    EXPECT_EQ(10, LexIO::UVarint64Bytes(0x8000000000000000));
    EXPECT_EQ(10, LexIO::UVarint64Bytes(0xffffffffffffffff));
}

TEST(Varint, Varint64Bytes)
{
    EXPECT_EQ(1, LexIO::Varint64Bytes(0));
    EXPECT_EQ(1, LexIO::Varint64Bytes(0x7f));
    EXPECT_EQ(2, LexIO::Varint64Bytes(0x80));
    EXPECT_EQ(2, LexIO::Varint64Bytes(0x3fff));
    EXPECT_EQ(3, LexIO::Varint64Bytes(0x4000));
    EXPECT_EQ(3, LexIO::Varint64Bytes(0x1fffff));
    EXPECT_EQ(4, LexIO::Varint64Bytes(0x200000));
    EXPECT_EQ(4, LexIO::Varint64Bytes(0xfffffff));
    EXPECT_EQ(5, LexIO::Varint64Bytes(0x10000000));
    EXPECT_EQ(5, LexIO::Varint64Bytes(0x7ffffffff));
    EXPECT_EQ(6, LexIO::Varint64Bytes(0x800000000));
    EXPECT_EQ(6, LexIO::Varint64Bytes(0x3ffffffffff));
    EXPECT_EQ(7, LexIO::Varint64Bytes(0x40000000000));
    EXPECT_EQ(7, LexIO::Varint64Bytes(0x1ffffffffffff));
    EXPECT_EQ(8, LexIO::Varint64Bytes(0x2000000000000));
    EXPECT_EQ(8, LexIO::Varint64Bytes(0xffffffffffffff));
    EXPECT_EQ(9, LexIO::Varint64Bytes(0x100000000000000));
    EXPECT_EQ(9, LexIO::Varint64Bytes(0x7fffffffffffffff));
    EXPECT_EQ(10, LexIO::Varint64Bytes(0 - 0x1));
    EXPECT_EQ(10, LexIO::Varint64Bytes(0 - 0x8000000000000000));
}

TEST(Varint, SVarint64Bytes)
{
    EXPECT_EQ(1, LexIO::SVarint64Bytes(0));
    EXPECT_EQ(1, LexIO::SVarint64Bytes(1));
    EXPECT_EQ(1, LexIO::SVarint64Bytes(-1));
    EXPECT_EQ(1, LexIO::SVarint64Bytes(0 - 0x40));
    EXPECT_EQ(2, LexIO::SVarint64Bytes(0x40));
    EXPECT_EQ(2, LexIO::SVarint64Bytes(0 - 0x2000));
    EXPECT_EQ(3, LexIO::SVarint64Bytes(0x2000));
    EXPECT_EQ(3, LexIO::SVarint64Bytes(0 - 0x100000));
    EXPECT_EQ(4, LexIO::SVarint64Bytes(0x100000));
    EXPECT_EQ(4, LexIO::SVarint64Bytes(0 - 0x8000000));
    EXPECT_EQ(5, LexIO::SVarint64Bytes(0x8000000));
    EXPECT_EQ(5, LexIO::SVarint64Bytes(0 - 0x400000000));
    EXPECT_EQ(6, LexIO::SVarint64Bytes(0x400000000));
    EXPECT_EQ(6, LexIO::SVarint64Bytes(0 - 0x20000000000));
    EXPECT_EQ(7, LexIO::SVarint64Bytes(0x20000000000));
    EXPECT_EQ(7, LexIO::SVarint64Bytes(0 - 0x1000000000000));
    EXPECT_EQ(8, LexIO::SVarint64Bytes(0x1000000000000));
    EXPECT_EQ(8, LexIO::SVarint64Bytes(0 - 0x80000000000000));
    EXPECT_EQ(9, LexIO::SVarint64Bytes(0x80000000000000));
    EXPECT_EQ(9, LexIO::SVarint64Bytes(0 - 0x4000000000000000));
    EXPECT_EQ(10, LexIO::SVarint64Bytes(0x4000000000000000));
    EXPECT_EQ(10, LexIO::SVarint64Bytes(0 - 0x8000000000000000));
    EXPECT_EQ(10, LexIO::SVarint64Bytes(0x7fffffffffffffff));
}
