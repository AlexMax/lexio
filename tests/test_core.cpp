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

//******************************************************************************

struct GoodReader
{
    size_t LexRead(LexIO::ByteSpanT) { return 0; }
};

static_assert(LexIO::IsReader<GoodReader>::value, "GoodReader does not fulfill IsReader");
static_assert(LexIO::IsReaderV<GoodReader>, "GoodReader does not fulfill IsReaderV");

//******************************************************************************

struct GoodBufferedReader : public GoodReader
{
    size_t LexGetBufferSize() const { return 0; }
    LexIO::ConstByteSpanT LexFillBuffer(const size_t) { return LexIO::ConstByteSpanT(); }
    void LexConsumeBuffer(const size_t size) { (void)size; }
};

static_assert(LexIO::IsBufferedReader<GoodBufferedReader>::value,
              "GoodBufferedReader does not fulfill IsBufferedReader");
static_assert(LexIO::IsBufferedReaderV<GoodBufferedReader>, "GoodBufferedReader does not fulfill IsBufferedReaderV");

//******************************************************************************

struct GoodWriter
{
    size_t LexWrite(LexIO::ConstByteSpanT) { return 0; }
    void LexFlush() {}
};

static_assert(LexIO::IsWriter<GoodWriter>::value, "GoodWriter does not fulfill IsWriter");
static_assert(LexIO::IsWriterV<GoodWriter>, "GoodWriter does not fulfill IsWriterV");

//******************************************************************************

struct GoodSeekable
{
    size_t LexSeek(const LexIO::SeekPos) { return 0; }
};

static_assert(LexIO::IsSeekable<GoodSeekable>::value, "GoodSeekable does not fulfill IsSeekable");
static_assert(LexIO::IsSeekableV<GoodSeekable>, "GoodSeekable does not fulfill IsSeekableV");

//******************************************************************************

struct BadReaderMissingClass
{
};

static_assert(!LexIO::IsReaderV<BadReaderMissingClass>, "BadReaderMissingClass incorrectly fulfills IsReaderV");

struct BadReaderBadParam
{
    size_t LexRead(uint8_t *&buffer, const size_t size)
    {
        (void)buffer;
        (void)size;
        return 0;
    }
};

static_assert(!LexIO::IsReaderV<BadReaderBadParam>, "BadReaderBadParam incorrectly fulfills IsReaderV");

struct BadReaderBadReturn
{
    void LexRead(LexIO::ByteSpanT buffer) { (void)buffer; }
};

static_assert(!LexIO::IsReaderV<BadReaderBadReturn>, "BadReaderBadReturn incorrectly fulfills IsReaderV");

//******************************************************************************

template <typename T, std::size_t N>
constexpr std::size_t CountOf(T const (&)[N]) noexcept
{
    return N;
}

constexpr uint8_t BUFFER_TEXT[] = "The quick brown fox\njumps over the lazy dog.\n";
constexpr size_t BUFFER_LENGTH = CountOf(BUFFER_TEXT) - sizeof('\0');

static LexIO::VectorStream GetBuffer()
{
    LexIO::ConstByteSpanT textSpan{&BUFFER_TEXT[0], BUFFER_LENGTH};

    LexIO::VectorStream rvo;
    rvo.LexWrite(textSpan);
    rvo.LexSeek(LexIO::SeekPos(0, LexIO::seek::start));
    return rvo;
}

//******************************************************************************

TEST_CASE("Test ReadAll")
{
    LexIO::VectorStream basic = GetBuffer();
    auto buffer = LexIO::VectorBufReader<LexIO::VectorStream>::FromReader(std::move(basic));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadAll(std::back_inserter(data), buffer);
    REQUIRE(bytes == 45);
    REQUIRE(data.size() == 45);
    REQUIRE(*(data.end() - 1) == '\n');
}

TEST_CASE("Test ReadAll with a small buffer")
{
    LexIO::VectorStream basic = GetBuffer();
    auto buffer = LexIO::VectorBufReader<LexIO::VectorStream>::FromReader(std::move(basic));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadAll(std::back_inserter(data), buffer);
    REQUIRE(data[4] == 'q'); // Check the buffer boundary.
    REQUIRE(data[8] == 'k');
    REQUIRE(bytes == 45);
    REQUIRE(data.size() == 45);
}

TEST_CASE("Test ReadUntil")
{
    LexIO::VectorStream basic = GetBuffer();
    auto buffer = LexIO::VectorBufReader<LexIO::VectorStream>::FromReader(std::move(basic));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    REQUIRE(bytes == 20);
    REQUIRE(data.size() == 20);
    REQUIRE(*(data.end() - 1) == '\n');
}

TEST_CASE("Test Rewind")
{
    LexIO::VectorStream basic = GetBuffer();
    REQUIRE(LexIO::Read8(basic) == 'T');
    REQUIRE(LexIO::Read8(basic) == 'h');
    REQUIRE(LexIO::Read8(basic) == 'e');
    REQUIRE(LexIO::Rewind(basic) == 0);
    REQUIRE(LexIO::Tell(basic) == 0);
}

TEST_CASE("Test Seek/Tell")
{
    LexIO::VectorStream basic = GetBuffer();

    LexIO::Seek(basic, 5, LexIO::seek::start);
    REQUIRE(LexIO::Tell(basic) == 5);

    LexIO::Seek(basic, 5, LexIO::seek::current);
    REQUIRE(LexIO::Tell(basic) == 10);

    LexIO::Seek(basic, 5, LexIO::seek::end);
    REQUIRE(LexIO::Tell(basic) == BUFFER_LENGTH - 5);
}

TEST_CASE("Test Seek/Tell with SeekPos")
{
    LexIO::VectorStream basic = GetBuffer();

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::seek::start));
    REQUIRE(LexIO::Tell(basic) == 5);

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::seek::current));
    REQUIRE(LexIO::Tell(basic) == 10);

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::seek::end));
    REQUIRE(LexIO::Tell(basic) == BUFFER_LENGTH - 5);
}

TEST_CASE("Test Tell after Rewind")
{
    LexIO::VectorStream basic = GetBuffer();

    LexIO::Seek(basic, 5, LexIO::seek::start);
    REQUIRE(LexIO::Rewind(basic) == 0);
    REQUIRE(LexIO::Tell(basic) == 0);
}

TEST_CASE("Test Length")
{
    LexIO::VectorStream basic = GetBuffer();

    REQUIRE(LexIO::Length(basic) == BUFFER_LENGTH);
}
