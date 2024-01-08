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
#include "lexio/serialize/int.hpp"

//******************************************************************************

TEST(ViewStream, FulfillReader)
{
    EXPECT_TRUE(LexIO::IsReaderV<LexIO::ViewStream>);
}

TEST(ViewStream, FailBufferedReader)
{
    EXPECT_TRUE(LexIO::IsBufferedReaderV<LexIO::ViewStream>);
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

TEST(ViewStream, FillBufferSingle)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    auto test = LexIO::FillBuffer(viewStream, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(ViewStream, FillBufferMultiple)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    // Buffer initial four bytes.
    auto test = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer less than what we had before, should read nothing.
    test = LexIO::FillBuffer(viewStream, 2);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer more than what we had before.
    test = LexIO::FillBuffer(viewStream, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(ViewStream, FillBufferEOF)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    // Buffer everything.
    auto test = LexIO::FillBuffer(viewStream, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);

    // Buffer more than everything.
    test = LexIO::FillBuffer(viewStream, 96);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(ViewStream, FillBufferEOFInitial)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    auto test = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer everything.
    test = LexIO::FillBuffer(viewStream, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(ViewStream, FillBufferZeroRead)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    auto test = LexIO::FillBuffer(viewStream, 0);
    EXPECT_EQ(test.second, 0);
}

TEST(ViewStream, FillBufferWrite)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    auto view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.second, 4);
    LexIO::Write(viewStream, {'X', 'Y', 'Z', 'Z'});
    view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.first[0], 'k');
    EXPECT_EQ(view.first[1], ' ');
    EXPECT_EQ(view.first[2], 'b');
    EXPECT_EQ(view.first[3], 'r');
    EXPECT_EQ(view.second, 4);
}

TEST(ViewStream, FillBufferFlush)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    auto view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.second, 4);
    LexIO::Flush(viewStream);
    view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.first[0], 'T');
    EXPECT_EQ(view.first[1], 'h');
    EXPECT_EQ(view.first[2], 'e');
    EXPECT_EQ(view.first[3], ' ');
    EXPECT_EQ(view.second, 4);
}

TEST(ViewStream, FillBufferSeek)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    auto view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.second, 4);
    LexIO::Seek(viewStream, 4, LexIO::Whence::current);
    view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.first[0], 'k');
    EXPECT_EQ(view.first[1], ' ');
    EXPECT_EQ(view.first[2], 'b');
    EXPECT_EQ(view.first[3], 'r');
    EXPECT_EQ(view.second, 4);
}

TEST(ViewStream, ConsumeBufferSingle)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    // Fill, then consume whole buffer.
    LexIO::FillBuffer(viewStream, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 8));

    auto test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.second, 0);

    // Subsequent read should pick up where we left off.
    test = LexIO::FillBuffer(viewStream, 8);
    EXPECT_EQ(test.first[0], 'k');
    EXPECT_EQ(test.first[7], ' ');
    EXPECT_EQ(test.second, 8);
}

TEST(ViewStream, ConsumeBufferMultiple)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    // Consume half the buffer.
    LexIO::FillBuffer(viewStream, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 4));
    auto test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, 4);

    // Consume the other half of the buffer.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 4));
    test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.second, 0);
}

TEST(ViewStream, ConsumeBufferEOF)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    // Fill to EOF, consume part of it.
    auto test = LexIO::FillBuffer(viewStream, 64);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 4));
    test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, BUFFER_LENGTH - 4);

    // Consume the rest of it.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, BUFFER_LENGTH - 4));
    test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.second, 0);
}

TEST(ViewStream, ConsumeBufferTooLarge)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    auto viewStream = GetViewStream(buffer);

    LexIO::FillBuffer(viewStream, 8);
    EXPECT_ANY_THROW(LexIO::ConsumeBuffer(viewStream, 12));
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

//******************************************************************************

TEST(ConstViewStream, FulfillReader)
{
    EXPECT_TRUE(LexIO::IsReaderV<LexIO::ConstViewStream>);
}

TEST(ConstViewStream, FailBufferedReader)
{
    EXPECT_TRUE(LexIO::IsBufferedReaderV<LexIO::ConstViewStream>);
}

TEST(ConstViewStream, FulfillWriter)
{
    EXPECT_FALSE(LexIO::IsWriterV<LexIO::ConstViewStream>);
}

TEST(ConstViewStream, FulfillSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekableV<LexIO::ConstViewStream>);
}

TEST(ConstViewStream, DefCtor)
{
    auto viewStream = LexIO::ConstViewStream{};

    EXPECT_EQ(0, LexIO::Length(viewStream));
}

TEST(ConstViewStream, CopyCtor)
{
    auto copyStream = GetConstViewStream();
    auto viewStream = LexIO::ConstViewStream{copyStream};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(LexIO::ReadU8(viewStream), BUFFER_TEXT[i]);
    }
}

TEST(ConstViewStream, CopyAssign)
{
    auto copyStream = GetConstViewStream();
    auto viewStream = LexIO::ConstViewStream{};
    viewStream = copyStream;

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(LexIO::ReadU8(viewStream), BUFFER_TEXT[i]);
    }
}

TEST(ConstViewStream, MoveCtor)
{
    auto moveStream = GetConstViewStream();
    auto viewStream = LexIO::ConstViewStream{std::move(moveStream)};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(LexIO::ReadU8(viewStream), BUFFER_TEXT[i]);
    }
}

