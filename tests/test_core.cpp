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
    size_t LexRead(uint8_t *, const size_t) { return 0; }
};

TEST_CASE("GoodReader must fulfill IsReader")
{
    REQUIRE(LexIO::IsReader<GoodReader>::value);
}

TEST_CASE("GoodReader must fulfill IsReaderV")
{
    REQUIRE(LexIO::IsReaderV<GoodReader>);
}

//******************************************************************************

struct GoodBufferedReader : public GoodReader
{
    LexIO::BufferView LexFillBuffer(const size_t) { return LexIO::BufferView(); }
    void LexConsumeBuffer(const size_t size) { (void)size; }
};

TEST_CASE("GoodBufferedReader must fulfill IsBufferedReader")
{
    REQUIRE(LexIO::IsBufferedReader<GoodBufferedReader>::value);
}

TEST_CASE("GoodBufferedReader must fulfill IsBufferedReaderV")
{
    REQUIRE(LexIO::IsBufferedReaderV<GoodBufferedReader>);
}

//******************************************************************************

struct GoodWriter
{
    size_t LexWrite(const uint8_t *, const size_t) { return 0; }
    void LexFlush() {}
};

TEST_CASE("GoodWriter must fulfill IsWriter")
{
    REQUIRE(LexIO::IsWriter<GoodWriter>::value);
}

TEST_CASE("GoodWriter must fulfill IsWriterV")
{
    REQUIRE(LexIO::IsWriterV<GoodWriter>);
}

//******************************************************************************

struct GoodSeekable
{
    size_t LexSeek(const LexIO::SeekPos) { return 0; }
};

TEST_CASE("GoodSeekable must fulfill IsSeekable")
{
    REQUIRE(LexIO::IsSeekable<GoodSeekable>::value);
}

TEST_CASE("GoodSeekable must fulfill IsSeekableV")
{
    REQUIRE(LexIO::IsSeekableV<GoodSeekable>);
}

//******************************************************************************

struct BadReaderMissingClass
{
};

TEST_CASE("BadReaderMissingClass must not fulfill IsReaderV")
{
    REQUIRE_FALSE(LexIO::IsReaderV<BadReaderMissingClass>);
}

struct BadReaderBadParam
{
    size_t LexRead(uint8_t *&, const size_t) { return 0; }
};

TEST_CASE("BadReaderBadParam must not fulfill IsReaderV")
{
    REQUIRE_FALSE(LexIO::IsReaderV<BadReaderBadParam>);
}

struct BadReaderBadReturn
{
    void LexRead(uint8_t *, const size_t) {}
};

TEST_CASE("BadReaderBadReturn must not fulfill IsReaderV")
{
    REQUIRE_FALSE(LexIO::IsReaderV<BadReaderBadReturn>);
}

//******************************************************************************

TEST_CASE("Test Read with ptr/len")
{
    LexIO::VectorStream basic = GetStream();

    uint8_t buffer[5] = {0};
    REQUIRE(LexIO::Read(&buffer[0], sizeof(buffer), basic) == 5);
}

TEST_CASE("Test Read with array")
{
    LexIO::VectorStream basic = GetStream();

    uint8_t buffer[5] = {0};
    REQUIRE(LexIO::Read(buffer, basic) == 5);
}

TEST_CASE("Test Read with iterator pair")
{
    LexIO::VectorStream basic = GetStream();

    uint8_t buffer[5] = {0};
    REQUIRE(LexIO::Read(&buffer[0], &buffer[5], basic) == 5);
}

TEST_CASE("Test ReadAll")
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadAll(std::back_inserter(data), buffer);
    REQUIRE(bytes == 45);
    REQUIRE(data.size() == 45);
    REQUIRE(*(data.end() - 1) == '\n');
}

TEST_CASE("Test ReadAll with a small buffer")
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadAll(std::back_inserter(data), buffer, 4);
    REQUIRE(data[4] == 'q'); // Check the buffer boundary.
    REQUIRE(data[8] == 'k');
    REQUIRE(bytes == 45);
    REQUIRE(data.size() == 45);
}

TEST_CASE("Test ReadUntil")
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    size_t bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    REQUIRE(bytes == 20);
    REQUIRE(data.size() == 20);
    REQUIRE(*(data.end() - 1) == '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    REQUIRE(bytes == 25);
    REQUIRE(data.size() == 45);
    REQUIRE(*(data.end() - 1) == '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    REQUIRE(bytes == 0);
    REQUIRE(data.size() == 45);
}

TEST_CASE("Test ReadUntil with a small buffer")
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    size_t bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n', 4);
    REQUIRE(bytes == 20);
    REQUIRE(data.size() == 20);
    REQUIRE(*(data.end() - 1) == '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n', 4);
    REQUIRE(bytes == 25);
    REQUIRE(data.size() == 45);
    REQUIRE(*(data.end() - 1) == '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n', 4);
    REQUIRE(bytes == 0);
    REQUIRE(data.size() == 45);
}

TEST_CASE("Test Write with ptr/len")
{
    LexIO::VectorStream basic = GetStream();

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    REQUIRE(LexIO::Write(basic, &data[0], sizeof(data)) == 5);
}

TEST_CASE("Test Write with array")
{
    LexIO::VectorStream basic = GetStream();

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    REQUIRE(LexIO::Write(basic, data) == 5);
}

TEST_CASE("Test Write with iterator pair")
{
    LexIO::VectorStream basic = GetStream();

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    REQUIRE(LexIO::Write(basic, &data[0], &data[5]) == 5);
}

TEST_CASE("Test Rewind")
{
    LexIO::VectorStream basic = GetStream();
    REQUIRE(LexIO::Read8(basic) == 'T');
    REQUIRE(LexIO::Read8(basic) == 'h');
    REQUIRE(LexIO::Read8(basic) == 'e');
    REQUIRE(LexIO::Rewind(basic) == 0);
    REQUIRE(LexIO::Tell(basic) == 0);
}

TEST_CASE("Test Seek/Tell")
{
    LexIO::VectorStream basic = GetStream();

    LexIO::Seek(basic, 5, LexIO::Whence::start);
    REQUIRE(LexIO::Tell(basic) == 5);

    LexIO::Seek(basic, 5, LexIO::Whence::current);
    REQUIRE(LexIO::Tell(basic) == 10);

    LexIO::Seek(basic, 5, LexIO::Whence::end);
    REQUIRE(LexIO::Tell(basic) == BUFFER_LENGTH - 5);
}

TEST_CASE("Test Seek/Tell with SeekPos")
{
    LexIO::VectorStream basic = GetStream();

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::Whence::start));
    REQUIRE(LexIO::Tell(basic) == 5);

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::Whence::current));
    REQUIRE(LexIO::Tell(basic) == 10);

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::Whence::end));
    REQUIRE(LexIO::Tell(basic) == BUFFER_LENGTH - 5);
}

TEST_CASE("Test Tell after Rewind")
{
    LexIO::VectorStream basic = GetStream();

    LexIO::Seek(basic, 5, LexIO::Whence::start);
    REQUIRE(LexIO::Rewind(basic) == 0);
    REQUIRE(LexIO::Tell(basic) == 0);
}

TEST_CASE("Test Length")
{
    LexIO::VectorStream basic = GetStream();

    REQUIRE(LexIO::Length(basic) == BUFFER_LENGTH);
}
