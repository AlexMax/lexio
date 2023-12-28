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

using VectorBufWriter = LexIO::FixedBufWriter<LexIO::VectorStream>;

//******************************************************************************

TEST_CASE("VectorBufWriter must fulfill Reader")
{
    REQUIRE(LexIO::IsReaderV<VectorBufWriter>);
}

TEST_CASE("VectorBufWriter must fulfill Writer")
{
    REQUIRE(LexIO::IsWriterV<VectorBufWriter>);
}

TEST_CASE("VectorBufWriter must fulfill Seekable")
{
    REQUIRE(LexIO::IsSeekableV<VectorBufWriter>);
}

TEST_CASE("Write")
{
    auto stream = LexIO::VectorStream{};

    auto bufWriter = VectorBufWriter{std::move(stream)};
    REQUIRE(::BUFFER_LENGTH == LexIO::Write(bufWriter, ::BUFFER_TEXT, ::BUFFER_LENGTH));
    LexIO::Flush(bufWriter);

    stream = std::move(bufWriter).Writer();
    REQUIRE(::BUFFER_LENGTH == LexIO::Length(stream));
}

TEST_CASE("Write, small buffer")
{
    auto stream = LexIO::VectorStream{};

    auto bufWriter = VectorBufWriter{std::move(stream), 16};
    REQUIRE(::BUFFER_LENGTH == LexIO::Write(bufWriter, ::BUFFER_TEXT, ::BUFFER_LENGTH));
    LexIO::Flush(bufWriter);

    stream = std::move(bufWriter).Writer();
    REQUIRE(::BUFFER_LENGTH == LexIO::Length(stream));
}
