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

using VectorBufWriter = LexIO::FixedBufWriter<LexIO::VectorStream>;

//******************************************************************************

TEST(FixedBufWriter, FulfillReader)
{
    EXPECT_TRUE(LexIO::IsReaderV<VectorBufWriter>);
}

TEST(FixedBufWriter, FailBufferedReader)
{
    EXPECT_FALSE(LexIO::IsBufferedReaderV<VectorBufWriter>);
}

TEST(FixedBufWriter, FulfillWriter)
{
    EXPECT_TRUE(LexIO::IsWriterV<VectorBufWriter>);
}

TEST(FixedBufWriter, FulfillSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekableV<VectorBufWriter>);
}

TEST(FixedBufWriter, Write)
{
    auto stream = LexIO::VectorStream{};

    auto bufWriter = VectorBufWriter{std::move(stream)};
    EXPECT_EQ(::BUFFER_LENGTH, LexIO::Write(bufWriter, ::BUFFER_TEXT, ::BUFFER_LENGTH));
    LexIO::Flush(bufWriter);

    stream = std::move(bufWriter).Writer();
    EXPECT_EQ(::BUFFER_LENGTH, LexIO::Length(stream));
}

TEST(FixedBufWriter, WriteSmallBuffer)
{
    auto stream = LexIO::VectorStream{};

    auto bufWriter = VectorBufWriter{std::move(stream), 16};
    EXPECT_EQ(::BUFFER_LENGTH, LexIO::Write(bufWriter, ::BUFFER_TEXT, ::BUFFER_LENGTH));
    LexIO::Flush(bufWriter);

    stream = std::move(bufWriter).Writer();
    EXPECT_EQ(::BUFFER_LENGTH, LexIO::Length(stream));
}