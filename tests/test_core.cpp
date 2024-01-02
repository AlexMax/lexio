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

using PartialVectorStream = PartialStream<LexIO::VectorStream>;
using PartialArrayStream = PartialStream<LexIO::ArrayStream<5>>;

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
    auto stream = GetStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Core, RawReadTrunc)
{
    auto stream = GetStreamTrunc();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}
TEST(Core, ReadPtrLen)
{
    auto stream = GetStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], sizeof(buffer), stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Core, ReadPtrLenPartial)
{
    auto stream = PartialVectorStream(GetStream());

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], sizeof(buffer), stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Core, ReadPtrLenTrunc)
{
    auto stream = GetStreamTrunc();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], sizeof(buffer), stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Core, ReadArray)
{
    auto stream = GetStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(buffer, stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Core, ReadArrayPartial)
{
    auto stream = PartialVectorStream(GetStream());

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(buffer, stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Core, ReadArrayTrunc)
{
    auto stream = GetStreamTrunc();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(buffer, stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Core, ReadIterator)
{
    auto stream = GetStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], &buffer[5], stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Core, ReadIteratorPartial)
{
    auto stream = PartialVectorStream(GetStream());

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], &buffer[5], stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Core, ReadIteratorTrunc)
{
    auto stream = GetStreamTrunc();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], &buffer[5], stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Core, ReadToEOF)
{
    auto stream = GetStream();
    auto buffer = VectorBufReader(std::move(stream));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), buffer);
    EXPECT_EQ(bytes, 45);
    EXPECT_EQ(data.size(), 45);
    EXPECT_EQ(*(data.end() - 1), '\n');
}

TEST(Core, ReadToEOFSmallBuffer)
{
    LexIO::VectorStream stream = GetStream();
    auto buffer = VectorBufReader(std::move(stream));

    std::vector<uint8_t> data;
    const size_t bytes = LexIO::ReadToEOF(std::back_inserter(data), buffer, 4);
    EXPECT_EQ(data[4], 'q'); // Check the buffer boundary.
    EXPECT_EQ(data[8], 'k');
    EXPECT_EQ(bytes, 45);
    EXPECT_EQ(data.size(), 45);
}

TEST(Core, ReadUntil)
{
    auto stream = GetStream();
    auto buffer = VectorBufReader(std::move(stream));

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
    LexIO::VectorStream stream = GetStream();
    auto buffer = VectorBufReader(std::move(stream));

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
    auto stream = LexIO::ArrayStream<5>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::RawWrite(stream, &data[0], sizeof(data)), 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(Core, RawWriteTrunc)
{
    auto stream = LexIO::ArrayStream<4>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::RawWrite(stream, &data[0], sizeof(data)), 4);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
}

TEST(Core, WritePtrLen)
{
    auto stream = LexIO::ArrayStream<5>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], sizeof(data)), 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(Core, WritePtrLenPartial)
{
    auto stream = PartialArrayStream{LexIO::ArrayStream<5>{}};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], sizeof(data)), 5);
    EXPECT_EQ(cstream.Stream().Container()[i++], 'X');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
}

TEST(Core, WritePtrLenTrunc)
{
    auto stream = LexIO::ArrayStream<4>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], sizeof(data)), 4);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
}

TEST(Core, WriteArray)
{
    auto stream = LexIO::ArrayStream<5>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, data), 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(Core, WriteArrayPartial)
{
    auto stream = PartialArrayStream{LexIO::ArrayStream<5>{}};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, data), 5);
    EXPECT_EQ(cstream.Stream().Container()[i++], 'X');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
}

TEST(Core, WriteArrayTrunc)
{
    auto stream = LexIO::ArrayStream<4>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, data), 4);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
}

TEST(Core, WriteIterator)
{
    auto stream = LexIO::ArrayStream<5>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], &data[5]), 5);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
}

TEST(Core, WriteIteratorPartial)
{
    auto stream = PartialArrayStream{LexIO::ArrayStream<5>{}};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], &data[5]), 5);
    EXPECT_EQ(cstream.Stream().Container()[i++], 'X');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Z');
    EXPECT_EQ(cstream.Stream().Container()[i++], 'Y');
}

TEST(Core, WriteIteratorTrunc)
{
    auto stream = LexIO::ArrayStream<4>{};
    const auto &cstream = stream;

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], &data[5]), 4);
    EXPECT_EQ(cstream.Container()[i++], 'X');
    EXPECT_EQ(cstream.Container()[i++], 'Y');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
    EXPECT_EQ(cstream.Container()[i++], 'Z');
}

TEST(Core, Rewind)
{
    LexIO::VectorStream stream = GetStream();
    EXPECT_EQ(LexIO::Read8(stream), 'T');
    EXPECT_EQ(LexIO::Read8(stream), 'h');
    EXPECT_EQ(LexIO::Read8(stream), 'e');
    EXPECT_EQ(LexIO::Rewind(stream), 0);
    EXPECT_EQ(LexIO::Tell(stream), 0);
}

TEST(Core, Seek_Tell)
{
    LexIO::VectorStream stream = GetStream();

    LexIO::Seek(stream, 5);
    EXPECT_EQ(LexIO::Tell(stream), 5);

    LexIO::Seek(stream, 5, LexIO::Whence::start);
    EXPECT_EQ(LexIO::Tell(stream), 5);

    LexIO::Seek(stream, 5, LexIO::Whence::current);
    EXPECT_EQ(LexIO::Tell(stream), 10);

    LexIO::Seek(stream, 5, LexIO::Whence::end);
    EXPECT_EQ(LexIO::Tell(stream), BUFFER_LENGTH - 5);
}

TEST(Core, Seek_Tell_SeekPos)
{
    LexIO::VectorStream stream = GetStream();

    LexIO::Seek(stream, LexIO::SeekPos(5, LexIO::Whence::start));
    EXPECT_EQ(LexIO::Tell(stream), 5);

    LexIO::Seek(stream, LexIO::SeekPos(5, LexIO::Whence::current));
    EXPECT_EQ(LexIO::Tell(stream), 10);

    LexIO::Seek(stream, LexIO::SeekPos(5, LexIO::Whence::end));
    EXPECT_EQ(LexIO::Tell(stream), BUFFER_LENGTH - 5);
}

TEST(Core, TellAfterRewind)
{
    LexIO::VectorStream stream = GetStream();

    LexIO::Seek(stream, 5, LexIO::Whence::start);
    EXPECT_EQ(LexIO::Rewind(stream), 0);
    EXPECT_EQ(LexIO::Tell(stream), 0);
}

TEST(Core, Length)
{
    LexIO::VectorStream stream = GetStream();

    EXPECT_EQ(LexIO::Length(stream), BUFFER_LENGTH);
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
