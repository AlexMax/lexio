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

#include "lexio/stream/view.hpp"

#include "./test.h"

//******************************************************************************

TEST(ViewStream, FulfillReader)
{
    EXPECT_TRUE(LexIO::IsReaderV<LexIO::ViewStream>);
}

TEST(ViewStream, FailBufferedReader)
{
    EXPECT_FALSE(LexIO::IsBufferedReaderV<LexIO::ViewStream>);
}

TEST(ViewStream, FulfillWriter)
{
    EXPECT_TRUE(LexIO::IsWriterV<LexIO::ViewStream>);
}

TEST(ViewStream, FulfillSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekableV<LexIO::ViewStream>);
}

TEST(ViewStream, DefCtor)
{
    auto vecStream = LexIO::ViewStream{};

    EXPECT_EQ(0, LexIO::Length(vecStream));
}

TEST(ViewStream, CopyCtor)
{
    auto copyStream = GetVectorStream();
    auto vecStream = LexIO::ViewStream{copyStream};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, CopyAssign)
{
    auto copyStream = GetVectorStream();
    auto vecStream = LexIO::ViewStream{};
    vecStream = copyStream;

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, MoveCtor)
{
    auto vecStream = LexIO::ViewStream{GetVectorStream()};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, MoveAssign)
{
    auto vecStream = LexIO::ViewStream{};
    vecStream = GetVectorStream();

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, VectorCopyCtor)
{
    auto copyVec = std::vector<uint8_t>{&BUFFER_TEXT[0], &BUFFER_TEXT[BUFFER_LENGTH]};
    auto vecStream = LexIO::ViewStream{copyVec};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, VectorMoveCtor)
{
    auto moveVec = std::vector<uint8_t>{&BUFFER_TEXT[0], &BUFFER_TEXT[BUFFER_LENGTH]};
    auto vecStream = LexIO::ViewStream{std::move(moveVec)};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, Read)
{
    auto vecStream = GetVectorStream();

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        uint8_t data[1] = {0};
        EXPECT_EQ(1, LexIO::Read(data, vecStream));
        EXPECT_EQ(data[0], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, Write)
{
    auto vecStream = LexIO::ViewStream{};

    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(1, LexIO::Write(vecStream, &BUFFER_TEXT[i], 1));
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
}

TEST(ViewStream, Seek)
{
    auto vecStream = GetVectorStream();

    EXPECT_EQ(4, LexIO::Seek(vecStream, 4, LexIO::Whence::start));
    EXPECT_EQ(8, LexIO::Seek(vecStream, 4, LexIO::Whence::current));

    EXPECT_EQ(BUFFER_LENGTH - 4, LexIO::Seek(vecStream, 4, LexIO::Whence::end));
    EXPECT_EQ(BUFFER_LENGTH - 8, LexIO::Seek(vecStream, -4, LexIO::Whence::current));
}

TEST(ViewStream, Seek_Negative)
{
    auto vecStream = GetVectorStream();

    EXPECT_ANY_THROW(LexIO::Seek(vecStream, -1, LexIO::Whence::current));
}
