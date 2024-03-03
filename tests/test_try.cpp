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

#include "lexio/try.hpp"

#include "./test.h"
#include "lexio/serialize/int.hpp"

using PartialVectorStream = PartialStream<LexIO::VectorStream>;

struct ScopeClearError
{
    ScopeClearError() {}
    ~ScopeClearError() { LexIO::ClearLastError(); }
};

//******************************************************************************

TEST(Reader, TryRawRead)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRawRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, TryRawReadTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRawRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_EQ(out, 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Reader, TryRawReadError)
{
    auto onExit = ScopeClearError{};
    auto stream = ErrorStream{};

    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_FALSE(LexIO::TryRawRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_ANY_THROW(LexIO::ThrowLastError());
}

TEST(Reader, TryReadPtrLen)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, TryReadCharPtrLen)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    int8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, TryReadPtrLenPartial)
{
    auto stream = PartialVectorStream(GetVectorStream());

    size_t i = 0;
    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, TryReadPtrLenTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_EQ(out, 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Reader, TryReadPtrLenError)
{
    auto onExit = ScopeClearError{};
    auto stream = ErrorStream{};

    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_FALSE(LexIO::TryRead(out, &buffer[0], stream, sizeof(buffer)));
    EXPECT_ANY_THROW(LexIO::ThrowLastError());
}

TEST(Reader, TryReadArray)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRead(out, buffer, stream));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

//******************************************************************************

TEST(Writer, TryRawWrite)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRawWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(Writer, TryRawWriteTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryRawWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_EQ(out, 4);
    EXPECT_EQ(streamBuf[i++], 'X');
    EXPECT_EQ(streamBuf[i++], 'Y');
    EXPECT_EQ(streamBuf[i++], 'Z');
    EXPECT_EQ(streamBuf[i++], 'Z');
}

TEST(Writer, TryRawWriteError)
{
    auto onExit = ScopeClearError{};
    auto stream = ErrorStream{};

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_FALSE(LexIO::TryRawWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_ANY_THROW(LexIO::ThrowLastError());
}

TEST(Writer, TryWritePtrLen)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(Writer, TryWriteCharPtrLen)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    size_t i = 0;
    const int8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(Writer, TryWritePtrLenPartial)
{
    auto stream = PartialVectorStream{LexIO::VectorStream{}};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(cstream.Stream().Container()[i++], 'X');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
}

TEST(Writer, TryWritePtrLenTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_EQ(out, 4);
    EXPECT_EQ(streamBuf[i++], 'X');
    EXPECT_EQ(streamBuf[i++], 'Y');
    EXPECT_EQ(streamBuf[i++], 'Z');
    EXPECT_EQ(streamBuf[i++], 'Z');
}

TEST(Writer, TryWritePtrLenError)
{
    auto onExit = ScopeClearError{};
    auto stream = ErrorStream{};

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_FALSE(LexIO::TryWrite(out, stream, &data[0], sizeof(data)));
    EXPECT_ANY_THROW(LexIO::ThrowLastError());
}

TEST(Writer, TryWriteArray)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    size_t out = 0;
    EXPECT_TRUE(LexIO::TryWrite(out, stream, data));
    EXPECT_EQ(out, 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

//******************************************************************************

TEST(Seekable, TryRewind)
{
    LexIO::VectorStream stream = GetVectorStream();
    size_t out = 0;
    EXPECT_EQ(LexIO::Read8(stream), 'T');
    EXPECT_EQ(LexIO::Read8(stream), 'h');
    EXPECT_EQ(LexIO::Read8(stream), 'e');
    EXPECT_TRUE(LexIO::TryRewind(out, stream));
    EXPECT_EQ(out, 0);
}

TEST(Seekable, TrySeek_Tell)
{
    LexIO::VectorStream stream = GetVectorStream();
    size_t out = 0;

    LexIO::TrySeek(out, stream, 5);
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, 5);

    LexIO::TrySeek(out, stream, 5, LexIO::Whence::start);
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, 5);

    LexIO::TrySeek(out, stream, 5, LexIO::Whence::current);
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, 10);

    LexIO::TrySeek(out, stream, 5, LexIO::Whence::end);
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, TEST_TEXT_LENGTH - 5);
}

TEST(Seekable, TrySeek_Tell_SeekPos)
{
    LexIO::VectorStream stream = GetVectorStream();
    size_t out = 0;

    LexIO::TrySeek(out, stream, LexIO::SeekPos{5, LexIO::Whence::start});
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, 5);

    LexIO::TrySeek(out, stream, LexIO::SeekPos{5, LexIO::Whence::current});
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, 10);

    LexIO::TrySeek(out, stream, LexIO::SeekPos{5, LexIO::Whence::end});
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, TEST_TEXT_LENGTH - 5);
}

TEST(Seekable, TryTellAfterRewind)
{
    LexIO::VectorStream stream = GetVectorStream();
    size_t out = 0;

    LexIO::TrySeek(out, stream, 5, LexIO::Whence::start);
    EXPECT_TRUE(LexIO::TryRewind(out, stream));
    EXPECT_EQ(out, 0);
    EXPECT_TRUE(LexIO::TryTell(out, stream));
    EXPECT_EQ(out, 0);
}

TEST(Seekable, TryLength)
{
    LexIO::VectorStream stream = GetVectorStream();

    size_t out = 0;
    EXPECT_TRUE(LexIO::TryLength(out, stream));
    EXPECT_EQ(out, TEST_TEXT_LENGTH);
}
