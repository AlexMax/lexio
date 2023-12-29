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

#include <algorithm>

#include "./test.h"

//******************************************************************************

struct GoodReader
{
    size_t LexRead(uint8_t *, const size_t) { return 0; }
};

TEST(Core, IsReader)
{
    EXPECT_TRUE(LexIO::IsReader<GoodReader>::value);
}

TEST(Core, IsReaderV)
{
    EXPECT_TRUE(LexIO::IsReaderV<GoodReader>);
}

//******************************************************************************

struct GoodBufferedReader : public GoodReader
{
    LexIO::BufferView LexFillBuffer(const size_t) { return LexIO::BufferView(); }
    void LexConsumeBuffer(const size_t size) { (void)size; }
};

TEST(Core, IsBufferedReader)
{
    EXPECT_TRUE(LexIO::IsBufferedReader<GoodBufferedReader>::value);
}

TEST(Core, IsBufferedReaderV)
{
    EXPECT_TRUE(LexIO::IsBufferedReaderV<GoodBufferedReader>);
}

//******************************************************************************

struct GoodWriter
{
    size_t LexWrite(const uint8_t *, const size_t) { return 0; }
    void LexFlush() {}
};

TEST(Core, IsWriter)
{
    EXPECT_TRUE(LexIO::IsWriter<GoodWriter>::value);
}

TEST(Core, IsWriterV)
{
    EXPECT_TRUE(LexIO::IsWriterV<GoodWriter>);
}

//******************************************************************************

struct GoodSeekable
{
    size_t LexSeek(const LexIO::SeekPos) { return 0; }
};

TEST(Core, IsSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekable<GoodSeekable>::value);
}

TEST(Core, IsSeekableV)
{
    EXPECT_TRUE(LexIO::IsSeekableV<GoodSeekable>);
}

//******************************************************************************

struct BadReaderMissingClass
{
};

TEST(Core, IsReaderVMissingClass)
{
    EXPECT_FALSE(LexIO::IsReaderV<BadReaderMissingClass>);
}

struct BadReaderBadParam
{
    size_t LexRead(uint8_t *&, const size_t) { return 0; }
};

TEST(Core, IsReaderVBadParam)
{
    EXPECT_FALSE(LexIO::IsReaderV<BadReaderBadParam>);
}

struct BadReaderBadReturn
{
    void LexRead(uint8_t *, const size_t) {}
};

TEST(Core, IsReaderVBadReturn)
{
    EXPECT_FALSE(LexIO::IsReaderV<BadReaderBadReturn>);
}

//******************************************************************************

TEST(Core, RawRead)
{
    LexIO::VectorStream basic = GetStream();

    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), basic), 5);
}
TEST(Core, ReadPtrLen)
{
    LexIO::VectorStream basic = GetStream();

    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], sizeof(buffer), basic), 5);
}

TEST(Core, ReadArray)
{
    LexIO::VectorStream basic = GetStream();

    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(buffer, basic), 5);
}

TEST(Core, ReadIterator)
{
    LexIO::VectorStream basic = GetStream();

    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], &buffer[5], basic), 5);
}

TEST(Core, ReadToEOF)
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), buffer);
    EXPECT_EQ(bytes, 45);
    EXPECT_EQ(data.size(), 45);
    EXPECT_EQ(*(data.end() - 1), '\n');
}

TEST(Core, ReadToEOFSmallBuffer)
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), buffer, 4);
    EXPECT_EQ(data[4], 'q'); // Check the buffer boundary.
    EXPECT_EQ(data[8], 'k');
    EXPECT_EQ(bytes, 45);
    EXPECT_EQ(data.size(), 45);
}

TEST(Core, ReadUntil)
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    size_t bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    EXPECT_EQ(bytes, 20);
    EXPECT_EQ(data.size(), 20);
    EXPECT_EQ(*(data.end() - 1), '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    EXPECT_EQ(bytes, 25);
    EXPECT_EQ(data.size(), 45);
    EXPECT_EQ(*(data.end() - 1), '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n');
    EXPECT_EQ(bytes, 0);
    EXPECT_EQ(data.size(), 45);
}

