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

#include "lexio/serialize/int.hpp"

#include "./test.h"

TEST(Int, TryReadU8_ReadU8)
{
    LexIO::VectorStream buffer = {0x88};

    uint8_t test;
    EXPECT_EQ(LexIO::TryReadU8(test, buffer), true);
    EXPECT_EQ(test, 0x88);
    EXPECT_EQ(LexIO::TryReadU8(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadU8(buffer), 0x88);
    EXPECT_ANY_THROW(LexIO::ReadU8(buffer));
}

TEST(Int, TryWriteU8_WriteU8)
{
    {
        uint8_t streamBuf[1] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteU8(buffer, 0x88), true);
        EXPECT_EQ(streamBuf[0], 0x88);
        EXPECT_EQ(LexIO::TryWriteU8(buffer, 0x88), false);
    }

    {
        uint8_t streamBuf[1] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteU8(buffer, 0x88));
        EXPECT_EQ(streamBuf[0], 0x88);
        EXPECT_ANY_THROW(LexIO::WriteU8(buffer, 0x88));
    }
}

TEST(Int, TryRead8_Read8)
{
    LexIO::VectorStream buffer = {0x88};

    int8_t test;
    EXPECT_EQ(LexIO::TryRead8(test, buffer), true);
    EXPECT_EQ(test, -120);
    EXPECT_EQ(LexIO::TryRead8(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::Read8(buffer), -120);
    EXPECT_ANY_THROW(LexIO::Read8(buffer));
}

TEST(Int, TryWrite8_Write8)
{
    {
        uint8_t streamBuf[1] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWrite8(buffer, -120), true);
        EXPECT_EQ(streamBuf[0], 0x88);
        EXPECT_EQ(LexIO::TryWrite8(buffer, -120), false);
    }

    {
        uint8_t streamBuf[1] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::Write8(buffer, -120));
        EXPECT_EQ(streamBuf[0], 0x88);
        EXPECT_ANY_THROW(LexIO::Write8(buffer, -120));
    }
}

TEST(Int, TryReadU16LE_ReadU16LE)
{
    LexIO::VectorStream buffer({0x88, 0x99});

    uint16_t test;
    EXPECT_EQ(LexIO::TryReadU16LE(test, buffer), true);
    EXPECT_EQ(test, 0x9988);
    EXPECT_EQ(LexIO::TryReadU16LE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadU16LE(buffer), 0x9988);
    EXPECT_ANY_THROW(LexIO::ReadU16LE(buffer));
}

TEST(Int, TryWriteU16LE_WriteU16LE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteU16LE(buffer, 0x9988), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(LexIO::TryWriteU16LE(buffer, 0x9988), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteU16LE(buffer, 0x9988));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_ANY_THROW(LexIO::WriteU16LE(buffer, 0x9988));
    }
}

TEST(Int, TryReadU16BE_ReadU16BE)
{
    LexIO::VectorStream buffer({0x88, 0x99});

    uint16_t test;
    EXPECT_EQ(LexIO::TryReadU16BE(test, buffer), true);
    EXPECT_EQ(test, 0x8899);
    EXPECT_EQ(LexIO::TryReadU16BE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadU16BE(buffer), 0x8899);
    EXPECT_ANY_THROW(LexIO::ReadU16BE(buffer));
}

TEST(Int, TryWriteU16BE_WriteU16BE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteU16BE(buffer, 0x9988), true);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWriteU16BE(buffer, 0x9988), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteU16BE(buffer, 0x9988));
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::WriteU16BE(buffer, 0x9988));
    }
}

TEST(Int, TryRead16LE_Read16LE)
{
    LexIO::VectorStream buffer({0x88, 0x99});

    int16_t test;
    EXPECT_EQ(LexIO::TryRead16LE(test, buffer), true);
    EXPECT_EQ(test, -26232);
    EXPECT_EQ(LexIO::TryRead16LE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::Read16LE(buffer), -26232);
    EXPECT_ANY_THROW(LexIO::Read16LE(buffer));
}

