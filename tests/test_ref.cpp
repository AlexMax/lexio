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

#include "./test.h"
#include "catch2/catch_all.hpp"

#include <memory>

static void AcceptReader(const LexIO::ReaderRef &) {}
static void AcceptBufferedReader(const LexIO::BufferedReaderRef &) {}
static void AcceptWriter(const LexIO::WriterRef &) {}
static void AcceptSeekable(const LexIO::SeekableRef &) {}
static void AcceptReaderSeekable(const LexIO::ReaderSeekableRef &) {}

//******************************************************************************

TEST_CASE("Test LexIO::ReaderRef")
{
    struct GoodR
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodR{};
    LexIO::ReaderRef ref(test);

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);

    AcceptReader(ref);
}

TEST_CASE("Test LexIO::BufferedReaderRef")
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

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);
    CHECK(LexIO::FillBuffer(ref, 0).second == 0);
    LexIO::ConsumeBuffer(ref, 0);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
}

TEST_CASE("Test LexIO::WriterRef")
{
    struct GoodW
    {
        size_t LexWrite(const uint8_t *, const size_t) { return 0; }
        void LexFlush() {}
    };

    uint8_t buffer[4];
    auto test = GoodW{};
    LexIO::WriterRef ref(test);

    CHECK(LexIO::Write(ref, &buffer[0], sizeof(buffer)) == 0);
    LexIO::Flush(ref);

    AcceptWriter(ref);
}

TEST_CASE("Test LexIO::ReaderWriterRef")
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

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);
    CHECK(LexIO::Write(ref, &buffer[0], sizeof(buffer)) == 0);
    LexIO::Flush(ref);

    AcceptReader(ref);
    AcceptWriter(ref);
}

TEST_CASE("Test LexIO::BufferedReaderWriterRef")
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

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);
    CHECK(LexIO::FillBuffer(ref, 0).second == 0);
    LexIO::ConsumeBuffer(ref, 0);
    CHECK(LexIO::Write(ref, &buffer[0], sizeof(buffer)) == 0);
    LexIO::Flush(ref);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
    AcceptWriter(ref);
}

TEST_CASE("Test LexIO::ReaderSeekableRef")
{
    struct GoodRS
    {
        size_t LexRead(uint8_t *, const size_t) { return 0; }
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    uint8_t buffer[4];
    auto test = GoodRS{};
    LexIO::ReaderSeekableRef ref(test);

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);
    CHECK(LexIO::Seek(ref, LexIO::SeekPos{}) == 0);

    AcceptReader(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}

TEST_CASE("Test LexIO::BufferedReaderSeekableRef")
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

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);
    CHECK(LexIO::FillBuffer(ref, 0).second == 0);
    LexIO::ConsumeBuffer(ref, 0);
    CHECK(LexIO::Seek(ref, LexIO::SeekPos{}) == 0);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}

TEST_CASE("Test LexIO::ReaderWriterSeekableRef")
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

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);
    CHECK(LexIO::Write(ref, &buffer[0], sizeof(buffer)) == 0);
    LexIO::Flush(ref);
    CHECK(LexIO::Seek(ref, LexIO::SeekPos{}) == 0);

    AcceptReader(ref);
    AcceptWriter(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}

TEST_CASE("Test LexIO::BufferedReaderWriterSeekableRef")
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

    CHECK(LexIO::Read(&buffer[0], sizeof(buffer), ref) == 0);
    CHECK(LexIO::FillBuffer(ref, 0).second == 0);
    LexIO::ConsumeBuffer(ref, 0);
    CHECK(LexIO::Write(ref, &buffer[0], sizeof(buffer)) == 0);
    LexIO::Flush(ref);
    CHECK(LexIO::Seek(ref, LexIO::SeekPos{}) == 0);

    AcceptReader(ref);
    AcceptBufferedReader(ref);
    AcceptWriter(ref);
    AcceptSeekable(ref);
    AcceptReaderSeekable(ref);
}
