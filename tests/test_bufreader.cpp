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

static_assert(LexIO::IsBufferedReaderV<VectorBufReader>, "VectorBufReader is not a BufferedReader");
static_assert(LexIO::IsWriterV<VectorBufReader>, "VectorBufReader is not a Writer");
static_assert(LexIO::IsSeekableV<VectorBufReader>, "VectorBufReader is not a Seekable");

TEST_CASE("FillBuffer, single call")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    auto test = LexIO::FillBuffer(bufReader, 8);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[7] == 'c');
    REQUIRE(test.second == 8);
}

TEST_CASE("FillBuffer, multiple calls")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Buffer initial four bytes.
    auto test = LexIO::FillBuffer(bufReader, 4);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[3] == ' ');
    REQUIRE(test.second == 4);

    // Buffer less than what we had before, should read nothing.
    test = LexIO::FillBuffer(bufReader, 2);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[3] == ' ');
    REQUIRE(test.second == 4);

    // Buffer more than what we had before.
    test = LexIO::FillBuffer(bufReader, 8);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[3] == ' ');
    REQUIRE(test.first[4] == 'q');
    REQUIRE(test.first[7] == 'c');
    REQUIRE(test.second == 8);
}

TEST_CASE("FillBuffer, EOF")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Buffer everything.
    auto test = LexIO::FillBuffer(bufReader, 64);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[BUFFER_LENGTH - 1] == '\n');
    REQUIRE(test.second == BUFFER_LENGTH);

    // Buffer more than everything.
    test = LexIO::FillBuffer(bufReader, 96);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[BUFFER_LENGTH - 1] == '\n');
    REQUIRE(test.second == BUFFER_LENGTH);
}

TEST_CASE("FillBuffer, EOF with initial buffer")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    auto test = LexIO::FillBuffer(bufReader, 4);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[3] == ' ');
    REQUIRE(test.second == 4);

    // Buffer everything.
    test = LexIO::FillBuffer(bufReader, 64);
    REQUIRE(test.first[0] == 'T');
    REQUIRE(test.first[3] == ' ');
    REQUIRE(test.first[4] == 'q');
    REQUIRE(test.first[BUFFER_LENGTH - 1] == '\n');
    REQUIRE(test.second == BUFFER_LENGTH);
}

TEST_CASE("FillBuffer, zero sized read")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    auto test = LexIO::FillBuffer(bufReader, 0);
    REQUIRE(test.second == 0);
}

TEST_CASE("ConsumeBuffer, single call")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Fill, then consume whole buffer.
    LexIO::FillBuffer(bufReader, 8);
    REQUIRE_NOTHROW(LexIO::ConsumeBuffer(bufReader, 8));

    auto test = LexIO::GetBuffer(bufReader);
    REQUIRE(test.second == 0);

    // Subsequent read should pick up where we left off.
    test = LexIO::FillBuffer(bufReader, 8);
    REQUIRE(test.first[0] == 'k');
    REQUIRE(test.first[7] == ' ');
    REQUIRE(test.second == 8);
}

TEST_CASE("ConsumeBuffer, multiple calls")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Consume half the buffer.
    LexIO::FillBuffer(bufReader, 8);
    REQUIRE_NOTHROW(LexIO::ConsumeBuffer(bufReader, 4));
    auto test = LexIO::GetBuffer(bufReader);
    REQUIRE(test.first[0] == 'q');
    REQUIRE(test.first[3] == 'c');
    REQUIRE(test.second == 4);

    // Consume the other half of the buffer.
    REQUIRE_NOTHROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    REQUIRE(test.second == 0);
}

TEST_CASE("ConsumeBuffer, EOF")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Fill to EOF, consume part of it.
    auto test = LexIO::FillBuffer(bufReader, 64);
    REQUIRE_NOTHROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    REQUIRE(test.first[0] == 'q');
    REQUIRE(test.first[3] == 'c');
    REQUIRE(test.second == BUFFER_LENGTH - 4);

    // Consume the rest of it.
    REQUIRE_NOTHROW(LexIO::ConsumeBuffer(bufReader, BUFFER_LENGTH - 4));
    test = LexIO::GetBuffer(bufReader);
    REQUIRE(test.second == 0);
}

TEST_CASE("Copy constructor/Copy assignment")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));
    auto bufTest = LexIO::FillBuffer(bufReader, 8);
    VectorBufReader copyReader{bufReader};
    auto copyTest = LexIO::FillBuffer(copyReader, 8);

    REQUIRE(copyTest.first[0] == bufTest.first[0]);
    REQUIRE(copyTest.first[7] == bufTest.first[7]);
    REQUIRE(copyTest.second == bufTest.second);

    LexIO::FillBuffer(copyReader, 12);
    copyReader = bufReader;
    copyTest = LexIO::FillBuffer(copyReader, 8);

    REQUIRE(copyTest.first[0] == bufTest.first[0]);
    REQUIRE(copyTest.first[7] == bufTest.first[7]);
    REQUIRE(copyTest.second == bufTest.second);
}

TEST_CASE("Move constructor")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));
    LexIO::FillBuffer(bufReader, 8);
    VectorBufReader moveReader{std::move(bufReader)};
    auto moveTest = LexIO::FillBuffer(moveReader, 8);

    REQUIRE(moveTest.first[0] == 'T');
    REQUIRE(moveTest.first[7] == 'c');
    REQUIRE(moveTest.second == 8);
}

TEST_CASE("Move assignment")
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));
    auto moveReader = bufReader;

    LexIO::FillBuffer(bufReader, 8);
    moveReader = std::move(bufReader);
    auto moveTest = LexIO::FillBuffer(moveReader, 8);

    REQUIRE(moveTest.first[0] == 'T');
    REQUIRE(moveTest.first[7] == 'c');
    REQUIRE(moveTest.second == 8);
}
