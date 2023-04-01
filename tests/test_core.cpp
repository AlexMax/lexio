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

#include <iterator>

#include "./test.h"
#include "doctest.h"

//******************************************************************************

TEST_SUITE_BEGIN("utils");

struct GoodReader
{
    size_t RawRead(LexIO::ByteSpanT buffer)
    {
        (void)buffer;
        return 0;
    }
};

static_assert(LexIO::IsReader<GoodReader>::value, "GoodReader does not fulfill IsReader");
static_assert(LexIO::IsReaderV<GoodReader>, "GoodReader does not fulfill IsReaderV");

//******************************************************************************

struct GoodBufferedReader : public GoodReader
{
    size_t GetBufferSize() const { return 0; }
    LexIO::ConstByteSpanT FillBuffer(const size_t size)
    {
        (void)size;
        return LexIO::ConstByteSpanT();
    }
    void ConsumeBuffer(const size_t size) { (void)size; }
};

static_assert(LexIO::IsBufferedReader<GoodBufferedReader>::value,
              "GoodBufferedReader does not fulfill IsBufferedReader");
static_assert(LexIO::IsBufferedReaderV<GoodBufferedReader>, "GoodBufferedReader does not fulfill IsBufferedReaderV");

//******************************************************************************

struct GoodWriter
{
    size_t RawWrite(LexIO::ConstByteSpanT buffer)
    {
        (void)buffer;
        return 0;
    }
    void Flush() {}
};

static_assert(LexIO::IsWriter<GoodWriter>::value, "GoodWriter does not fulfill IsWriter");
static_assert(LexIO::IsWriterV<GoodWriter>, "GoodWriter does not fulfill IsWriterV");

//******************************************************************************

struct GoodSeekable
{
    size_t Seek(const LexIO::WhenceStart whence)
    {
        (void)whence;
        return 0;
    }
    size_t Seek(const LexIO::WhenceCurrent whence)
    {
        (void)whence;
        return 0;
    }
    size_t Seek(const LexIO::WhenceEnd whence)
    {
        (void)whence;
        return 0;
    }
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
    size_t RawRead(uint8_t *&buffer, const size_t size)
    {
        (void)buffer;
        (void)size;
        return 0;
    }
};

static_assert(!LexIO::IsReaderV<BadReaderBadParam>, "BadReaderBadParam incorrectly fulfills IsReaderV");

struct BadReaderBadReturn
{
    void RawRead(LexIO::ByteSpanT buffer) { (void)buffer; }
};

static_assert(!LexIO::IsReaderV<BadReaderBadReturn>, "BadReaderBadReturn incorrectly fulfills IsReaderV");

//******************************************************************************

template <typename T, std::size_t N>
constexpr std::size_t CountOf(T const (&)[N]) noexcept
{
    return N;
}

static LexIO::VectorStream GetBuffer()
{
    const uint8_t text[] = "The quick brown fox\njumps over the lazy dog.\n";
    LexIO::ConstByteSpanT textSpan{&text[0], CountOf(text) - 1};

    LexIO::VectorStream rvo;
    rvo.RawWrite(textSpan);
    rvo.Seek(LexIO::WhenceStart(0));
    return rvo;
}

//******************************************************************************

TEST_CASE("Test Read(span<char>)")
{
    LexIO::VectorStream buffer = GetBuffer();

    std::string data;
    data.resize(9);
    const size_t bytes = LexIO::Read({data.begin(), data.end()}, buffer);
    REQUIRE(bytes == 9);
    REQUIRE(data == "The quick");
}

TEST_CASE("Test Read(void, size)")
{
    LexIO::VectorStream buffer = GetBuffer();

    void *data = calloc(10, 1);
    const size_t bytes = LexIO::Read(data, 9, buffer);
    char *dataChar = static_cast<char *>(data);
    REQUIRE(bytes == 9);
    REQUIRE(strlen(dataChar) == 9);
    REQUIRE(!strcmp(dataChar, "The quick"));
    free(data);
}

TEST_CASE("Test Write(span<char>)")
{
    LexIO::VectorStream buffer;
    std::string data{"The quick"};
    std::array<uint8_t, 10> check;
    memset(check.data(), 0x00, check.size());

    size_t bytes = LexIO::Write(buffer, {data.begin(), data.end()});
    REQUIRE(bytes == 9);

    LexIO::Seek(buffer, LexIO::WhenceStart{0});
    bytes = LexIO::Read(check, buffer);
    REQUIRE(bytes == 9);

    char *checkChar = reinterpret_cast<char *>(check.data());
    REQUIRE(strcmp(checkChar, data.c_str()) == 0);
}

TEST_CASE("Test Write(void, size)")
{
    LexIO::VectorStream buffer;
    std::string data{"The quick"};
    const void *dataVoid = static_cast<const void *>(data.data());
    std::array<uint8_t, 10> check;
    memset(check.data(), 0x00, check.size());

    size_t bytes = LexIO::Write(buffer, dataVoid, data.length());
    REQUIRE(bytes == 9);

    LexIO::Seek(buffer, LexIO::WhenceStart{0});
    bytes = LexIO::Read(check, buffer);
    REQUIRE(bytes == 9);

    char *checkChar = reinterpret_cast<char *>(check.data());
    REQUIRE(strcmp(checkChar, data.c_str()) == 0);
}

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

TEST_SUITE_END();
