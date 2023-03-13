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
// Stream that uses std::array under the hood.
//

#pragma once

#include "./core.h"

#include "./container.h"

#include <array>

namespace LexIO
{

/**
 * @brief A stream backed by a std::array.
 */
template <size_t N>
using ArrayStream = LexIO::ContainerFixed<std::array<uint8_t, N>>;

} // namespace LexIO
