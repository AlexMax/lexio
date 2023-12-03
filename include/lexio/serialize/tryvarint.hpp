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

#include "./tryint.hpp"

namespace LexIO
{

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadUVarint32(uint32_t &out, READER &reader)
{
    constexpr int MAX_BYTES = 5;
    uint32_t rvo = 0;
    uint8_t b = 0;

    for (int count = 0;; count++)
    {
        if (count == MAX_BYTES)
        {
            return false;
        }
        else if (!TryReadU8<READER>(b, reader))
        {
            return false;
        }

        rvo |= static_cast<uint32_t>(b & 0x7F) << (7 * count);

        if ((b & 0x80) == 0)
        {
            break;
        }
    }

    out = rvo;
    return true;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteUVarint32(WRITER &writer, const uint32_t value)
{
    uint32_t v = value;
    while (v >= 0x80)
    {
        if (!TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v | 0x80)))
        {
            return false;
        }
        v >>= 7;
    }
    return TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v));
}

constexpr size_t UVarint32Bytes(const uint32_t value)
{
    size_t count = 1;
    uint32_t v = value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadVarint32(int32_t &out, READER &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadUVarint32<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteVarint32(WRITER &writer, const int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteUVarint32<WRITER>);
}

constexpr size_t Varint32Bytes(const int32_t value)
{
    size_t count = 1;
    uint32_t v = 0 + value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadSVarint32(int32_t &out, READER &reader)
{
    uint32_t outVal;
    if (!TryReadUVarint32<READER>(outVal, reader))
    {
        return false;
    }
    out = static_cast<int32_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteSVarint32(WRITER &writer, const int32_t value)
{
    const uint32_t var = (static_cast<uint32_t>(value) << 1) ^ static_cast<uint32_t>(value >> 31);
    return TryWriteUVarint32<WRITER>(writer, var);
}

constexpr size_t SVarint32Bytes(const int32_t value)
{
    size_t count = 1;
    uint32_t v = (static_cast<uint32_t>(value) << 1) ^ static_cast<uint32_t>(value >> 31);
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadUVarint64(uint64_t &out, READER &reader)
{
    constexpr int MAX_BYTES = 10;
    uint64_t rvo = 0;
    uint8_t b = 0;

    for (int count = 0;; count++)
    {
        if (count == MAX_BYTES)
        {
            return false;
        }
        if (!TryReadU8<READER>(b, reader))
        {
            return false;
        }

        rvo |= static_cast<uint64_t>(b & 0x7F) << (7 * count);

        if ((b & 0x80) == 0)
        {
            break;
        }
    }

    out = rvo;
    return true;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteUVarint64(WRITER &writer, const uint64_t value)
{
    uint64_t v = value;
    while (v >= 0x80)
    {
        if (!TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v | 0x80)))
        {
            return false;
        }
        v >>= 7;
    }
    return TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v));
}

constexpr size_t UVarint64Bytes(const uint64_t value)
{
    size_t count = 1;
    uint64_t v = value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadVarint64(int64_t &out, READER &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadUVarint64<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteVarint64(WRITER &writer, const int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteUVarint64<WRITER>);
}

constexpr size_t Varint64Bytes(const int64_t value)
{
    size_t count = 1;
    uint64_t v = 0 + value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadSVarint64(int64_t &out, READER &reader)
{
    uint64_t outVal;
    if (!TryReadUVarint64<READER>(outVal, reader))
    {
        return false;
    }
    out = static_cast<int64_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteSVarint64(WRITER &writer, const int64_t value)
{
    const uint64_t var = (static_cast<uint64_t>(value) << 1) ^ static_cast<uint64_t>(value >> 63);
    return TryWriteUVarint64<WRITER>(writer, var);
}

constexpr size_t SVarint64Bytes(const int64_t value)
{
    size_t count = 1;
    uint64_t v = (static_cast<uint64_t>(value) << 1) ^ static_cast<uint64_t>(value >> 63);
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

} // namespace LexIO
