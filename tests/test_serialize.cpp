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
#include "doctest.h"

//******************************************************************************

TEST_SUITE_BEGIN("file");

TEST_CASE("ReadU8")
{
    LexIO::VectorStream buffer = {0x88};
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("WriteU8")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU8(buffer, 0x88);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadU16LE")
{
    LexIO::VectorStream buffer({0x88, 0x99});
    REQUIRE(LexIO::ReadU16LE(buffer) == 0x9988);
}

TEST_CASE("WriteU16LE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU16LE(buffer, 0x9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
}

TEST_CASE("ReadU16BE")
{
    LexIO::VectorStream buffer({0x88, 0x99});
    REQUIRE(LexIO::ReadU16BE(buffer) == 0x8899);
}

TEST_CASE("WriteU16BE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU16BE(buffer, 0x9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Read16LE")
{
    LexIO::VectorStream buffer({0x88, 0x99});
    REQUIRE(LexIO::Read16LE(buffer) == -26232);
}

TEST_CASE("Write16LE")
{
    LexIO::VectorStream buffer;
    LexIO::Write16LE(buffer, -26232);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
}

TEST_CASE("Read16BE")
{
    LexIO::VectorStream buffer({0x99, 0x88});
    REQUIRE(LexIO::Read16BE(buffer) == -26232);
}

TEST_CASE("Write16BE")
{
    LexIO::VectorStream buffer;
    LexIO::Write16BE(buffer, -26232);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadU32LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});
    REQUIRE(LexIO::ReadU32LE(buffer) == 0xbbaa9988);
}

TEST_CASE("WriteU32LE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU32LE(buffer, 0xbbaa9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
}

TEST_CASE("ReadU32BE")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::ReadU32BE(buffer) == 0xbbaa9988);
}

TEST_CASE("WriteU32BE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteU32BE(buffer, 0xbbaa9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Read32LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});
    REQUIRE(LexIO::Read32LE(buffer) == -1146447480);
}

TEST_CASE("Write32LE")
{
    LexIO::VectorStream buffer;
    LexIO::Write32LE(buffer, -1146447480);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
}

TEST_CASE("Read32BE")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::Read32BE(buffer) == -1146447480);
}

TEST_CASE("Write32BE")
{
    LexIO::VectorStream buffer;
    LexIO::Write32BE(buffer, -1146447480);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadFloatLE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});
    const float check = LexIO::ReadFloatLE(buffer);

    int exp;
    const float x = frexpf(check, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
}

TEST_CASE("WriteFloatLE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteFloatLE(buffer, ldexpf(-0.666405201f, -7));

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
}

TEST_CASE("ReadFloatBE")
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});
    const float check = LexIO::ReadFloatBE(buffer);

    int exp;
    const float x = frexpf(check, &exp);
    REQUIRE(x == -0.666405201f);
    REQUIRE(exp == -7);
}

TEST_CASE("WriteFloatBE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteFloatBE(buffer, ldexpf(-0.666405201f, -7));

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("ReadU64LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    REQUIRE(LexIO::ReadU64LE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("WriteU64LE")
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

TEST_CASE("ReadU64BE")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::ReadU64BE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("WriteU64BE")
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

TEST_CASE("Read64LE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    REQUIRE(LexIO::Read64LE(buffer) == -4822678189205112);
}

TEST_CASE("Write64LE")
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

TEST_CASE("Read64BE")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});
    REQUIRE(LexIO::Read64BE(buffer) == -4822678189205112);
}

TEST_CASE("Write64BE")
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

TEST_CASE("ReadDoubleLE")
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    const double check = LexIO::ReadDoubleLE(buffer);

    int exp;
    const double x = frexp(check, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
}

TEST_CASE("WriteDoubleLE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteDoubleLE(buffer, ldexp(-0.96457516339869276, 1024));
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

TEST_CASE("ReadDoubleBE")
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});
    const double check = LexIO::ReadDoubleBE(buffer);

    int exp;
    const double x = frexp(check, &exp);
    REQUIRE(x == -0.96457516339869276);
    REQUIRE(exp == 1024);
}

TEST_CASE("WriteDoubleBE")
{
    LexIO::VectorStream buffer;
    LexIO::WriteDoubleBE(buffer, ldexp(-0.96457516339869276, 1024));
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

TEST_CASE("ReadUVarint64")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});
    REQUIRE(LexIO::ReadUVarint64(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("WriteUVarint64")
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

TEST_CASE("ReadVarint64")
{
    LexIO::VectorStream buffer({0x88, 0xb3, 0xaa, 0xdd, 0xcb, 0xb9, 0xb7, 0xf7, 0xff, 0x01});
    REQUIRE(LexIO::ReadVarint64(buffer) == -4822678189205112);
}

TEST_CASE("WriteVarint64")
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

TEST_CASE("ReadSVarint64")
{
    LexIO::VectorStream buffer({0xef, 0x99, 0xab, 0xc5, 0xe8, 0x8c, 0x91, 0x11});
    REQUIRE(LexIO::ReadSVarint64(buffer) == -4822678189205112);
}

TEST_CASE("WriteSVarint64")
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

TEST_SUITE_END();
