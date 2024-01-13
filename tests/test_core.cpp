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

#include "lexio/core.hpp"

#include "./test.h"
#include "lexio/serialize/int.hpp"
#include <array>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"
#endif

using PartialVectorStream = PartialStream<LexIO::VectorStream>;
using PartialViewStream = PartialStream<LexIO::ViewStream>;

//******************************************************************************

struct GoodReader
{
    size_t LexRead(uint8_t *, const size_t) { return 0; }
};

TEST(Reader, IsReader)
{
    EXPECT_TRUE(LexIO::IsReader<GoodReader>::value);
}

TEST(Reader, IsReaderV)
{
    EXPECT_TRUE(LexIO::IsReaderV<GoodReader>);
}

//******************************************************************************

struct GoodBufferedReader : public GoodReader
{
    LexIO::BufferView LexFillBuffer(const size_t) { return LexIO::BufferView(); }
    void LexConsumeBuffer(const size_t size) { (void)size; }
};

TEST(BufferedReader, IsBufferedReader)
{
    EXPECT_TRUE(LexIO::IsBufferedReader<GoodBufferedReader>::value);
}

TEST(BufferedReader, IsBufferedReaderV)
{
    EXPECT_TRUE(LexIO::IsBufferedReaderV<GoodBufferedReader>);
}

//******************************************************************************

struct GoodWriter
{
    size_t LexWrite(const uint8_t *, const size_t) { return 0; }
    void LexFlush() {}
};

TEST(Writer, IsWriter)
{
    EXPECT_TRUE(LexIO::IsWriter<GoodWriter>::value);
}

TEST(Writer, IsWriterV)
{
    EXPECT_TRUE(LexIO::IsWriterV<GoodWriter>);
}

//******************************************************************************

struct GoodSeekable
{
    size_t LexSeek(const LexIO::SeekPos) { return 0; }
};

TEST(Seekable, IsSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekable<GoodSeekable>::value);
}

TEST(Seekable, IsSeekableV)
{
    EXPECT_TRUE(LexIO::IsSeekableV<GoodSeekable>);
}

//******************************************************************************

struct BadReaderMissingClass
{
};

TEST(Reader, IsReaderVMissingClass)
{
    EXPECT_FALSE(LexIO::IsReaderV<BadReaderMissingClass>);
}

struct BadReaderBadParam
{
    size_t LexRead(uint8_t *&, const size_t) { return 0; }
};

TEST(Reader, IsReaderVBadParam)
{
    EXPECT_FALSE(LexIO::IsReaderV<BadReaderBadParam>);
}

struct BadReaderBadReturn
{
    void LexRead(uint8_t *, const size_t) {}
};

TEST(Reader, IsReaderVBadReturn)
{
    EXPECT_FALSE(LexIO::IsReaderV<BadReaderBadReturn>);
}

//******************************************************************************

static void AcceptReader(LexIO::ReaderRef) {}
static void AmbiguousReader(LexIO::UnbufferedReaderRef) {}
static void AmbiguousReader(LexIO::BufferedReaderRef) {}

TEST(ReaderRef, CopyCtor)
{
    auto test = GoodReader{};
    LexIO::ReaderRef ref(test);
    LexIO::ReaderRef copy(ref);
}

TEST(ReaderRef, CopyAssign)
{
    auto test = GoodReader{};
    LexIO::ReaderRef ref(test);
    LexIO::ReaderRef copy(test);
    copy = ref;
    copy = copy;
}

TEST(ReaderRef, Accept)
{
    auto test = GoodReader{};
    LexIO::ReaderRef ref(test);

    EXPECT_NO_THROW(AcceptReader(ref));
    EXPECT_NO_THROW(AmbiguousReader(ref));
}

TEST(ReaderRef, Call)
{
    uint8_t buffer[4];
    auto test = GoodReader{};
    LexIO::ReaderRef ref(test);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
}

//******************************************************************************

static void AcceptBufferedReader(LexIO::BufferedReaderRef ref)
{
    AcceptReader(ref);
}

