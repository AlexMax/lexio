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

//
//  This is a test TU that does not include Catch2, used to test compile speed.
//

#include "./test.h"

#include <cstdio>
#include <iterator>

using FileBufReader = LexIO::VectorBufReader<LexIO::File>;

int main()
{
    auto file = LexIO::Open("../test/test_file.txt", LexIO::OpenMode::read);
    auto buffer = FileBufReader::FromReader(std::move(file));

    std::vector<uint8_t> data;
    LexIO::ReadAll(std::back_inserter(data), buffer);
    data.push_back(0x00);

    printf("%s\n", reinterpret_cast<const char *>(data.data()));
    return 0;
}