TEST(Int, TryWrite16LE_Write16LE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWrite16LE(buffer, -26232), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(LexIO::TryWrite16LE(buffer, -26232), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::Write16LE(buffer, -26232));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_ANY_THROW(LexIO::Write16LE(buffer, -26232));
    }
}

TEST(Int, TryRead16BE_Read16BE)
{
    LexIO::VectorStream buffer({0x99, 0x88});

    int16_t test;
    EXPECT_EQ(LexIO::TryRead16BE(test, buffer), true);
    EXPECT_EQ(test, -26232);
    EXPECT_EQ(LexIO::TryRead16BE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::Read16BE(buffer), -26232);
    EXPECT_ANY_THROW(LexIO::Read16BE(buffer));
}

TEST(Int, TryWrite16BE_Write16BE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWrite16BE(buffer, -26232), true);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWrite16BE(buffer, -26232), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[2] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::Write16BE(buffer, -26232));
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::Write16BE(buffer, -26232));
    }
}

TEST(Int, TryReadU32LE_ReadU32LE)
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});

    uint32_t test;
    EXPECT_EQ(LexIO::TryReadU32LE(test, buffer), true);
    EXPECT_EQ(test, 0xbbaa9988);
    EXPECT_EQ(LexIO::TryReadU32LE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadU32LE(buffer), 0xbbaa9988);
    EXPECT_ANY_THROW(LexIO::ReadU32LE(buffer));
}

TEST(Int, TryWriteU32LE_WriteU32LE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteU32LE(buffer, 0xbbaa9988), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(LexIO::TryWriteU32LE(buffer, 0xbbaa9988), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteU32LE(buffer, 0xbbaa9988));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_ANY_THROW(LexIO::WriteU32LE(buffer, 0xbbaa9988));
    }
}

TEST(Int, TryReadU32BE_ReadU32BE)
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});

    uint32_t test;
    EXPECT_EQ(LexIO::TryReadU32BE(test, buffer), true);
    EXPECT_EQ(test, 0xbbaa9988);
    EXPECT_EQ(LexIO::TryReadU32BE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadU32BE(buffer), 0xbbaa9988);
    EXPECT_ANY_THROW(LexIO::ReadU32BE(buffer));
}

TEST(Int, TryWriteU32BE_WriteU32BE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteU32BE(buffer, 0xbbaa9988), true);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWriteU32BE(buffer, 0xbbaa9988), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteU32BE(buffer, 0xbbaa9988));
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::WriteU32BE(buffer, 0xbbaa9988));
    }
}

TEST(Int, TryRead32LE_Read32LE)
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb});

    int32_t test;
    EXPECT_EQ(LexIO::TryRead32LE(test, buffer), true);
    EXPECT_EQ(test, -1146447480);
    EXPECT_EQ(LexIO::TryRead32LE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::Read32LE(buffer), -1146447480);
    EXPECT_ANY_THROW(LexIO::Read32LE(buffer));
}

TEST(Int, TryWrite32LE_Write32LE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWrite32LE(buffer, -1146447480), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(LexIO::TryWrite32LE(buffer, -1146447480), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::Write32LE(buffer, -1146447480));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_ANY_THROW(LexIO::Write32LE(buffer, -1146447480));
    }
}

TEST(Int, TryRead32BE_Read32BE)
{
    LexIO::VectorStream buffer({0xbb, 0xaa, 0x99, 0x88});

    int32_t test;
    EXPECT_EQ(LexIO::TryRead32BE(test, buffer), true);
    EXPECT_EQ(test, -1146447480);
    EXPECT_EQ(LexIO::TryRead32BE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::Read32BE(buffer), -1146447480);
    EXPECT_ANY_THROW(LexIO::Read32BE(buffer));
}

TEST(Int, TryWrite32BE_Write32BE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWrite32BE(buffer, -1146447480), true);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWrite32BE(buffer, -1146447480), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[4] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::Write32BE(buffer, -1146447480));
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::Write32BE(buffer, -1146447480));
    }
}

