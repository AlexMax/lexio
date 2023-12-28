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

template <typename T>
void CommonTest(T &buf)
{
    // Test reading.
    LexIO::WriteU8(buf, 192);
    EXPECT_EQ(LexIO::Tell(buf), 1);
    LexIO::WriteU16LE(buf, 1993);
    EXPECT_EQ(LexIO::Tell(buf), 3);
    LexIO::WriteU32LE(buf, 20200602);
    EXPECT_EQ(LexIO::Tell(buf), 7);
    LexIO::WriteU64LE(buf, uint64_t(UINT32_MAX) + 20200613);
    EXPECT_EQ(LexIO::Tell(buf), 15);
    EXPECT_EQ(LexIO::Length(buf), 15);

    // Test writing.
    LexIO::Rewind(buf);
    EXPECT_EQ(LexIO::Tell(buf), 0);
    EXPECT_EQ(LexIO::ReadU8(buf), 192);
    EXPECT_EQ(LexIO::ReadU16LE(buf), 1993);
    EXPECT_EQ(LexIO::ReadU32LE(buf), 20200602);
    EXPECT_EQ(LexIO::ReadU64LE(buf), uint64_t(UINT32_MAX) + 20200613);
    EXPECT_EQ(LexIO::Tell(buf), 15);

    // Test seeking.
    LexIO::Seek(buf, 2, LexIO::Whence::start);
    EXPECT_EQ(LexIO::Tell(buf), 2);
    LexIO::Seek(buf, 2, LexIO::Whence::current);
    EXPECT_EQ(LexIO::Tell(buf), 4);
    LexIO::Seek(buf, -2, LexIO::Whence::current);
    EXPECT_EQ(LexIO::Tell(buf), 2);
    LexIO::Seek(buf, 2, LexIO::Whence::end);
    EXPECT_EQ(LexIO::Tell(buf), 13);

    // Test overwriting data.
    LexIO::WriteU16LE(buf, 1993);
    EXPECT_EQ(LexIO::Tell(buf), 15);
    EXPECT_EQ(LexIO::Length(buf), 15);
    LexIO::Seek(buf, -2, LexIO::Whence::current);
    EXPECT_EQ(LexIO::Tell(buf), 13);
    EXPECT_EQ(LexIO::Length(buf), 15);
    EXPECT_EQ(LexIO::ReadU16LE(buf), 1993);
}

constexpr size_t TEST_SIZE = 15;

TEST(ContainerFixed, StdArray)
{
    using ContainerFixed = LexIO::ContainerFixed<std::array<uint8_t, TEST_SIZE>>;
    ContainerFixed buf;
    CommonTest(buf);
    EXPECT_ANY_THROW(LexIO::WriteU8(buf, 0));
    EXPECT_EQ(LexIO::Tell(buf), TEST_SIZE);
}

TEST(ContainerFixed, PreallocSize)
{
    using ContainerStatic = LexIO::ContainerStatic<std::vector<uint8_t>>;
    ContainerStatic buf(TEST_SIZE);
    CommonTest(buf);
    EXPECT_ANY_THROW(LexIO::WriteU8(buf, 0));
    EXPECT_EQ(LexIO::Tell(buf), TEST_SIZE);
}

TEST(ContainerFixed, StdVector)
{
    using ContainerDynamic = LexIO::ContainerDynamic<std::vector<uint8_t>>;
    ContainerDynamic buf;
    CommonTest(buf);
    EXPECT_NO_THROW(LexIO::WriteU8(buf, 0));
    EXPECT_EQ(LexIO::Tell(buf), TEST_SIZE + 1);
}