TEST(BufferedReaderRef, CopyCtor)
{
    auto test = GoodBufferedReader{};
    LexIO::BufferedReaderRef ref(test);
    LexIO::BufferedReaderRef copy(ref);
}

TEST(BufferedReaderRef, ManualCtor)
{
    auto test = GoodBufferedReader{};
    LexIO::BufferedReaderRef ref(&test, LexIO::Detail::WrapRead<GoodBufferedReader>,
                                 LexIO::Detail::WrapFillBuffer<GoodBufferedReader>,
                                 LexIO::Detail::WrapConsumeBuffer<GoodBufferedReader>);
}

TEST(BufferedReaderRef, CopyAssign)
{
    auto test = GoodBufferedReader{};
    LexIO::BufferedReaderRef ref(test);
    LexIO::BufferedReaderRef copy(test);
    copy = ref;
    copy = copy;
}

TEST(BufferedReaderRef, Accept)
{
    auto test = GoodBufferedReader{};
    LexIO::BufferedReaderRef ref(test);

    EXPECT_NO_THROW(AcceptReader(ref));
    EXPECT_NO_THROW(AcceptBufferedReader(ref));
    EXPECT_NO_THROW(AmbiguousReader(test));
}

TEST(BufferedReaderRef, Call)
{
    uint8_t buffer[4] = {0};
    auto test = GoodBufferedReader{};
    LexIO::BufferedReaderRef ref(test);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(ref.LexRead(&buffer[0], sizeof(buffer)), 0);
    EXPECT_EQ(LexIO::FillBuffer(ref, 0).second, 0);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(ref, 0));
}

//******************************************************************************

static void AcceptUnbufferedReader(LexIO::UnbufferedReaderRef ref)
{
    AcceptReader(ref);
}

TEST(UnbufferedReaderRef, CopyCtor)
{
    auto test = GoodReader{};
    LexIO::UnbufferedReaderRef ref(test);
    LexIO::UnbufferedReaderRef copy(ref);
}

TEST(UnbufferedReaderRef, ManualCtor)
{
    auto test = GoodReader{};
    LexIO::UnbufferedReaderRef ref(&test, LexIO::Detail::WrapRead<GoodReader>);
}

TEST(UnbufferedReaderRef, CopyAssign)
{
    auto test = GoodReader{};
    LexIO::UnbufferedReaderRef ref(test);
    LexIO::UnbufferedReaderRef copy(test);
    copy = ref;
    copy = copy;
}

TEST(UnbufferedReaderRef, Accept)
{
    auto test = GoodReader{};
    LexIO::UnbufferedReaderRef ref(test);

    EXPECT_NO_THROW(AcceptReader(ref));
    EXPECT_NO_THROW(AcceptUnbufferedReader(ref));
    EXPECT_NO_THROW(AmbiguousReader(ref));
}

TEST(UnbufferedReaderRef, Call)
{
    uint8_t buffer[4];
    auto test = GoodReader{};
    LexIO::UnbufferedReaderRef ref(test);

    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), ref), 0);
    EXPECT_EQ(ref.LexRead(&buffer[0], sizeof(buffer)), 0);
}

//******************************************************************************

static void AcceptWriter(LexIO::WriterRef) {}

TEST(WriterRef, CopyCtor)
{
    auto test = GoodWriter{};
    LexIO::WriterRef ref(test);
    LexIO::WriterRef copy(ref);
}

TEST(WriterRef, ManualCtor)
{
    auto test = GoodWriter{};
    LexIO::WriterRef ref(&test, LexIO::Detail::WrapWrite<GoodWriter>, LexIO::Detail::WrapFlush<GoodWriter>);
}

TEST(WriterRef, CopyAssign)
{
    auto test = GoodWriter{};
    LexIO::WriterRef ref(test);
    LexIO::WriterRef copy(test);
    copy = ref;
    copy = copy;
}

TEST(WriterRef, Accept)
{
    auto test = GoodWriter{};
    LexIO::WriterRef ref(test);

    EXPECT_NO_THROW(AcceptWriter(ref));
}

