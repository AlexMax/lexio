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

#if 0

//******************************************************************************

template <typename T, std::size_t N>
constexpr std::size_t CountOf(T const (&)[N]) noexcept
{
    return N;
}

static LexIO::VectorStream GetBuffer()
{
    const uint8_t text[] = "The quick brown fox\njumps over the lazy dog.\n";

    LexIO::VectorStream rvo;
    rvo.LexWrite(&text[0], CountOf(text) - 1);
    rvo.LexSeek(LexIO::SeekPos(0, LexIO::seek::start));
    return rvo;
}

TEST_CASE("ReadBytes")
{
    LexIO::VectorStream buffer = GetBuffer();

    std::array<uint8_t, 10> data;
    data.fill(0x00);

    REQUIRE_NOTHROW(LexIO::ReadBytes(data.begin(), data.end() - 1, buffer));
    char *dataChar = LexIO::Detail::BitCast<char *>(&(*data.begin()));
    REQUIRE(strlen(dataChar) == 9);
    REQUIRE(!strcmp(dataChar, "The quick"));
}

TEST_CASE("WriteBytes")
{
    LexIO::VectorStream buffer;
    std::string src{"The quick"};
    std::array<uint8_t, 10> data;
    data.fill(0x00);
    std::memcpy(data.data(), src.data(), src.size());

    REQUIRE_NOTHROW(LexIO::WriteBytes(buffer, data.begin(), data.end()));

    std::array<uint8_t, 10> check;
    check.fill(0x00);
    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read(check, buffer) == 10);
    REQUIRE(memcmp(check.data(), data.data(), check.size()) == 0);
}

TEST_CASE("ReadData")
{
    LexIO::VectorStream buffer = GetBuffer();

    void *data = calloc(10, 1);
    REQUIRE_NOTHROW(LexIO::ReadData(data, 9, buffer));
    char *dataChar = static_cast<char *>(data);
    REQUIRE(strlen(dataChar) == 9);
    REQUIRE(!strcmp(dataChar, "The quick"));
    free(data);
}

TEST_CASE("WriteData")
{
    LexIO::VectorStream buffer;
    std::string data{"The quick"};
    const void *dataVoid = static_cast<const void *>(data.data());
    std::array<uint8_t, 10> check;
    memset(check.data(), 0x00, check.size());

    REQUIRE_NOTHROW(LexIO::WriteData(buffer, dataVoid, data.length()));

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read(check, buffer) == 9);

    char *checkChar = reinterpret_cast<char *>(check.data());
    REQUIRE(strcmp(checkChar, data.c_str()) == 0);
}

TEST_CASE("ReadString")
{
    LexIO::VectorStream buffer = GetBuffer();

    std::string data;
    data.resize(9);
    REQUIRE_NOTHROW(LexIO::ReadString(data.begin(), data.end(), buffer));
    REQUIRE(data == "The quick");
}

TEST_CASE("WriteString")
{
    LexIO::VectorStream buffer;
    std::string data{"The quick"};
    std::array<uint8_t, 10> check;
    memset(check.data(), 0x00, check.size());

    REQUIRE_NOTHROW(LexIO::WriteString(buffer, data.cbegin(), data.cend()));

    LexIO::Rewind(buffer);
    REQUIRE(LexIO::Read(check, buffer) == 9);

    char *checkChar = reinterpret_cast<char *>(check.data());
    REQUIRE(strcmp(checkChar, data.c_str()) == 0);
}

#endif
