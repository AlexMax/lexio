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

#include "lexio/serialize/float.hpp"

#include "./test.h"
#include <cmath>

static ErrorStream g_errorStream;

TEST(Float, TryReadFloat32LE_ReadFloat32LE)
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});

    int exp = 0;
    LexIO::float32_t test = 0, x = 0;
    EXPECT_EQ(LexIO::TryReadFloat32LE(test, buffer), true);
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.666405201f);
    EXPECT_EQ(exp, -7);
    EXPECT_EQ(LexIO::TryReadFloat32LE(test, buffer), false);
    EXPECT_EQ(LexIO::TryReadFloat32LE(test, g_errorStream), false);

    LexIO::Rewind(buffer);
    EXPECT_NO_THROW(test = LexIO::ReadFloat32LE(buffer));
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.666405201f);
    EXPECT_EQ(exp, -7);
    EXPECT_ANY_THROW(LexIO::ReadFloat32LE(buffer));
}

TEST(Float, TryWriteFloat32LE_WriteFloat32LE)
{
    const LexIO::float32_t test = std::ldexp(-0.666405201f, -7);

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteFloat32LE(buffer, test), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(LexIO::TryWriteFloat32LE(buffer, test), false);
        EXPECT_EQ(LexIO::TryWriteFloat32LE(g_errorStream, test), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteFloat32LE(buffer, test));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_ANY_THROW(LexIO::WriteFloat32LE(buffer, test));
    }
}

TEST(Float, TryReadFloat32BE_ReadFloat32BE)
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});

    int exp = 0;
    LexIO::float32_t test = 0, x = 0;
    EXPECT_EQ(LexIO::TryReadFloat32BE(test, buffer), true);
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.666405201f);
    EXPECT_EQ(exp, -7);
    EXPECT_EQ(LexIO::TryReadFloat32BE(test, buffer), false);
    EXPECT_EQ(LexIO::TryReadFloat32BE(test, g_errorStream), false);

    LexIO::Rewind(buffer);
    EXPECT_NO_THROW(test = LexIO::ReadFloat32BE(buffer));
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.666405201f);
    EXPECT_EQ(exp, -7);
    EXPECT_ANY_THROW(LexIO::ReadFloat32BE(buffer));
}

TEST(Float, TryWriteFloat32BE_WriteFloat32BE)
{
    const LexIO::float32_t test = std::ldexp(-0.666405201f, -7);

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteFloat32BE(buffer, test), true);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWriteFloat32BE(buffer, test), false);
        EXPECT_EQ(LexIO::TryWriteFloat32BE(g_errorStream, test), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteFloat32BE(buffer, test));
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::WriteFloat32BE(buffer, test));
    }
}

TEST(Float, TryReadFloat64LE_ReadFloat64LE)
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    int exp = 0;
    LexIO::float64_t test = 0, x = 0;
    EXPECT_EQ(LexIO::TryReadFloat64LE(test, buffer), true);
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.96457516339869276);
    EXPECT_EQ(exp, 1024);
    EXPECT_EQ(LexIO::TryReadFloat64LE(test, buffer), false);
    EXPECT_EQ(LexIO::TryReadFloat64LE(test, g_errorStream), false);

    LexIO::Rewind(buffer);
    EXPECT_NO_THROW(test = LexIO::ReadFloat64LE(buffer));
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.96457516339869276);
    EXPECT_EQ(exp, 1024);
    EXPECT_ANY_THROW(LexIO::ReadFloat64LE(buffer));
}

TEST(Float, TryWriteFloat64LE_WriteFloat64LE)
{
    const LexIO::float64_t test = std::ldexp(-0.96457516339869276, 1024);

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteFloat64LE(buffer, test), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(LexIO::TryWriteFloat64LE(buffer, test), false);
        EXPECT_EQ(LexIO::TryWriteFloat64LE(g_errorStream, test), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteFloat64LE(buffer, test));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_ANY_THROW(LexIO::WriteFloat64LE(buffer, test));
    }
}

TEST(Float, TryReadFloat64BE_ReadFloat64BE)
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});

    int exp = 0;
    LexIO::float64_t test = 0, x = 0;
    EXPECT_EQ(LexIO::TryReadFloat64BE(test, buffer), true);
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.96457516339869276);
    EXPECT_EQ(exp, 1024);
    EXPECT_EQ(LexIO::TryReadFloat64BE(test, buffer), false);
    EXPECT_EQ(LexIO::TryReadFloat64BE(test, g_errorStream), false);

    LexIO::Rewind(buffer);
    EXPECT_NO_THROW(test = LexIO::ReadFloat64BE(buffer));
    x = std::frexp(test, &exp);
    EXPECT_EQ(x, -0.96457516339869276);
    EXPECT_EQ(exp, 1024);
    EXPECT_ANY_THROW(LexIO::ReadFloat64BE(buffer));
}

TEST(Float, TryWriteFloat64LE_WriteFloat64BE)
{
    const LexIO::float64_t test = std::ldexp(-0.96457516339869276, 1024);

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteFloat64BE(buffer, test), true);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWriteFloat64BE(buffer, test), false);
        EXPECT_EQ(LexIO::TryWriteFloat64BE(g_errorStream, test), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteFloat64BE(buffer, test));
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::WriteFloat64BE(buffer, test));
    }
}