TEST(ConstViewStream, MoveAssign)
{
    auto viewStream = LexIO::ConstViewStream{};
    viewStream = GetConstViewStream();

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(LexIO::ReadU8(viewStream), BUFFER_TEXT[i]);
    }
}

TEST(ConstViewStream, VectorIterCtor)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    std::memcpy(&buffer[0], &BUFFER_TEXT[0], BUFFER_LENGTH);
    auto viewStream = LexIO::ConstViewStream{&buffer[0], &buffer[BUFFER_LENGTH]};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(LexIO::ReadU8(viewStream), BUFFER_TEXT[i]);
    }
}

TEST(ConstViewStream, VectorArrayCtor)
{
    uint8_t buffer[BUFFER_LENGTH] = {0};
    std::memcpy(&buffer[0], &BUFFER_TEXT[0], BUFFER_LENGTH);
    auto viewStream = LexIO::ConstViewStream{buffer};

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        EXPECT_EQ(LexIO::ReadU8(viewStream), BUFFER_TEXT[i]);
    }
}

TEST(ConstViewStream, Read)
{
    auto viewStream = GetConstViewStream();

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Length(viewStream));
    for (size_t i = 0; i < BUFFER_LENGTH; i++)
    {
        uint8_t data[1] = {0};
        EXPECT_EQ(1, LexIO::Read(data, viewStream));
        EXPECT_EQ(data[0], BUFFER_TEXT[i]);
    }
}

TEST(ConstViewStream, FillBufferSingle)
{
    auto viewStream = GetConstViewStream();

    auto test = LexIO::FillBuffer(viewStream, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(ConstViewStream, FillBufferMultiple)
{
    auto viewStream = GetConstViewStream();

    // Buffer initial four bytes.
    auto test = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer less than what we had before, should read nothing.
    test = LexIO::FillBuffer(viewStream, 2);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer more than what we had before.
    test = LexIO::FillBuffer(viewStream, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(ConstViewStream, FillBufferEOF)
{
    auto viewStream = GetConstViewStream();

    // Buffer everything.
    auto test = LexIO::FillBuffer(viewStream, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);

    // Buffer more than everything.
    test = LexIO::FillBuffer(viewStream, 96);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(ConstViewStream, FillBufferEOFInitial)
{
    auto viewStream = GetConstViewStream();

    auto test = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer everything.
    test = LexIO::FillBuffer(viewStream, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(ConstViewStream, FillBufferZeroRead)
{
    auto viewStream = GetConstViewStream();

    auto test = LexIO::FillBuffer(viewStream, 0);
    EXPECT_EQ(test.second, 0);
}

TEST(ConstViewStream, FillBufferSeek)
{
    auto viewStream = GetConstViewStream();

    auto view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.second, 4);
    LexIO::Seek(viewStream, 4, LexIO::Whence::current);
    view = LexIO::FillBuffer(viewStream, 4);
    EXPECT_EQ(view.first[0], 'k');
    EXPECT_EQ(view.first[1], ' ');
    EXPECT_EQ(view.first[2], 'b');
    EXPECT_EQ(view.first[3], 'r');
    EXPECT_EQ(view.second, 4);
}

TEST(ConstViewStream, ConsumeBufferSingle)
{
    auto viewStream = GetConstViewStream();

    // Fill, then consume whole buffer.
    LexIO::FillBuffer(viewStream, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 8));

    auto test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.second, 0);

    // Subsequent read should pick up where we left off.
    test = LexIO::FillBuffer(viewStream, 8);
    EXPECT_EQ(test.first[0], 'k');
    EXPECT_EQ(test.first[7], ' ');
    EXPECT_EQ(test.second, 8);
}

TEST(ConstViewStream, ConsumeBufferMultiple)
{
    auto viewStream = GetConstViewStream();

    // Consume half the buffer.
    LexIO::FillBuffer(viewStream, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 4));
    auto test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, 4);

    // Consume the other half of the buffer.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 4));
    test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.second, 0);
}

TEST(ConstViewStream, ConsumeBufferEOF)
{
    auto viewStream = GetConstViewStream();

    // Fill to EOF, consume part of it.
    auto test = LexIO::FillBuffer(viewStream, 64);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, 4));
    test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, BUFFER_LENGTH - 4);

    // Consume the rest of it.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(viewStream, BUFFER_LENGTH - 4));
    test = LexIO::GetBuffer(viewStream);
    EXPECT_EQ(test.second, 0);
}

TEST(ConstViewStream, ConsumeBufferTooLarge)
{
    auto viewStream = GetConstViewStream();

    LexIO::FillBuffer(viewStream, 8);
    EXPECT_ANY_THROW(LexIO::ConsumeBuffer(viewStream, 12));
}

TEST(ConstViewStream, Seek)
{
    auto viewStream = GetConstViewStream();

    EXPECT_EQ(4, LexIO::Seek(viewStream, 4, LexIO::Whence::start));
    EXPECT_EQ(8, LexIO::Seek(viewStream, 4, LexIO::Whence::current));

    EXPECT_EQ(BUFFER_LENGTH - 4, LexIO::Seek(viewStream, 4, LexIO::Whence::end));
    EXPECT_EQ(BUFFER_LENGTH - 8, LexIO::Seek(viewStream, -4, LexIO::Whence::current));
}

TEST(ConstViewStream, Seek_Negative)
{
    auto viewStream = GetConstViewStream();

    EXPECT_ANY_THROW(LexIO::Seek(viewStream, -1, LexIO::Whence::current));
}
