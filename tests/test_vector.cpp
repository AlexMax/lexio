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

#include "lexio/stream/vector.hpp"

#include "./test.h"

//******************************************************************************

TEST(VectorStream, FulfillReader)
{
    EXPECT_TRUE(LexIO::IsReaderV<LexIO::VectorStream>);
}

TEST(VectorStream, FailBufferedReader)
{
    EXPECT_FALSE(LexIO::IsBufferedReaderV<LexIO::VectorStream>);
}

TEST(VectorStream, FulfillWriter)
{
    EXPECT_TRUE(LexIO::IsWriterV<LexIO::VectorStream>);
}

TEST(VectorStream, FulfillSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekableV<LexIO::VectorStream>);
}

TEST(VectorStream, DefCtor)
{
    auto vecStream = LexIO::VectorStream{};

    EXPECT_EQ(0, LexIO::Length(vecStream));
}

TEST(VectorStream, CopyCtor)
{
    auto copyStream = GetStream();
    auto vecStream = LexIO::VectorStream{copyStream};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, CopyAssign)
{
    auto copyStream = GetStream();
    auto vecStream = LexIO::VectorStream{};
    vecStream = copyStream;

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, MoveCtor)
{
    auto vecStream = LexIO::VectorStream{GetStream()};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, MoveAssign)
{
    auto vecStream = LexIO::VectorStream{};
    vecStream = GetStream();

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, VectorCopyCtor)
{
    auto copyVec = std::vector<uint8_t>{&BUFFER_TEXT[0], &BUFFER_TEXT[BUFFER_LENGTH]};
    auto vecStream = LexIO::VectorStream{copyVec};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, VectorMoveCtor)
{
    auto moveVec = std::vector<uint8_t>{&BUFFER_TEXT[0], &BUFFER_TEXT[BUFFER_LENGTH]};
    auto vecStream = LexIO::VectorStream{std::move(moveVec)};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, Read)
{
    auto vecStream = GetStream();

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        uint8_t data[1] = {0};
        EXPECT_EQ(1, LexIO::Read(data, vecStream));
        EXPECT_EQ(data[0], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, Write)
{
    auto vecStream = LexIO::VectorStream{};

    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(1, LexIO::Write(vecStream, &BUFFER_TEXT[i], 1));
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
}

TEST(VectorStream, Seek)
{
    auto vecStream = GetStream();

    EXPECT_EQ(4, LexIO::Seek(vecStream, 4, LexIO::Whence::start));
    EXPECT_EQ(8, LexIO::Seek(vecStream, 4, LexIO::Whence::current));

    EXPECT_EQ(BUFFER_LENGTH - 4, LexIO::Seek(vecStream, 4, LexIO::Whence::end));
    EXPECT_EQ(BUFFER_LENGTH - 8, LexIO::Seek(vecStream, -4, LexIO::Whence::current));
}

TEST(VectorStream, Seek_Negative)
{
    auto vecStream = GetStream();

    EXPECT_ANY_THROW(LexIO::Seek(vecStream, -1, LexIO::Whence::current));
}
