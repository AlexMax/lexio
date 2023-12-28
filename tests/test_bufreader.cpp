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

TEST(GenericBufReader, CopyCtor_CopyAssign)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));
    auto bufTest = LexIO::FillBuffer(bufReader, 8);
    VectorBufReader copyReader{bufReader};
    auto copyTest = LexIO::FillBuffer(copyReader, 8);

    EXPECT_EQ(copyTest.first[0], bufTest.first[0]);
    EXPECT_EQ(copyTest.first[7], bufTest.first[7]);
    EXPECT_EQ(copyTest.second, bufTest.second);

    LexIO::FillBuffer(copyReader, 12);
    copyReader = bufReader;
    copyTest = LexIO::FillBuffer(copyReader, 8);

    EXPECT_EQ(copyTest.first[0], bufTest.first[0]);
    EXPECT_EQ(copyTest.first[7], bufTest.first[7]);
    EXPECT_EQ(copyTest.second, bufTest.second);
}

TEST(GenericBufReader, CopyAssignSelf)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));
    LexIO::FillBuffer(bufReader, 8);
    bufReader = bufReader;
    auto bufTest = LexIO::FillBuffer(bufReader, 8);

    EXPECT_EQ(bufTest.first[0], 'T');
    EXPECT_EQ(bufTest.first[7], 'c');
    EXPECT_EQ(bufTest.second, 8);
}

TEST(GenericBufReader, MoveCtor)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));
    LexIO::FillBuffer(bufReader, 8);
    VectorBufReader moveReader{std::move(bufReader)};
    auto moveTest = LexIO::FillBuffer(moveReader, 8);

    EXPECT_EQ(moveTest.first[0], 'T');
    EXPECT_EQ(moveTest.first[7], 'c');
    EXPECT_EQ(moveTest.second, 8);
}

TEST(GenericBufReader, MoveAssign)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));
    auto moveReader = bufReader;

    LexIO::FillBuffer(bufReader, 8);
    moveReader = std::move(bufReader);
    auto moveTest = LexIO::FillBuffer(moveReader, 8);

    EXPECT_EQ(moveTest.first[0], 'T');
    EXPECT_EQ(moveTest.first[7], 'c');
    EXPECT_EQ(moveTest.second, 8);
}

TEST(GenericBufReader, FillBufferSingle)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    auto test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(GenericBufReader, FillBufferMultiple)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Buffer initial four bytes.
    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer less than what we had before, should read nothing.
    test = LexIO::FillBuffer(bufReader, 2);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer more than what we had before.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[7], 'c');
    EXPECT_EQ(test.second, 8);
}

TEST(GenericBufReader, FillBufferEOF)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Buffer everything.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);

    // Buffer more than everything.
    test = LexIO::FillBuffer(bufReader, 96);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(GenericBufReader, FillBufferEOFInitial)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    auto test = LexIO::FillBuffer(bufReader, 4);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.second, 4);

    // Buffer everything.
    test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_EQ(test.first[0], 'T');
    EXPECT_EQ(test.first[3], ' ');
    EXPECT_EQ(test.first[4], 'q');
    EXPECT_EQ(test.first[BUFFER_LENGTH - 1], '\n');
    EXPECT_EQ(test.second, BUFFER_LENGTH);
}

TEST(GenericBufReader, FillBufferZeroRead)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    auto test = LexIO::FillBuffer(bufReader, 0);
    EXPECT_EQ(test.second, 0);
}

#if defined(__has_feature)
#if !__has_feature(address_sanitizer)

TEST(GenericBufReader, FillBufferTooLarge)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    EXPECT_ANY_THROW(LexIO::FillBuffer(bufReader, SIZE_MAX));
}

#endif
#endif

TEST(GenericBufReader, ConsumeBufferSingle)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Fill, then consume whole buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 8));

    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.second, 0);

    // Subsequent read should pick up where we left off.
    test = LexIO::FillBuffer(bufReader, 8);
    EXPECT_EQ(test.first[0], 'k');
    EXPECT_EQ(test.first[7], ' ');
    EXPECT_EQ(test.second, 8);
}

TEST(GenericBufReader, ConsumeBufferMultiple)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Consume half the buffer.
    LexIO::FillBuffer(bufReader, 8);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    auto test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, 4);

    // Consume the other half of the buffer.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.second, 0);
}

TEST(GenericBufReader, ConsumeBufferEOF)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    // Fill to EOF, consume part of it.
    auto test = LexIO::FillBuffer(bufReader, 64);
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.first[0], 'q');
    EXPECT_EQ(test.first[3], 'c');
    EXPECT_EQ(test.second, BUFFER_LENGTH - 4);

    // Consume the rest of it.
    EXPECT_NO_THROW(LexIO::ConsumeBuffer(bufReader, BUFFER_LENGTH - 4));
    test = LexIO::GetBuffer(bufReader);
    EXPECT_EQ(test.second, 0);
}

TEST(GenericBufReader, ConsumeBufferTooLarge)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    LexIO::FillBuffer(bufReader, 8);
    EXPECT_ANY_THROW(LexIO::ConsumeBuffer(bufReader, 12));
}

TEST(GenericBufReader, Read)
{
    auto stream = GetStream();
    auto bufReader = VectorBufReader(std::move(stream));

    uint8_t output[8] = {0};
    size_t count = LexIO::Read(output, bufReader);
    EXPECT_EQ(output[0], 'T');
    EXPECT_EQ(output[7], 'c');
    EXPECT_EQ(count, 8);
}
