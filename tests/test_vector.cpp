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

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, CopyAssign)
{
    auto copyStream = GetVectorStream();
    auto vecStream = LexIO::VectorStream{};
    vecStream = copyStream;

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, MoveCtor)
{
    auto vecStream = LexIO::VectorStream{GetVectorStream()};

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, MoveAssign)
{
    auto vecStream = LexIO::VectorStream{};
    vecStream = GetVectorStream();

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, VectorCopyCtor)
{
    auto copyVec = std::vector<uint8_t>{&TEST_TEXT_DATA[0], &TEST_TEXT_DATA[TEST_TEXT_LENGTH]};
    auto vecStream = LexIO::VectorStream{copyVec};

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, VectorMoveCtor)
{
    auto moveVec = std::vector<uint8_t>{&TEST_TEXT_DATA[0], &TEST_TEXT_DATA[TEST_TEXT_LENGTH]};
    auto vecStream = LexIO::VectorStream{std::move(moveVec)};

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vecStream.Container()[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, CopyFrom)
{
    auto vecStream = LexIO::VectorStream{};
    LexIO::Write(vecStream, &TEST_TEXT_DATA[0], TEST_TEXT_LENGTH);

    auto vec = vecStream.Container();
    EXPECT_EQ(TEST_TEXT_LENGTH, vec.size());
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vec[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, MoveFrom)
{
    auto vecStream = LexIO::VectorStream{};
    LexIO::Write(vecStream, &TEST_TEXT_DATA[0], TEST_TEXT_LENGTH);

    auto vec = std::move(vecStream).Container();
    EXPECT_EQ(TEST_TEXT_LENGTH, vec.size());
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(vec[i], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, Read)
{
    auto vecStream = GetVectorStream();

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        uint8_t data[1] = {0};
        EXPECT_EQ(1, LexIO::Read(data, vecStream));
        EXPECT_EQ(data[0], TEST_TEXT_DATA[i]);
    }
}

TEST(VectorStream, FillBufferSingle)
{
    auto bufReader = GetVectorStream();

    auto test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[7], 'c');
    EXPECT_EQ(test.Size(), 8);
}

TEST(VectorStream, FillBufferMultiple)
{
    auto bufReader = GetVectorStream();

    // Buffer initial four bytes.
    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Size(), 4);

    // Buffer less than what we had before, should read nothing.
    test = LexIO::FillBuffer(bufReader, 2);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Size(), 4);

    // Buffer more than what we had before.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Data()[4], 'q');
    EXPECT_EQ(test.Data()[7], 'c');
    EXPECT_EQ(test.Size(), 8);
}

TEST(VectorStream, FillBufferEOF)
{
    auto bufReader = GetVectorStream();

    // Buffer everything.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[TEST_TEXT_LENGTH - 1], '\n');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH);

    // Buffer more than everything.
    test = LexIO::FillBuffer(bufReader, 96);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[TEST_TEXT_LENGTH - 1], '\n');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH);
}

TEST(VectorStream, FillBufferEOFInitial)
{
    auto bufReader = GetVectorStream();

    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Size(), 4);

    // Buffer everything.
    test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Data()[4], 'q');
    EXPECT_EQ(test.Data()[TEST_TEXT_LENGTH - 1], '\n');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH);
}

TEST(VectorStream, FillBufferZeroRead)
{
    auto bufReader = GetVectorStream();

    auto test = LexIO::FillBuffer(bufReader, 0);
    EXPECT_EQ(test.Size(), 0);
}

TEST(VectorStream, ConsumeBufferSingle)
{
    auto bufReader = GetVectorStream();

    // Fill, then consume whole buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 8));

    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Size(), 0);

    // Subsequent read should pick up where we left off.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.Data()[0], 'k');
    EXPECT_EQ(test.Data()[7], ' ');
    EXPECT_EQ(test.Size(), 8);
}

TEST(VectorStream, ConsumeBufferMultiple)
{
    auto bufReader = GetVectorStream();

    // Consume half the buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Data()[0], 'q');
    EXPECT_EQ(test.Data()[3], 'c');
    EXPECT_EQ(test.Size(), 4);

    // Consume the other half of the buffer.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Size(), 0);
}

