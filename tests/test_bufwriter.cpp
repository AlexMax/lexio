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

#include "lexio/bufwriter.hpp"

#include "./test.h"

using VectorBufWriter = LexIO::FixedBufWriter<LexIO::VectorStream>;
using VectorBufWriterNoCopy = LexIO::FixedBufWriter<NoCopyStream<LexIO::VectorStream>>;

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"
#endif

//******************************************************************************

TEST(FixedBufWriter, FulfillWriter)
{
    EXPECT_TRUE(LexIO::IsWriterV<VectorBufWriter>);
}

TEST(FixedBufWriter, CopyCtor_CopyAssign)
{
    auto bufWriter = VectorBufWriter{};

    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    LexIO::Write(bufWriter, data);
    auto copyWriter{bufWriter};
    LexIO::Flush(bufWriter);
    LexIO::Flush(copyWriter);

    for (size_t i = 0; i < sizeof(data); i++)
    {
        EXPECT_EQ(copyWriter.Writer().Container()[i], data[i]);
    }

    LexIO::Write(bufWriter, data);
    copyWriter = bufWriter;
    LexIO::Flush(bufWriter);
    LexIO::Flush(copyWriter);

    for (size_t i = 0; i < sizeof(data) * 2; i++)
    {
        EXPECT_EQ(copyWriter.Writer().Container()[i], data[i % sizeof(data)]);
    }
}

TEST(FixedBufWriter, CopyAssignSelf)
{
    auto bufWriter = VectorBufWriter{};

    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    LexIO::Write(bufWriter, data);
    bufWriter = bufWriter;
    LexIO::Flush(bufWriter);

    for (size_t i = 0; i < sizeof(data); i++)
    {
        EXPECT_EQ(bufWriter.Writer().Container()[i], data[i]);
    }
}

TEST(FixedBufWriter, MoveCtor)
{
    auto bufWriter = VectorBufWriterNoCopy{};

    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    LexIO::Write(bufWriter, data);
    auto moveWriter{std::move(bufWriter)};
    LexIO::Flush(moveWriter);

    for (size_t i = 0; i < sizeof(data); i++)
    {
        EXPECT_EQ(moveWriter.Writer().Stream().Container()[i], data[i]);
    }
}

TEST(FixedBufWriter, MoveAssign)
{
    auto bufWriter = VectorBufWriterNoCopy{};
    auto moveWriter = VectorBufWriterNoCopy{};

    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    LexIO::Write(bufWriter, data);
    moveWriter = std::move(bufWriter);
    LexIO::Flush(moveWriter);

    for (size_t i = 0; i < sizeof(data); i++)
    {
        EXPECT_EQ(moveWriter.Writer().Stream().Container()[i], data[i]);
    }
}

TEST(FixedBufWriter, MoveAssignSelf)
{
    auto bufWriter = VectorBufWriterNoCopy{};

    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    LexIO::Write(bufWriter, data);
    bufWriter = std::move(bufWriter);
    LexIO::Flush(bufWriter);

    for (size_t i = 0; i < sizeof(data); i++)
    {
        EXPECT_EQ(bufWriter.Writer().Stream().Container()[i], data[i]);
    }
}

TEST(FixedBufWriter, Write)
{
    auto stream = LexIO::VectorStream{};

    auto bufWriter = VectorBufWriter{std::move(stream)};
    EXPECT_EQ(::TEST_TEXT_LENGTH, LexIO::Write(bufWriter, ::TEST_TEXT_DATA, ::TEST_TEXT_LENGTH));
    LexIO::Flush(bufWriter);

    stream = std::move(bufWriter).Writer();
    EXPECT_EQ(::TEST_TEXT_LENGTH, LexIO::Length(stream));
}

TEST(FixedBufWriter, WriteSmallBuffer)
{
    auto stream = LexIO::VectorStream{};

    auto bufWriter = VectorBufWriter{std::move(stream), 16};
    EXPECT_EQ(::TEST_TEXT_LENGTH, LexIO::Write(bufWriter, ::TEST_TEXT_DATA, ::TEST_TEXT_LENGTH));
    LexIO::Flush(bufWriter);

    stream = std::move(bufWriter).Writer();
    EXPECT_EQ(::TEST_TEXT_LENGTH, LexIO::Length(stream));
}

TEST(FixedBufWriter, WriteFlushThenBuffer)
{
    auto stream = LexIO::VectorStream{};
    const auto &cstream = stream;

    auto bufWriter = VectorBufWriter{std::move(stream), 16};
    EXPECT_EQ(8, LexIO::Write(bufWriter, &::TEST_TEXT_DATA[0], 8));
    EXPECT_EQ(8, LexIO::Write(bufWriter, &::TEST_TEXT_DATA[8], 8));
    EXPECT_EQ(8, LexIO::Write(bufWriter, &::TEST_TEXT_DATA[16], 8));
    LexIO::Flush(bufWriter);

    stream = std::move(bufWriter).Writer();
    EXPECT_EQ(24, LexIO::Length(stream));
    EXPECT_EQ(0, LexIO::Rewind(stream));
    for (size_t i = 0; i < 24; i++)
    {
        EXPECT_EQ(cstream.Container()[i], ::TEST_TEXT_DATA[i]);
    }
}

TEST(FixedBufWriter, Seek)
{
    auto bufWriter = VectorBufWriter{};

    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    LexIO::Write(bufWriter, data);

    EXPECT_EQ(0, LexIO::Seek(bufWriter, 0, LexIO::Whence::start));
    for (size_t i = 0; i < sizeof(data); i++)
    {
        EXPECT_EQ(bufWriter.Writer().Container()[i], data[i]);
    }
}
