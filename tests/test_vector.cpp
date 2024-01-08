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
    EXPECT_TRUE(LexIO::IsBufferedReaderV<LexIO::VectorStream>);
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
    auto copyStream = GetVectorStream();
    auto vecStream = LexIO::VectorStream{copyStream};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, CopyAssign)
{
    auto copyStream = GetVectorStream();
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
    auto vecStream = LexIO::VectorStream{GetVectorStream()};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, MoveAssign)
{
    auto vecStream = LexIO::VectorStream{};
    vecStream = GetVectorStream();

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

TEST(VectorStream, CopyFrom)
{
    auto vecStream = LexIO::VectorStream{};
    LexIO::Write(vecStream, &BUFFER_TEXT[0], BUFFER_LENGTH);

    auto vec = vecStream.Container();
    EXPECT_EQ(BUFFER_LENGTH, vec.size());
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vec[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, MoveFrom)
{
    auto vecStream = LexIO::VectorStream{};
    LexIO::Write(vecStream, &BUFFER_TEXT[0], BUFFER_LENGTH);

    auto vec = std::move(vecStream).Container();
    EXPECT_EQ(BUFFER_LENGTH, vec.size());
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vec[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, CopyTo)
{
    auto vec = std::vector<uint8_t>{&BUFFER_TEXT[0], &BUFFER_TEXT[BUFFER_LENGTH]};
    auto vecStream = LexIO::VectorStream{};

    vecStream.Container(vec);
    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, MoveTo)
{
    auto vec = std::vector<uint8_t>{&BUFFER_TEXT[0], &BUFFER_TEXT[BUFFER_LENGTH]};
    auto vecStream = LexIO::VectorStream{};

    vecStream.Container(std::move(vec));
    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], BUFFER_TEXT[i]);
    }
}

TEST(VectorStream, Read)
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

TEST(VectorStream, FillBufferSingle)
{
    auto bufReader = GetVectorStream();

    auto test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(VectorStream, FillBufferMultiple)
{
    auto bufReader = GetVectorStream();

    // Buffer initial four bytes.
    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer less than what we had before, should read nothing.
    test = LexIO::FillBuffer(bufReader, 2);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer more than what we had before.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(VectorStream, FillBufferEOF)
{
    auto bufReader = GetVectorStream();

    // Buffer everything.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);

    // Buffer more than everything.
    test = LexIO::FillBuffer(bufReader, 96);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(VectorStream, FillBufferEOFInitial)
{
    auto bufReader = GetVectorStream();

    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer everything.
    test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(VectorStream, FillBufferZeroRead)
{
    auto bufReader = GetVectorStream();

    auto test = LexIO::FillBuffer(bufReader, 0);
    EXPECT_EQ(test.second, 0);
}

TEST(VectorStream, ConsumeBufferSingle)
{
    auto bufReader = GetVectorStream();

    // Fill, then consume whole buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 8));

    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.second, 0);

    // Subsequent read should pick up where we left off.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.first[0], 'k');
    EXPECT_EQ(test.first[7], ' ');
    EXPECT_EQ(test.second, 8);
}

TEST(VectorStream, ConsumeBufferMultiple)
{
    auto bufReader = GetVectorStream();

    // Consume half the buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, 4);

    // Consume the other half of the buffer.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.second, 0);
}

TEST(VectorStream, ConsumeBufferEOF)
{
    auto bufReader = GetVectorStream();

    // Fill to EOF, consume part of it.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, BUFFER_LENGTH - 4);

    // Consume the rest of it.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, BUFFER_LENGTH - 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.second, 0);
}

TEST(VectorStream, ConsumeBufferTooLarge)
{
    auto bufReader = GetVectorStream();

    LexIO::FillBuffer(bufReader, 8);
    EXPECT_ANY_THROW(LexIO::ConsumeBuffer(bufReader, 12));
}

TEST(VectorStream, FillBufferWrite)
{
    auto bufReader = GetVectorStream();

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.second, 4);
    LexIO::Write(bufReader, {'X', 'Y', 'Z', 'Z'});
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.first[0], 'k');
    EXPECT_EQ(view.first[1], ' ');
    EXPECT_EQ(view.first[2], 'b');
    EXPECT_EQ(view.first[3], 'r');
    EXPECT_EQ(view.second, 4);
}

TEST(VectorStream, FillBufferFlush)
{
    auto bufReader = GetVectorStream();

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.second, 4);
    LexIO::Flush(bufReader);
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.first[0], 'T');
    EXPECT_EQ(view.first[1], 'h');
    EXPECT_EQ(view.first[2], 'e');
    EXPECT_EQ(view.first[3], ' ');
    EXPECT_EQ(view.second, 4);
}

TEST(VectorStream, FillBufferSeek)
{
    auto bufReader = GetVectorStream();

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.second, 4);
    LexIO::Seek(bufReader, 4, LexIO::Whence::current);
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.first[0], 'k');
    EXPECT_EQ(view.first[1], ' ');
    EXPECT_EQ(view.first[2], 'b');
    EXPECT_EQ(view.first[3], 'r');
    EXPECT_EQ(view.second, 4);
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
    auto vecStream = GetVectorStream();

    EXPECT_EQ(4, LexIO::Seek(vecStream, 4, LexIO::Whence::start));
    EXPECT_EQ(8, LexIO::Seek(vecStream, 4, LexIO::Whence::current));

    EXPECT_EQ(BUFFER_LENGTH - 4, LexIO::Seek(vecStream, 4, LexIO::Whence::end));
    EXPECT_EQ(BUFFER_LENGTH - 8, LexIO::Seek(vecStream, -4, LexIO::Whence::current));
}

TEST(VectorStream, Seek_Negative)
{
    auto vecStream = GetVectorStream();

    EXPECT_ANY_THROW(LexIO::Seek(vecStream, -1, LexIO::Whence::current));
}