TEST(VectorStream, ConsumeBufferEOF)
{
    auto bufReader = GetVectorStream();

    // Fill to EOF, consume part of it.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Data()[0], 'q');
    EXPECT_EQ(test.Data()[3], 'c');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH - 4);

    // Consume the rest of it.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, TEST_TEXT_LENGTH - 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Size(), 0);
}

TEST(VectorStream, ConsumeBufferTooLarge)
{
    auto bufReader = GetVectorStream();

    LexIO::FillBuffer(bufReader, 8);
    EXPECT_ANY_THROW(LexIO::ConsumeBuffer(bufReader, 12));
}

TEST(VectorStream, FillBufferSmallRead)
{
    auto vectorStream = GetVectorStream();
    uint8_t data[5] = {0};

    auto view = LexIO::FillBuffer(vectorStream, 8);
    EXPECT_EQ(8, view.Size());

    // Do a small-enough read to where we don't use up our entire buffer.
    EXPECT_EQ(5, LexIO::Read(data, vectorStream));

    size_t i = 0;
    EXPECT_EQ(data[i++], 'T');
    EXPECT_EQ(data[i++], 'h');
    EXPECT_EQ(data[i++], 'e');
    EXPECT_EQ(data[i++], ' ');
    EXPECT_EQ(data[i++], 'q');

    // Do a larger read that goes past what we've previously filled the
    // buffer with.
    EXPECT_EQ(5, LexIO::Read(data, vectorStream));

    i = 0;
    EXPECT_EQ(data[i++], 'u');
    EXPECT_EQ(data[i++], 'i');
    EXPECT_EQ(data[i++], 'c');
    EXPECT_EQ(data[i++], 'k');
    EXPECT_EQ(data[i++], ' ');
}

TEST(VectorStream, FillBufferWrite)
{
    auto bufReader = GetVectorStream();

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Size(), 4);
    LexIO::Write(bufReader, {'X', 'Y', 'Z', 'Z'});
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Data()[0], 'k');
    EXPECT_EQ(view.Data()[1], ' ');
    EXPECT_EQ(view.Data()[2], 'b');
    EXPECT_EQ(view.Data()[3], 'r');
    EXPECT_EQ(view.Size(), 4);
}

TEST(VectorStream, FillBufferFlush)
{
    auto bufReader = GetVectorStream();

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Size(), 4);
    LexIO::Flush(bufReader);
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Data()[0], 'T');
    EXPECT_EQ(view.Data()[1], 'h');
    EXPECT_EQ(view.Data()[2], 'e');
    EXPECT_EQ(view.Data()[3], ' ');
    EXPECT_EQ(view.Size(), 4);
}

TEST(VectorStream, FillBufferSeek)
{
    auto bufReader = GetVectorStream();

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Size(), 4);
    LexIO::Seek(bufReader, 4, LexIO::Whence::current);
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Data()[0], 'k');
    EXPECT_EQ(view.Data()[1], ' ');
    EXPECT_EQ(view.Data()[2], 'b');
    EXPECT_EQ(view.Data()[3], 'r');
    EXPECT_EQ(view.Size(), 4);
}

TEST(VectorStream, Write)
{
    auto vecStream = LexIO::VectorStream{};

    for (size_t i = 0; i < TEST_TEXT_LENGTH; i++)
    {
        EXPECT_EQ(1, LexIO::Write(vecStream, &TEST_TEXT_DATA[i], 1));
        EXPECT_EQ(vecStream.Container()[i], TEST_TEXT_DATA[i]);
    }
    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Length(vecStream));
}

TEST(VectorStream, Seek)
{
    auto vecStream = GetVectorStream();

    EXPECT_EQ(4, LexIO::Seek(vecStream, 4, LexIO::Whence::start));
    EXPECT_EQ(8, LexIO::Seek(vecStream, 4, LexIO::Whence::current));

    EXPECT_EQ(TEST_TEXT_LENGTH - 4, LexIO::Seek(vecStream, 4, LexIO::Whence::end));
    EXPECT_EQ(TEST_TEXT_LENGTH - 8, LexIO::Seek(vecStream, -4, LexIO::Whence::current));
}

TEST(VectorStream, Seek_Negative)
{
    auto vecStream = GetVectorStream();

    EXPECT_ANY_THROW(LexIO::Seek(vecStream, -1, LexIO::Whence::current));
}
