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
    auto viewStream = LexIO::ViewStream{};

    EXPECT_EQ(0, LexIO::Length(viewStream));
}

TEST(ViewStream, CopyCtor)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto copyStream = GetViewStream(buffer);
    auto viewStream = LexIO::ViewStream{copyStream};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(buffer[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, CopyAssign)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto copyStream = GetViewStream(buffer);
    auto viewStream = LexIO::ViewStream{};
    viewStream = copyStream;

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(buffer[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, MoveCtor)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto moveStream = GetViewStream(buffer);
    auto viewStream = LexIO::ViewStream{std::move(moveStream)};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(buffer[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, MoveAssign)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = LexIO::ViewStream{};
    viewStream = GetViewStream(buffer);

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(buffer[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, VectorIterCtor)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    std::memcpy(&buffer[0], &BUFFER_TEXT[0], BUFFER_LENGTH);
    auto viewStream = LexIO::ViewStream{&buffer[0], &buffer[BUFFER_LENGTH]};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(buffer[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, VectorArrayCtor)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    std::memcpy(&buffer[0], &BUFFER_TEXT[0], BUFFER_LENGTH);
    auto viewStream = LexIO::ViewStream{buffer};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(buffer[i], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, Read)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        uint8_t data[1] = {0};
        EXPECT_EQ(1, LexIO::Read(data, viewStream));
        EXPECT_EQ(data[0], BUFFER_TEXT[i]);
    }
}

TEST(ViewStream, Write)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = LexIO::ViewStream{buffer};

    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(1, LexIO::Write(viewStream, &BUFFER_TEXT[i], 1));
        EXPECT_EQ(buffer[i], BUFFER_TEXT[i]);
    }
    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
}

TEST(ViewStream, Flush)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = LexIO::ViewStream{buffer};

    EXPECT_NO_THROW(LexIO::Flush(viewStream));
}

TEST(ViewStream, Seek)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    EXPECT_EQ(4, LexIO::Seek(viewStream, 4, LexIO::Whence::start));
    EXPECT_EQ(8, LexIO::Seek(viewStream, 4, LexIO::Whence::current));

    EXPECT_EQ(BUFFER_LENGTH - 4, LexIO::Seek(viewStream, 4, LexIO::Whence::end));
    EXPECT_EQ(BUFFER_LENGTH - 8, LexIO::Seek(viewStream, -4, LexIO::Whence::current));
}

TEST(ViewStream, Seek_Negative)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    EXPECT_ANY_THROW(LexIO::Seek(viewStream, -1, LexIO::Whence::current));
}
