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
// Stream and BufReader that uses std::vector under the hood.
//

#pragma once

#include "../core.hpp"

#include "./container.hpp"

#include <vector>

namespace LexIO
{

/**
 * @brief A stream backed by a std::vector.
 */
using VectorStream = LexIO::ContainerDynamic<std::vector<uint8_t>>;

} // namespace LexIO
