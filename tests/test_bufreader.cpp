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

#include "lexio/bufreader.hpp"

#include "./test.h"

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"
#endif

using VectorBufReader = LexIO::GenericBufReader<LexIO::VectorStream>;
using VectorBufReaderNoCopy = LexIO::GenericBufReader<NoCopyStream<LexIO::VectorStream>>;

//******************************************************************************

TEST(GenericBufReader, FulfillBufferedReader)
{
    EXPECT_TRUE(LexIO::IsBufferedReaderV<VectorBufReader>);
}

TEST(GenericBufReader, FulfillWriter)
{
    EXPECT_TRUE(LexIO::IsWriterV<VectorBufReader>);
}

TEST(GenericBufReader, FulfillSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekableV<VectorBufReader>);
}

TEST(GenericBufReader, DefCtor)
{
    auto bufReader = VectorBufReader{};

    EXPECT_EQ(0, LexIO::Length(bufReader));
}

TEST(GenericBufReader, CopyCtor_CopyAssign)
{
    auto bufReader = VectorBufReader{GetVectorStream()};
    auto bufTest = LexIO::FillBuffer(bufReader, 8);
    VectorBufReader copyReader{bufReader};
    auto copyTest = LexIO::FillBuffer(copyReader, 8);

    EXPECT_EQ(copyTest.Data()[0], bufTest.Data()[0]);
    EXPECT_EQ(copyTest.Data()[7], bufTest.Data()[7]);
    EXPECT_EQ(copyTest.Size(), bufTest.Size());

    LexIO::FillBuffer(copyReader, 12);
    copyReader = bufReader;
    copyTest = LexIO::FillBuffer(copyReader, 8);

    EXPECT_EQ(copyTest.Data()[0], bufTest.Data()[0]);
    EXPECT_EQ(copyTest.Data()[7], bufTest.Data()[7]);
    EXPECT_EQ(copyTest.Size(), bufTest.Size());
}

TEST(GenericBufReader, CopyAssignSelf)
{
    auto bufReader = VectorBufReader{GetVectorStream()};
    LexIO::FillBuffer(bufReader, 8);
    bufReader = bufReader;
    auto bufTest = LexIO::FillBuffer(bufReader, 8);

    EXPECT_EQ(bufTest.Data()[0], 'T');
    EXPECT_EQ(bufTest.Data()[7], 'c');
    EXPECT_EQ(bufTest.Size(), 8);
}

TEST(GenericBufReader, MoveCtor)
{
    VectorBufReaderNoCopy bufReader{GetVectorStream()};
    LexIO::FillBuffer(bufReader, 8);
    VectorBufReaderNoCopy moveReader{std::move(bufReader)};
    auto moveTest = LexIO::FillBuffer(moveReader, 8);

    EXPECT_EQ(moveTest.Data()[0], 'T');
    EXPECT_EQ(moveTest.Data()[7], 'c');
    EXPECT_EQ(moveTest.Size(), 8);
}

TEST(GenericBufReader, MoveAssign)
{
    auto bufReader = VectorBufReaderNoCopy{GetVectorStream()};
    auto moveReader = VectorBufReaderNoCopy{};

    LexIO::FillBuffer(bufReader, 8);
    moveReader = std::move(bufReader);
    auto moveTest = LexIO::FillBuffer(moveReader, 8);

    EXPECT_EQ(moveTest.Data()[0], 'T');
    EXPECT_EQ(moveTest.Data()[7], 'c');
    EXPECT_EQ(moveTest.Size(), 8);
}

TEST(GenericBufReader, MoveAssignSelf)
{
    auto bufReader = VectorBufReaderNoCopy{GetVectorStream()};
    LexIO::FillBuffer(bufReader, 8);
    bufReader = std::move(bufReader);
    auto bufTest = LexIO::FillBuffer(bufReader, 8);

    EXPECT_EQ(bufTest.Data()[0], 'T');
    EXPECT_EQ(bufTest.Data()[7], 'c');
    EXPECT_EQ(bufTest.Size(), 8);
}

TEST(GenericBufReader, Read)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    uint8_t output[8] = {0};
    size_t count = LexIO::Read(output, bufReader);
    EXPECT_EQ(output[0], 'T');
    EXPECT_EQ(output[7], 'c');
    EXPECT_EQ(count, 8);
}

TEST(GenericBufReader, FillBufferSingle)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    auto test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[7], 'c');
    EXPECT_EQ(test.Size(), 8);
}

TEST(GenericBufReader, FillBufferMultiple)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    // Buffer initial four bytes.
    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Size(), 4);

    // Buffer less than what we had before, should read nothing.
    test = LexIO::FillBuffer(bufReader, 2);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Size(), 4);

    // Buffer more than what we had before.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Data()[4], 'q');
    EXPECT_EQ(test.Data()[7], 'c');
    EXPECT_EQ(test.Size(), 8);
}

