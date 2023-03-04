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

#include "test_nonstd.h"

#include "lexio.h"

#include <catch2/catch_all.hpp>

//------------------------------------------------------------------------------

#pragma region "Read/Write Tests"

using VectorBuffer = LexIO::StdBuffer<std::vector<uint8_t>>;

TEST_CASE("Read UInt8", "[read]")
{
    VectorBuffer buffer = {0x88};
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Write UInt8", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU8(buffer, 0x88);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
}

TEST_CASE("Read U16LE", "[read]")
{
    VectorBuffer buffer({0x88, 0x99});
    REQUIRE(LexIO::ReadU16LE(buffer) == 0x9988);
}

TEST_CASE("Write UInt16LE", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU16LE(buffer, 0x9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
}

TEST_CASE("Read UInt32", "[read]")
{
    VectorBuffer buffer({0x88, 0x99, 0xaa, 0xbb});
    REQUIRE(LexIO::ReadU32LE(buffer) == 0xbbaa9988);
}

TEST_CASE("Write UInt32LE", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU32LE(buffer, 0xbbaa9988);

    LexIO::Seek(buffer, LexIO::WhenceStart(0));
    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
}

TEST_CASE("Read UInt64", "[read]")
{
    VectorBuffer buffer({0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    REQUIRE(LexIO::ReadU64LE(buffer) == 0xffeeddccbbaa9988);
}

TEST_CASE("Write UInt64LE", "[write]")
{
    VectorBuffer buffer;
    LexIO::WriteU64LE(buffer, 0xffeeddccbbaa9988);
    LexIO::Seek(buffer, LexIO::WhenceStart(0));

    REQUIRE(LexIO::ReadU8(buffer) == 0x88);
    REQUIRE(LexIO::ReadU8(buffer) == 0x99);
    REQUIRE(LexIO::ReadU8(buffer) == 0xaa);
    REQUIRE(LexIO::ReadU8(buffer) == 0xbb);
    REQUIRE(LexIO::ReadU8(buffer) == 0xcc);
    REQUIRE(LexIO::ReadU8(buffer) == 0xdd);
    REQUIRE(LexIO::ReadU8(buffer) == 0xee);
    REQUIRE(LexIO::ReadU8(buffer) == 0xff);
}

#pragma endregion

//------------------------------------------------------------------------------

#pragma region "Std Buffer Tests"

template <typename T>
void CommonTest(T &buf)
{
    // Test reading.
    LexIO::WriteU8(buf, 192);
    REQUIRE(LexIO::Tell(buf) == 1);
    LexIO::WriteU16LE(buf, 1993);
    REQUIRE(LexIO::Tell(buf) == 3);
    LexIO::WriteU32LE(buf, 20200602);
    REQUIRE(LexIO::Tell(buf) == 7);
    LexIO::WriteU64LE(buf, uint64_t(UINT32_MAX) + 20200613);
    REQUIRE(LexIO::Tell(buf) == 15);
    REQUIRE(LexIO::Length(buf) == 15);

    // Test writing.
    LexIO::Seek(buf, LexIO::WhenceStart(0));
    REQUIRE(LexIO::Tell(buf) == 0);
    REQUIRE(LexIO::ReadU8(buf) == 192);
    REQUIRE(LexIO::ReadU16LE(buf) == 1993);
    REQUIRE(LexIO::ReadU32LE(buf) == 20200602);
    REQUIRE(LexIO::ReadU64LE(buf) == uint64_t(UINT32_MAX) + 20200613);
    REQUIRE(LexIO::Tell(buf) == 15);

    // Test seeking.
    LexIO::Seek(buf, LexIO::WhenceStart(2));
    REQUIRE(LexIO::Tell(buf) == 2);
    LexIO::Seek(buf, LexIO::WhenceCurrent(2));
    REQUIRE(LexIO::Tell(buf) == 4);
    LexIO::Seek(buf, LexIO::WhenceCurrent(-2));
    REQUIRE(LexIO::Tell(buf) == 2);
    LexIO::Seek(buf, LexIO::WhenceEnd(2));
    REQUIRE(LexIO::Tell(buf) == 13);

    // Test overwriting data.
    LexIO::WriteU16LE(buf, 1993);
    REQUIRE(LexIO::Tell(buf) == 15);
    REQUIRE(LexIO::Length(buf) == 15);
    LexIO::Seek(buf, LexIO::WhenceCurrent(-2));
    REQUIRE(LexIO::Tell(buf) == 13);
    REQUIRE(LexIO::Length(buf) == 15);
    REQUIRE(LexIO::ReadU16LE(buf) == 1993);
}

TEST_CASE("LexIO::StdBuffer", "[stdbuf]")
{
    constexpr size_t SIZE = 15;

    SECTION("Test StdBuffer with std::vector")
    {
        VectorBuffer buf;
        CommonTest(buf);
        REQUIRE_NOTHROW(LexIO::WriteU8(buf, 0));
        REQUIRE(LexIO::Tell(buf) == SIZE + 1);
    }

    SECTION("Test StaticStdBuffer with std::array")
    {
        using ArrayBuffer = LexIO::StaticStdBuffer<std::array<uint8_t, SIZE>>;
        ArrayBuffer buf;
        CommonTest(buf);
        REQUIRE_THROWS(LexIO::WriteU8(buf, 0));
        REQUIRE(LexIO::Tell(buf) == SIZE);
    }

    SECTION("Test FixedStdBuffer with pre-allocated size")
    {
        using FixedVectorBuffer = LexIO::FixedStdBuffer<std::vector<uint8_t>>;
        FixedVectorBuffer buf(SIZE);
        CommonTest(buf);
        REQUIRE_THROWS(LexIO::WriteU8(buf, 0));
        REQUIRE(LexIO::Tell(buf) == SIZE);
    }
}

#pragma endregion

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int result = Catch::Session().run(argc, argv);
    return result;
}

//------------------------------------------------------------------------------

struct GoodReader
{
    size_t RawRead(LexIO::SpanT buffer)
    {
        (void)buffer;
        return 0;
    }
};

static_assert(LexIO::IsReader<GoodReader>::value, "GoodReader does not fulfill IsReader");
static_assert(LexIO::IsReaderV<GoodReader>, "GoodReader does not fulfill IsReaderV");

//------------------------------------------------------------------------------

struct GoodBufferedReader
{
    LexIO::ConstSpanT GetBuffer() const { return LexIO::ConstSpanT(); }
    LexIO::ConstSpanT FillBuffer(const size_t size)
    {
        (void)size;
        return LexIO::ConstSpanT();
    }
    void ConsumeBuffer(const size_t size) { (void)size; }
};

static_assert(LexIO::IsBufferedReader<GoodBufferedReader>::value,
              "GoodBufferedReader does not fulfill IsBufferedReader");
static_assert(LexIO::IsBufferedReaderV<GoodBufferedReader>, "GoodBufferedReader does not fulfill IsBufferedReaderV");

//------------------------------------------------------------------------------

struct GoodWriter
{
    size_t RawWrite(LexIO::ConstSpanT buffer)
    {
        (void)buffer;
        return 0;
    }
    void Flush() {}
};

static_assert(LexIO::IsWriter<GoodWriter>::value, "GoodWriter does not fulfill IsWriter");
static_assert(LexIO::IsWriterV<GoodWriter>, "GoodWriter does not fulfill IsWriterV");

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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
    void RawRead(LexIO::SpanT buffer) { (void)buffer; }
};

static_assert(!LexIO::IsReaderV<BadReaderBadReturn>, "BadReaderBadReturn incorrectly fulfills IsReaderV");