TEST(Int, TryReadU64LE_ReadU64LE)
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    uint64_t test;
    EXPECT_EQ(LexIO::TryReadU64LE(test, buffer), true);
    EXPECT_EQ(test, 0xffeeddccbbaa9988);
    EXPECT_EQ(LexIO::TryReadU64LE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadU64LE(buffer), 0xffeeddccbbaa9988);
    EXPECT_ANY_THROW(LexIO::ReadU64LE(buffer));
}

TEST(Int, TryWriteU64LE_WriteU64LE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteU64LE(buffer, 0xffeeddccbbaa9988), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(LexIO::TryWriteU64LE(buffer, 0xffeeddccbbaa9988), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_ANY_THROW(LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988));
    }
}

TEST(Int, TryReadU64BE_ReadU64BE)
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});

    uint64_t test;
    EXPECT_EQ(LexIO::TryReadU64BE(test, buffer), true);
    EXPECT_EQ(test, 0xffeeddccbbaa9988);
    EXPECT_EQ(LexIO::TryReadU64BE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::ReadU64BE(buffer), 0xffeeddccbbaa9988);
    EXPECT_ANY_THROW(LexIO::ReadU64BE(buffer));
}

TEST(Int, TryWriteU64BE_WriteU64BE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWriteU64BE(buffer, 0xffeeddccbbaa9988), true);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWriteU64BE(buffer, 0xffeeddccbbaa9988), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::WriteU64BE(buffer, 0xffeeddccbbaa9988));
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::WriteU64BE(buffer, 0xffeeddccbbaa9988));
    }
}

TEST(Int, TryRead64LE_Read64LE)
{
    LexIO::VectorStream buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});

    int64_t test;
    EXPECT_EQ(LexIO::TryRead64LE(test, buffer), true);
    EXPECT_EQ(test, -4822678189205112);
    EXPECT_EQ(LexIO::TryRead64LE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::Read64LE(buffer), -4822678189205112);
    EXPECT_ANY_THROW(LexIO::Read64LE(buffer));
}

TEST(Int, TryWrite64LE_Write64LE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWrite64LE(buffer, -4822678189205112), true);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(LexIO::TryWrite64LE(buffer, -4822678189205112), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::Write64LE(buffer, -4822678189205112));
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_ANY_THROW(LexIO::Write64LE(buffer, -4822678189205112));
    }
}

TEST(Int, TryRead64BE_Read64BE)
{
    LexIO::VectorStream buffer({0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88});

    int64_t test;
    EXPECT_EQ(LexIO::TryRead64BE(test, buffer), true);
    EXPECT_EQ(test, -4822678189205112);
    EXPECT_EQ(LexIO::TryRead64BE(test, buffer), false);

    LexIO::Rewind(buffer);
    EXPECT_EQ(LexIO::Read64BE(buffer), -4822678189205112);
    EXPECT_ANY_THROW(LexIO::Read64BE(buffer));
}

TEST(Int, TryWrite64BE_Write64BE)
{
    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_EQ(LexIO::TryWrite64BE(buffer, -4822678189205112), true);
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_EQ(LexIO::TryWrite64BE(buffer, -4822678189205112), false);
    }

    {
        size_t i = 0;
        uint8_t streamBuf[8] = {0};
        auto buffer = LexIO::ViewStream{streamBuf};

        EXPECT_NO_THROW(LexIO::Write64BE(buffer, -4822678189205112));
        EXPECT_EQ(streamBuf[i++], 0xff);
        EXPECT_EQ(streamBuf[i++], 0xee);
        EXPECT_EQ(streamBuf[i++], 0xdd);
        EXPECT_EQ(streamBuf[i++], 0xcc);
        EXPECT_EQ(streamBuf[i++], 0xbb);
        EXPECT_EQ(streamBuf[i++], 0xaa);
        EXPECT_EQ(streamBuf[i++], 0x99);
        EXPECT_EQ(streamBuf[i++], 0x88);
        EXPECT_ANY_THROW(LexIO::Write64BE(buffer, -4822678189205112));
    }
}