TEST(GenericBufReader, FillBufferEOF)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    // Buffer everything.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[TEST_TEXT_LENGTH - 1], '\n');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH);

    // Buffer more than everything.
    test = LexIO::FillBuffer(bufReader, 96);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[TEST_TEXT_LENGTH - 1], '\n');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH);
}

TEST(GenericBufReader, FillBufferEOFInitial)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Size(), 4);

    // Buffer everything.
    test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.Data()[0], 'T');
    EXPECT_EQ(test.Data()[3], ' ');
    EXPECT_EQ(test.Data()[4], 'q');
    EXPECT_EQ(test.Data()[TEST_TEXT_LENGTH - 1], '\n');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH);
}

TEST(GenericBufReader, FillBufferZeroRead)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    auto test = LexIO::FillBuffer(bufReader, 0);
    EXPECT_EQ(test.Size(), 0);
}

#if defined(__has_feature)
#if !__has_feature(address_sanitizer)

TEST(GenericBufReader, FillBufferTooLarge)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    EXPECT_ANY_THROW(LexIO::FillBuffer(bufReader, SIZE_MAX));
}

#endif
#endif

TEST(GenericBufReader, FillBufferSmallRead)
{
    auto bufReader = VectorBufReader{GetVectorStream()};
    uint8_t data[5] = {0};

    auto view = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(8, view.Size());

    // Do a small-enough read to where we don't use up our entire buffer.
    EXPECT_EQ(5, LexIO::Read(data, bufReader));

    size_t i = 0;
    EXPECT_EQ(data[i++], 'T');
    EXPECT_EQ(data[i++], 'h');
    EXPECT_EQ(data[i++], 'e');
    EXPECT_EQ(data[i++], ' ');
    EXPECT_EQ(data[i++], 'q');

    // Do a larger read that goes past what we've previously filled the
    // buffer with.
    EXPECT_EQ(5, LexIO::Read(data, bufReader));

    i = 0;
    EXPECT_EQ(data[i++], 'u');
    EXPECT_EQ(data[i++], 'i');
    EXPECT_EQ(data[i++], 'c');
    EXPECT_EQ(data[i++], 'k');
    EXPECT_EQ(data[i++], ' ');
}

TEST(GenericBufReader, FillBufferWrite)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Size(), 4);
    LexIO::Write(bufReader, {'X', 'Y', 'Z', 'Z'});
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Data()[0], 'k');
    EXPECT_EQ(view.Data()[1], ' ');
    EXPECT_EQ(view.Data()[2], 'b');
    EXPECT_EQ(view.Data()[3], 'r');
    EXPECT_EQ(view.Size(), 4);
}

TEST(GenericBufReader, FillBufferFlush)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Size(), 4);
    LexIO::Flush(bufReader);
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Data()[0], 'T');
    EXPECT_EQ(view.Data()[1], 'h');
    EXPECT_EQ(view.Data()[2], 'e');
    EXPECT_EQ(view.Data()[3], ' ');
    EXPECT_EQ(view.Size(), 4);
}

TEST(GenericBufReader, FillBufferSeek)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    auto view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Size(), 4);
    LexIO::Seek(bufReader, 4, LexIO::Whence::current);
    view = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(view.Data()[0], 'k');
    EXPECT_EQ(view.Data()[1], ' ');
    EXPECT_EQ(view.Data()[2], 'b');
    EXPECT_EQ(view.Data()[3], 'r');
    EXPECT_EQ(view.Size(), 4);
}

TEST(GenericBufReader, ConsumeBufferSingle)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    // Fill, then consume whole buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 8));

    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Size(), 0);

    // Subsequent read should pick up where we left off.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.Data()[0], 'k');
    EXPECT_EQ(test.Data()[7], ' ');
    EXPECT_EQ(test.Size(), 8);
}

TEST(GenericBufReader, ConsumeBufferMultiple)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    // Consume half the buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Data()[0], 'q');
    EXPECT_EQ(test.Data()[3], 'c');
    EXPECT_EQ(test.Size(), 4);

    // Consume the other half of the buffer.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Size(), 0);
}

TEST(GenericBufReader, ConsumeBufferEOF)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    // Fill to EOF, consume part of it.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Data()[0], 'q');
    EXPECT_EQ(test.Data()[3], 'c');
    EXPECT_EQ(test.Size(), TEST_TEXT_LENGTH - 4);

    // Consume the rest of it.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, TEST_TEXT_LENGTH - 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.Size(), 0);
}

TEST(GenericBufReader, ConsumeBufferTooLarge)
{
    auto bufReader = VectorBufReader{GetVectorStream()};

    LexIO::FillBuffer(bufReader, 8);
    EXPECT_ANY_THROW(LexIO::ConsumeBuffer(bufReader, 12));
}
