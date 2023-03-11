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

using LFileBufReader = LexIO::StdBufReader<LexIO::LFile>;

static_assert(LexIO::IsBufferedReaderV<LFileBufReader>, "LFileBufReader does not fulfill IsBufferedReaderV");

TEST_CASE("Test ReadAll on a standard StdBufReader", "[buf]")
{
    auto file = LexIO::LOpen("../test/test_file.txt", LexIO::LOpenMode::read);
    auto buffer = LFileBufReader::FromReader(std::move(file));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadAll(std::back_inserter(data), buffer);
    REQUIRE((bytes == 45 || bytes == 47)); // Newlines are different sizes.
    REQUIRE((data.size() == 45 || data.size() == 47));
    REQUIRE(*(data.end() - 1) == '\n');
}

TEST_CASE("Test ReadAll on a small StdBufReader", "[buf]")
{
    auto file = LexIO::LOpen("../test/test_file.txt", LexIO::LOpenMode::read);
    auto buffer = LFileBufReader::FromReader(std::move(file), 4);

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadAll(std::back_inserter(data), buffer);
    REQUIRE(data[4] == 'q'); // Check the buffer boundary.
    REQUIRE(data[8] == 'k');
    REQUIRE((bytes == 45 || bytes == 47)); // Newlines are different sizes.
    REQUIRE((data.size() == 45 || data.size() == 47));
}

TEST_CASE("Test ReadUntil on a StdBufReader", "[buf]")
{
    auto file = LexIO::LOpen("../test/test_file.txt", LexIO::LOpenMode::read);
    auto buffer = LFileBufReader::FromReader(std::move(file));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    REQUIRE((bytes == 20 || bytes == 21)); // Newlines are different sizes.
    REQUIRE((data.size() == 20 || data.size() == 21));
    REQUIRE(*(data.end() - 1) == '\n');
}
