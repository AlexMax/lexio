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

#include "lexio/ref.hpp"

#include "./test.h"

#include <memory>

static void AcceptReader(LexIO::ReaderRef) {}
static void AcceptBufferedReader(LexIO::BufferedReaderRef ref)
{
    AcceptReader(ref);
}
static void AcceptUnbufferedReader(LexIO::UnbufferedReaderRef ref)
{
    AcceptReader(ref);
}
static void AcceptWriter(LexIO::WriterRef) {}
static void AcceptSeekable(LexIO::SeekableRef) {}
static void AcceptReaderSeekable(LexIO::ReaderSeekableRef) {}

static void AmbiguousReader(LexIO::UnbufferedReaderRef) {}
static void AmbiguousReader(LexIO::BufferedReaderRef) {}

//******************************************************************************

TEST(Ref, ReaderRef)
{
    struct GoodR
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodR{};
    LexIO::ReaderRef ref(test);
    LexIO::ReaderRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);

    AcceptReader(ref);
}

TEST(Ref, BufferedReaderRef)
{
    struct GoodBR
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        LexIO::BufferView LexFillBuffer(const size_t) { return LexIO::BufferView{}; }
        void LexConsumeBuffer(const size_t) {}
    };

    uint8_t buffer[4];
    auto test = GoodBR{};
    LexIO::BufferedReaderRef ref(test);
    LexIO::BufferedReaderRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(LexIO::FillBuffer(ref, 0).second, 0);
    LexIO::ConsumeBuffer(ref, 0);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
    AmbiguousReader(test);
}

TEST(Ref, UnbufferedReaderRef)
{
    struct GoodR
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodR{};
    LexIO::UnbufferedReaderRef ref(test);
    LexIO::UnbufferedReaderRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);

    AcceptReader(ref);
    AcceptUnbufferedReader(ref);
    AmbiguousReader(test);
}

TEST(Ref, WriterRef)
{
    struct GoodW
    {
        size_t LexWrite(const uint8_t *, const size_t) { return 0; }
        void LexFlush() {}
    };

    uint8_t buffer[4];
    auto test = GoodW{};
    LexIO::WriterRef ref(test);
    LexIO::WriterRef copy(ref);

    EXPECT_EQ(LexIO::RawWrite(ref, &buffer[0], sizeof(buffer)), 0);
    LexIO::Flush(ref);

    AcceptWriter(ref);
}

TEST(Ref, SeekableRef)
{
    struct GoodS
    {
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    auto test = GoodS{};
    LexIO::SeekableRef ref(test);
    LexIO::SeekableRef copy(ref);

    EXPECT_EQ(LexIO::Seek(ref, LexIO::SeekPos{}), 0);

    AcceptSeekable(ref);
}

TEST(Ref, ReaderWriterRef)
{
    struct GoodRW
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        size_t LexWrite(const uint8_t *, const size_t) { return 0; }
        void LexFlush() {}
    };

    uint8_t buffer[4];
    auto test = GoodRW{};
    LexIO::ReaderWriterRef ref(test);
    LexIO::ReaderWriterRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(LexIO::RawWrite(ref, &buffer[0], sizeof(buffer)), 0);
    LexIO::Flush(ref);

    AcceptReader(ref);
    AcceptWriter(ref);
}

TEST(Ref, BufferedReaderWriterRef)
{
    struct GoodBRW
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        LexIO::BufferView LexFillBuffer(const size_t) { return LexIO::BufferView{}; }
        void LexConsumeBuffer(const size_t) {}
        size_t LexWrite(const uint8_t *, const size_t) { return 0; }
        void LexFlush() {}
    };

    uint8_t buffer[4];
    auto test = GoodBRW{};
    LexIO::BufferedReaderWriterRef ref(test);
    LexIO::BufferedReaderWriterRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(LexIO::FillBuffer(ref, 0).second, 0);
    LexIO::ConsumeBuffer(ref, 0);
    EXPECT_EQ(LexIO::RawWrite(ref, &buffer[0], sizeof(buffer)), 0);
    LexIO::Flush(ref);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
    AcceptWriter(ref);
}

TEST(Ref, ReaderSeekableRef)
{
    struct GoodRS
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodRS{};
    LexIO::ReaderSeekableRef ref(test);
    LexIO::ReaderSeekableRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(LexIO::Seek(ref, LexIO::SeekPos{}), 0);

    AcceptReader(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}

TEST(Ref, BufferedReaderSeekableRef)
{
    struct GoodBRS
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        LexIO::BufferView LexFillBuffer(const size_t) { return LexIO::BufferView{}; }
        void LexConsumeBuffer(const size_t) {}
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodBRS{};
    LexIO::BufferedReaderSeekableRef ref(test);
    LexIO::BufferedReaderSeekableRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(LexIO::FillBuffer(ref, 0).second, 0);
    LexIO::ConsumeBuffer(ref, 0);
    EXPECT_EQ(LexIO::Seek(ref, LexIO::SeekPos{}), 0);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}

TEST(Ref, ReaderWriterSeekableRef)
{
    struct GoodRWS
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        size_t LexWrite(const uint8_t *, const size_t) { return 0; }
        void LexFlush() {}
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodRWS{};
    LexIO::ReaderWriterSeekableRef ref(test);
    LexIO::ReaderWriterSeekableRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(LexIO::RawWrite(ref, &buffer[0], sizeof(buffer)), 0);
    LexIO::Flush(ref);
    EXPECT_EQ(LexIO::Seek(ref, LexIO::SeekPos{}), 0);

    AcceptReader(ref);
    AcceptWriter(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}

TEST(Ref, BufferedReaderWriterSeekableRef)
{
    struct GoodBRWS
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        LexIO::BufferView LexFillBuffer(const size_t) { return LexIO::BufferView{}; }
        void LexConsumeBuffer(const size_t) {}
        size_t LexWrite(const uint8_t *, const size_t) { return 0; }
        void LexFlush() {}
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodBRWS{};
    LexIO::BufferedReaderWriterSeekableRef ref(test);
    LexIO::BufferedReaderWriterSeekableRef copy(ref);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(LexIO::FillBuffer(ref, 0).second, 0);
    LexIO::ConsumeBuffer(ref, 0);
    EXPECT_EQ(LexIO::RawWrite(ref, &buffer[0], sizeof(buffer)), 0);
    LexIO::Flush(ref);
    EXPECT_EQ(LexIO::Seek(ref, LexIO::SeekPos{}), 0);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
    AcceptWriter(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}
