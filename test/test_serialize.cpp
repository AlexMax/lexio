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

#include <catch2/catch_all.hpp>

//------------------------------------------------------------------------------

TEST_CASE("ReadU8", "[serialize]")
{
    LexIO::VectorStream buffer = {0x88};
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("WriteU8", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU8(buffer, 0x88);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadU16LE", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0x99});
    REQUIRE(LexIO::ReadU16LE(buffer) == 0x9988);
}

TEST_CASE("WriteU16LE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU16LE(buffer, 0x9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
}

TEST_CASE("ReadU16BE", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0x99});
    REQUIRE(LexIO::ReadU16BE(buffer) == 0x8899);
}

TEST_CASE("WriteU16BE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU16BE(buffer, 0x9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Read16LE", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0x99});
    REQUIRE(LexIO::Read16LE(buffer) == -26232);
}

TEST_CASE("Write16LE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::Write16LE(buffer, -26232);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
}

TEST_CASE("Read16BE", "[serialize]")
{
    LexIO::VectorStream buffer({0x99, 0x88});
    REQUIRE(LexIO::Read16BE(buffer) == -26232);
}

TEST_CASE("Write16BE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::Write16BE(buffer, -26232);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadU32LE", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});
    REQUIRE(LexIO::ReadU32LE(buffer) == 0xbbaa9988);
}

TEST_CASE("WriteU32LE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU32LE(buffer, 0xbbaa9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
}

TEST_CASE("ReadU32BE", "[serialize]")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::ReadU32BE(buffer) == 0xbbaa9988);
}

TEST_CASE("WriteU32BE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU32BE(buffer, 0xbbaa9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Read32LE", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});
    REQUIRE(LexIO::Read32LE(buffer) == -1146447480);
}

TEST_CASE("Write32LE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::Write32LE(buffer, -1146447480);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
}

TEST_CASE("Read32BE", "[serialize]")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::Read32BE(buffer) == -1146447480);
}

TEST_CASE("Write32BE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::Write32BE(buffer, -1146447480);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadU64LE", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    REQUIRE(LexIO::ReadU64LE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("WriteU64LE", "[serialize]")
{
    LexIO::VectorStream buffer;
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

TEST_CASE("ReadU64BE", "[serialize]")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::ReadU64BE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("WriteU64BE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU64BE(buffer, 0xffeeddccbbaa9988);
    LexIO::Seek(buffer, LexIO::WhenceStart(0));

    REQUIRE(LexIO::ReadU8(buffer) == 0xff);
    REQUIRE(LexIO::ReadU8(buffer) == 0xee);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0xcc);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Read64LE", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    REQUIRE(LexIO::Read64LE(buffer) == -4822678189205112);
}

TEST_CASE("Write64LE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::Write64LE(buffer, -4822678189205112);
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

TEST_CASE("Read64BE", "[serialize]")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::Read64BE(buffer) == -4822678189205112);
}

TEST_CASE("Write64BE", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::Write64BE(buffer, -4822678189205112);
    LexIO::Seek(buffer, LexIO::WhenceStart(0));

    REQUIRE(LexIO::ReadU8(buffer) == 0xff);
    REQUIRE(LexIO::ReadU8(buffer) == 0xee);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0xcc);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadUVarint64", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});
    REQUIRE(LexIO::ReadUVarint64(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("WriteUVarint64", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteUVarint64(buffer, 0xffeeddccbbaa9988);
    LexIO::Seek(buffer, LexIO::WhenceStart(0));

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

TEST_CASE("ReadVarint64", "[serialize]")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});
    REQUIRE(LexIO::ReadVarint64(buffer) == -4822678189205112);
}

TEST_CASE("WriteVarint64", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteVarint64(buffer, -4822678189205112);
    LexIO::Seek(buffer, LexIO::WhenceStart(0));

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

TEST_CASE("ReadSVarint64", "[serialize]")
{
    LexIO::VectorStream buffer({0xef, 0x99, 0xab, 0xc5, 0xe8, 0x8c, 0x91, 0x11});
    REQUIRE(LexIO::ReadSVarint64(buffer) == -4822678189205112);
}

TEST_CASE("WriteSVarint64", "[serialize]")
{
    LexIO::VectorStream buffer;
    LexIO::WriteSVarint64(buffer, -4822678189205112);
    LexIO::Seek(buffer, LexIO::WhenceStart(0));

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
