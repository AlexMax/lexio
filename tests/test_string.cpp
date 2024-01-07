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

#include "lexio/string.hpp"

#include "./test.h"
#include <array>

TEST(String, ReadStringPtrLen)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    char buffer[5] = {0};
    EXPECT_EQ(5, LexIO::ReadString(&buffer[0], sizeof(buffer), stream));
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(String, ReadStringArray)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    char buffer[5] = {0};
    EXPECT_EQ(5, LexIO::ReadString(buffer, stream));
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(String, ReadStringIterator)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    char buffer[5] = {0};
    EXPECT_EQ(5, LexIO::ReadString(&buffer[0], &buffer[5], stream));
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(String, ReadStringIteratorEmpty)
{
    auto stream = GetVectorStream();

    std::array<char, 5> buffer = {0};
    EXPECT_EQ(0, LexIO::ReadString(buffer.begin(), buffer.begin(), stream));
}

TEST(String, WriteStringPtrLen)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    size_t i = 0;
    const char data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(5, LexIO::WriteString(stream, &data[0], sizeof(data)));
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(String, WriteStringArray)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    size_t i = 0;
    const char data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(5, LexIO::WriteString(stream, data));
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(String, WriteStringIterator)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    size_t i = 0;
    const char data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(5, LexIO::WriteString(stream, &data[0], &data[5]));
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(String, WriteStringIteratorEmpty)
{
    auto stream = LexIO::VectorStream{};

    const std::vector<char> data = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(0, LexIO::WriteString(stream, data.begin(), data.begin()));
}