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

#include "lexio/lib.hpp"

#include "./test.h"

TEST(Lib, ReadToEOF)
{
    auto stream = GetVectorStream();
    auto reader = LexIO::ReaderRef(stream);

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), reader);
    EXPECT_EQ(bytes, 45);
    EXPECT_EQ(data.size(), 45);
    EXPECT_EQ(*(data.end() - 1), '\n');
}

TEST(Lib, ReadToEOFEmpty)
{
    auto stream = LexIO::VectorStream{};
    auto reader = LexIO::ReaderRef{stream};

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), reader);
    EXPECT_EQ(bytes, 0);
    EXPECT_EQ(data.size(), 0);
}

TEST(Lib, ReadToEOFSmallBuffer)
{
    auto stream = LexIO::VectorStream{'X', 'Y', 'Z', 'Z', 'Y'};
    auto reader = LexIO::ReaderRef{stream};

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), reader);
    EXPECT_EQ(bytes, 5);
    EXPECT_EQ(data.size(), 5);
}

TEST(Lib, BufferedReadToEOF)
{
    auto bufReader = GetVectorStream();

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), bufReader);
    EXPECT_EQ(bytes, 45);
    EXPECT_EQ(data.size(), 45);
    EXPECT_EQ(*(data.end() - 1), '\n');
}

TEST(Lib, BufferedReadUntil)
{
    auto bufReader = GetVectorStream();

    std::vector<uint8_t> data;
    size_t bytes = LexIO::ReadUntil(std::back_inserter(data), bufReader, '\n');
    EXPECT_EQ(bytes, 20);
    EXPECT_EQ(data.size(), 20);
    EXPECT_EQ(*(data.end() - 1), '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), bufReader, '\n');
    EXPECT_EQ(bytes, 25);
    EXPECT_EQ(data.size(), 45);
    EXPECT_EQ(*(data.end() - 1), '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), bufReader, '\n');
    EXPECT_EQ(bytes, 0);
    EXPECT_EQ(data.size(), 45);
}

TEST(Lib, BufferedReadUntilLarge)
{
    auto bufReader = LexIO::VectorStream{};
    uint8_t writeData[] = {'X', 'Y', 'Z', 'Z', 'Y', 'F', 'O', 'O', 'B', 'A', 'R', ' '};

    for (size_t i = 0; i < 9216; i += sizeof(writeData))
    {
        LexIO::Write(bufReader, writeData);
    }
    LexIO::Write(bufReader, {'\n', 'E', 'N', 'D'});
    LexIO::Rewind(bufReader);

    std::vector<uint8_t> data;
    size_t bytes = LexIO::ReadUntil(std::back_inserter(data), bufReader, '\n');
    EXPECT_EQ(bytes, 9217);
    EXPECT_EQ(data.size(), 9217);
    EXPECT_EQ(*(data.end() - 1), '\n');
}

TEST(Lib, BufferedCopy)
{
    LexIO::VectorStream src = GetVectorStream();
    LexIO::VectorStream dest;
    const LexIO::VectorStream &cDest = dest;

    EXPECT_EQ(TEST_TEXT_LENGTH, LexIO::Copy(dest, src));
    EXPECT_EQ(src.Container(), cDest.Container());
}
