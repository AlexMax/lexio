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

#pragma once

#include <cstring>
#include <iterator>

#include "config.h"

#include "lexio/lexio.hpp"

using VectorBufReader = LexIO::GenericBufReader<LexIO::VectorStream>;

template <typename T, std::size_t N>
constexpr std::size_t CountOf(T const (&)[N]) noexcept
{
    return N;
}

constexpr uint8_t BUFFER_TEXT[] = "The quick brown fox\njumps over the lazy dog.\n";
constexpr size_t BUFFER_LENGTH = CountOf(BUFFER_TEXT) - sizeof('\0');

inline LexIO::VectorStream GetStream()
{
    LexIO::VectorStream rvo;
    rvo.LexWrite(&BUFFER_TEXT[0], BUFFER_LENGTH);
    rvo.LexSeek(LexIO::SeekPos(0, LexIO::Whence::start));
    return rvo;
}