TEST(WriterRef, Call)
{
    uint8_t buffer[4];
    auto test = GoodWriter{};
    LexIO::WriterRef ref(test);

    EXPECT_EQ(LexIO::RawWrite(ref, &buffer[0], sizeof(buffer)), 0);
    EXPECT_NO_THROW(LexIO::Flush(ref));
}

//******************************************************************************

static void AcceptSeekable(LexIO::SeekableRef) {}

TEST(SeekableRef, CopyCtor)
{
    auto test = GoodSeekable{};
    LexIO::SeekableRef ref(test);
    LexIO::SeekableRef copy(ref);
}

TEST(WriterRef, SeekableCtor)
{
    auto test = GoodSeekable{};
    LexIO::SeekableRef ref(&test, LexIO::Detail::WrapSeek<GoodSeekable>);
}

TEST(SeekableRef, CopyAssign)
{
    auto test = GoodSeekable{};
    LexIO::SeekableRef ref(test);
    LexIO::SeekableRef copy(test);
    copy = ref;
    copy = copy;
}

TEST(SeekableRef, Accept)
{
    auto test = GoodSeekable{};
    LexIO::SeekableRef ref(test);

    EXPECT_NO_THROW(AcceptSeekable(ref));
}

TEST(SeekableRef, Call)
{
    auto test = GoodSeekable{};
    LexIO::SeekableRef ref(test);

    EXPECT_EQ(LexIO::Seek(ref, LexIO::SeekPos{}), 0);
}

//******************************************************************************

TEST(Reader, RawRead)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, RawReadTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::RawRead(&buffer[0], sizeof(buffer), stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}
TEST(Reader, ReadPtrLen)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], sizeof(buffer), stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, ReadPtrLenPartial)
{
    auto stream = PartialVectorStream(GetVectorStream());

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], sizeof(buffer), stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, ReadPtrLenTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], sizeof(buffer), stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Reader, ReadArray)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(buffer, stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, ReadArrayPartial)
{
    auto stream = PartialVectorStream(GetVectorStream());

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(buffer, stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, ReadArrayTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(buffer, stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Reader, ReadIterator)
{
    auto stream = GetVectorStream();

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], &buffer[5], stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, ReadIteratorPartial)
{
    auto stream = PartialVectorStream(GetVectorStream());

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], &buffer[5], stream), 5);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
    EXPECT_EQ(buffer[i++], 'q');
}

TEST(Reader, ReadIteratorTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    uint8_t buffer[5] = {0};
    EXPECT_EQ(LexIO::Read(&buffer[0], &buffer[5], stream), 4);
    EXPECT_EQ(buffer[i++], 'T');
    EXPECT_EQ(buffer[i++], 'h');
    EXPECT_EQ(buffer[i++], 'e');
    EXPECT_EQ(buffer[i++], ' ');
}

TEST(Reader, ReadIteratorEmpty)
{
    auto stream = GetVectorStream();

    std::array<uint8_t, 5> buffer = {0};
    EXPECT_EQ(0, LexIO::Read(buffer.begin(), buffer.begin(), stream));
}

//******************************************************************************

TEST(Writer, RawWrite)
{
    auto stream = LexIO::VectorStream{};
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

TEST(Writer, RawWriteTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::RawWrite(stream, &data[0], sizeof(data)), 4);
    EXPECT_EQ(streamBuf[i++], 'X');
    EXPECT_EQ(streamBuf[i++], 'Y');
    EXPECT_EQ(streamBuf[i++], 'Z');
    EXPECT_EQ(streamBuf[i++], 'Z');
}

TEST(Writer, WritePtrLen)
{
    auto stream = LexIO::VectorStream{};
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

TEST(Writer, WritePtrLenPartial)
{
    auto stream = PartialVectorStream{LexIO::VectorStream{}};
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

TEST(Writer, WritePtrLenTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], sizeof(data)), 4);
    EXPECT_EQ(streamBuf[i++], 'X');
    EXPECT_EQ(streamBuf[i++], 'Y');
    EXPECT_EQ(streamBuf[i++], 'Z');
    EXPECT_EQ(streamBuf[i++], 'Z');
}