TEST(Core, ReadUntilSmallBuffer)
{
    LexIO::VectorStream basic = GetStream();
    auto buffer = VectorBufReader(std::move(basic));

    std::vector<uint8_t> data;
    size_t bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n', 4);
    EXPECT_EQ(bytes, 20);
    EXPECT_EQ(data.size(), 20);
    EXPECT_EQ(*(data.end() - 1), '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n', 4);
    EXPECT_EQ(bytes, 25);
    EXPECT_EQ(data.size(), 45);
    EXPECT_EQ(*(data.end() - 1), '\n');

    bytes = LexIO::ReadUntil(std::back_inserter(data), buffer, '\n', 4);
    EXPECT_EQ(bytes, 0);
    EXPECT_EQ(data.size(), 45);
}

TEST(Core, RawWrite)
{
    LexIO::VectorStream basic = GetStream();

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::RawWrite(basic, &data[0], sizeof(data)), 5);
}

TEST(Core, WritePtrLen)
{
    LexIO::VectorStream basic = GetStream();

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(basic, &data[0], sizeof(data)), 5);
}

TEST(Core, WriteArray)
{
    LexIO::VectorStream basic = GetStream();

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(basic, data), 5);
}

TEST(Core, WriteIterator)
{
    LexIO::VectorStream basic = GetStream();

    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(basic, &data[0], &data[5]), 5);
}

TEST(Core, Rewind)
{
    LexIO::VectorStream basic = GetStream();
    EXPECT_EQ(LexIO::Read8(basic), 'T');
    EXPECT_EQ(LexIO::Read8(basic), 'h');
    EXPECT_EQ(LexIO::Read8(basic), 'e');
    EXPECT_EQ(LexIO::Rewind(basic), 0);
    EXPECT_EQ(LexIO::Tell(basic), 0);
}

TEST(Core, Seek_Tell)
{
    LexIO::VectorStream basic = GetStream();

    LexIO::Seek(basic, 5, LexIO::Whence::start);
    EXPECT_EQ(LexIO::Tell(basic), 5);

    LexIO::Seek(basic, 5, LexIO::Whence::current);
    EXPECT_EQ(LexIO::Tell(basic), 10);

    LexIO::Seek(basic, 5, LexIO::Whence::end);
    EXPECT_EQ(LexIO::Tell(basic), BUFFER_LENGTH - 5);
}

TEST(Core, Seek_Tell_SeekPos)
{
    LexIO::VectorStream basic = GetStream();

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::Whence::start));
    EXPECT_EQ(LexIO::Tell(basic), 5);

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::Whence::current));
    EXPECT_EQ(LexIO::Tell(basic), 10);

    LexIO::Seek(basic, LexIO::SeekPos(5, LexIO::Whence::end));
    EXPECT_EQ(LexIO::Tell(basic), BUFFER_LENGTH - 5);
}

TEST(Core, TellAfterRewind)
{
    LexIO::VectorStream basic = GetStream();

    LexIO::Seek(basic, 5, LexIO::Whence::start);
    EXPECT_EQ(LexIO::Rewind(basic), 0);
    EXPECT_EQ(LexIO::Tell(basic), 0);
}

TEST(Core, Length)
{
    LexIO::VectorStream basic = GetStream();

    EXPECT_EQ(LexIO::Length(basic), BUFFER_LENGTH);
}

TEST(Core, Copy)
{
    VectorBufReader src = VectorBufReader(GetStream());
    LexIO::VectorStream dest;
    const LexIO::VectorStream &cDest = dest;

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Copy(dest, src));
    EXPECT_EQ(src.Reader().Container(), cDest.Container());
}

TEST(Core, CopySmallBuffer)
{
    VectorBufReader src = VectorBufReader(GetStream());
    LexIO::VectorStream dest;
    const LexIO::VectorStream &cDest = dest;

    EXPECT_EQ(BUFFER_LENGTH, LexIO::Copy(dest, src, 4));
    EXPECT_EQ(src.Reader().Container(), cDest.Container());
}
