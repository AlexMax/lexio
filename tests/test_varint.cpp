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