TEST(Writer, WriteArray)
{
    auto stream = LexIO::VectorStream{};
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

TEST(Writer, WriteArrayPartial)
{
    auto stream = PartialVectorStream{LexIO::VectorStream{}};
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

TEST(Writer, WriteArrayTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, data), 4);
    EXPECT_EQ(streamBuf[i++], 'X');
    EXPECT_EQ(streamBuf[i++], 'Y');
    EXPECT_EQ(streamBuf[i++], 'Z');
    EXPECT_EQ(streamBuf[i++], 'Z');
}

TEST(Writer, WriteIterator)
{
    auto stream = LexIO::VectorStream{};
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

TEST(Writer, WriteIteratorPartial)
{
    auto stream = PartialVectorStream{LexIO::VectorStream{}};
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

TEST(Writer, WriteIteratorTrunc)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    size_t i = 0;
    const uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(LexIO::Write(stream, &data[0], &data[5]), 4);
    EXPECT_EQ(streamBuf[i++], 'X');
    EXPECT_EQ(streamBuf[i++], 'Y');
    EXPECT_EQ(streamBuf[i++], 'Z');
    EXPECT_EQ(streamBuf[i++], 'Z');
}

TEST(Writer, WriteIteratorEmpty)
{
    uint8_t streamBuf[4] = {0};
    auto stream = GetViewStream(streamBuf);

    const std::vector<uint8_t> data = {'X', 'Y', 'Z', 'Z', 'Y'};
    EXPECT_EQ(0, LexIO::Write(stream, data.begin(), data.begin()));
}

//******************************************************************************

TEST(Seekable, Rewind)
{
    LexIO::VectorStream stream = GetVectorStream();
    EXPECT_EQ(LexIO::Read8(stream), 'T');
    EXPECT_EQ(LexIO::Read8(stream), 'h');
    EXPECT_EQ(LexIO::Read8(stream), 'e');
    EXPECT_EQ(LexIO::Rewind(stream), 0);
    EXPECT_EQ(LexIO::Tell(stream), 0);
}

TEST(Seekable, Seek_Tell)
{
    LexIO::VectorStream stream = GetVectorStream();

    LexIO::Seek(stream, 5);
    EXPECT_EQ(LexIO::Tell(stream), 5);

    LexIO::Seek(stream, 5, LexIO::Whence::start);
    EXPECT_EQ(LexIO::Tell(stream), 5);

    LexIO::Seek(stream, 5, LexIO::Whence::current);
    EXPECT_EQ(LexIO::Tell(stream), 10);

    LexIO::Seek(stream, 5, LexIO::Whence::end);
    EXPECT_EQ(LexIO::Tell(stream), TEST_TEXT_LENGTH - 5);
}

TEST(Seekable, Seek_Tell_SeekPos)
{
    LexIO::VectorStream stream = GetVectorStream();

    LexIO::Seek(stream, LexIO::SeekPos(5, LexIO::Whence::start));
    EXPECT_EQ(LexIO::Tell(stream), 5);

    LexIO::Seek(stream, LexIO::SeekPos(5, LexIO::Whence::current));
    EXPECT_EQ(LexIO::Tell(stream), 10);

    LexIO::Seek(stream, LexIO::SeekPos(5, LexIO::Whence::end));
    EXPECT_EQ(LexIO::Tell(stream), TEST_TEXT_LENGTH - 5);
}

TEST(Seekable, TellAfterRewind)
{
    LexIO::VectorStream stream = GetVectorStream();

    LexIO::Seek(stream, 5, LexIO::Whence::start);
    EXPECT_EQ(LexIO::Rewind(stream), 0);
    EXPECT_EQ(LexIO::Tell(stream), 0);
}

TEST(Seekable, Length)
{
    LexIO::VectorStream stream = GetVectorStream();

    EXPECT_EQ(LexIO::Length(stream), TEST_TEXT_LENGTH);
}
