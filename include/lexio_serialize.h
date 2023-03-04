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
// Serialization functions.
//

#pragma once

#include "lexio_core.h"

namespace LexIO
{

template <typename READER>
inline uint8_t ReadU8(READER &buffer)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = RawRead(buffer, buf);
    if (count != sizeof(uint8_t))
    {
        throw std::runtime_error("could not read 1 byte");
    }
    return buf[0];
}

template <typename READER>
inline int8_t Read8(READER &buffer)
{
    return static_cast<int8_t>(ReadU8(buffer));
}

template <typename READER>
inline uint16_t ReadU16LE(READER &buffer)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = RawRead(buffer, buf);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not read 2 bytes");
    }
    return buf[0] | (buf[1] << 8);
}

template <typename READER>
inline int16_t Read16LE(READER &buffer)
{
    return static_cast<int16_t>(ReadU16LE(buffer));
}

template <typename READER>
inline uint32_t ReadU32LE(READER &buffer)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = RawRead(buffer, buf);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not read 4 bytes");
    }
    return buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
}

template <typename READER>
inline int32_t Read32LE(READER &buffer)
{
    return static_cast<int32_t>(ReadU32LE(buffer));
}

template <typename READER>
inline uint64_t ReadU64LE(READER &buffer)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = RawRead(buffer, buf);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not read 8 bytes");
    }
    return static_cast<uint64_t>(buf[0]) | (static_cast<uint64_t>(buf[1]) << 8) |
           (static_cast<uint64_t>(buf[2]) << 16) | (static_cast<uint64_t>(buf[3]) << 24) |
           (static_cast<uint64_t>(buf[4]) << 32) | (static_cast<uint64_t>(buf[5]) << 40) |
           (static_cast<uint64_t>(buf[6]) << 48) | (static_cast<uint64_t>(buf[7]) << 56);
}

template <typename READER>
inline int64_t Read64LE(READER &buffer)
{
    return static_cast<int64_t>(ReadU64LE(buffer));
}

template <typename WRITER>
inline void WriteU8(WRITER &buffer, const uint8_t value)
{
    uint8_t buf[sizeof(uint8_t)] = {value};
    const size_t count = RawWrite(buffer, buf);
    if (count != sizeof(uint8_t))
    {
        throw std::runtime_error("could not write 1 byte");
    }
}

template <typename WRITER>
inline void Write8(WRITER &buffer, const int8_t value)
{
    WriteU8(buffer, static_cast<uint8_t>(value));
}

template <typename WRITER>
inline void WriteU16LE(WRITER &buffer, const uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {
        static_cast<uint8_t>(value & 0xff),
        static_cast<uint8_t>((value & 0xff00) >> 8),
    };
    const size_t count = RawWrite(buffer, buf);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not write 2 bytes");
    }
}

template <typename WRITER>
inline void Write16LE(WRITER &buffer, const int16_t value)
{
    WriteU16LE(buffer, static_cast<uint16_t>(value));
}

template <typename WRITER>
inline void WriteU32LE(WRITER &buffer, const uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {
        static_cast<uint8_t>(value & 0xff),
        static_cast<uint8_t>((value & 0xff00) >> 8),
        static_cast<uint8_t>((value & 0xff0000) >> 16),
        static_cast<uint8_t>((value & 0xff000000) >> 24),
    };
    const size_t count = RawWrite(buffer, buf);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not write 4 bytes");
    }
}

template <typename WRITER>
inline void Write32LE(WRITER &buffer, const int32_t value)
{
    WriteU32LE(buffer, static_cast<uint32_t>(value));
}

template <typename WRITER>
inline void WriteU64LE(WRITER &buffer, const uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {
        static_cast<uint8_t>(value & 0xff),
        static_cast<uint8_t>((value & 0xff00) >> 8),
        static_cast<uint8_t>((value & 0xff0000) >> 16),
        static_cast<uint8_t>((value & 0xff000000) >> 24),
        static_cast<uint8_t>((value & 0xff00000000) >> 32),
        static_cast<uint8_t>((value & 0xff0000000000) >> 40),
        static_cast<uint8_t>((value & 0xff000000000000) >> 48),
        static_cast<uint8_t>((value & 0xff00000000000000) >> 56),
    };
    const size_t count = RawWrite(buffer, buf);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not write 8 bytes");
    }
}

template <typename WRITER>
inline void Write64LE(WRITER &buffer, const int64_t value)
{
    WriteU64LE(buffer, static_cast<uint64_t>(value));
}

} // namespace LexIO
