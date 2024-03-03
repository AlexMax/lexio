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

#include "lexio/bufreader.hpp"
#include "lexio/stream/vector.hpp"
#include "lexio/stream/view.hpp"

#include <gtest/gtest.h>

template <typename T, std::size_t N>
constexpr std::size_t CountOf(T const (&)[N]) noexcept
{
    return N;
}

constexpr uint8_t TEST_TEXT_DATA[] = "The quick brown fox\njumps over the lazy dog.\n";
constexpr size_t TEST_TEXT_LENGTH = CountOf(TEST_TEXT_DATA) - sizeof('\0');

inline LexIO::VectorStream GetVectorStream()
{
    return LexIO::VectorStream{std::vector<uint8_t>{&TEST_TEXT_DATA[0], &TEST_TEXT_DATA[TEST_TEXT_LENGTH]}};
}

template <size_t N>
inline LexIO::ViewStream GetViewStream(uint8_t (&array)[N])
{
    std::memcpy(&array[0], &TEST_TEXT_DATA[0], N);
    return LexIO::ViewStream{&array[0], &array[N]};
}

inline LexIO::ConstViewStream GetConstViewStream()
{
    return LexIO::ConstViewStream{&TEST_TEXT_DATA[0], &TEST_TEXT_DATA[TEST_TEXT_LENGTH]};
}

template <typename STREAM>
class PartialStream
{
    STREAM m_stream;

  public:
    PartialStream() = delete;
    PartialStream(STREAM &&stream) : m_stream(stream) {}

    const STREAM &Stream() const { return m_stream; }

    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        return m_stream.LexRead(outDest, std::min<size_t>(count, 4));
    }

    size_t LexWrite(const uint8_t *src, const size_t count)
    {
        return m_stream.LexWrite(src, std::min<size_t>(count, 4));
    }

    void LexFlush() { m_stream.LexFlush(); }

    size_t LexSeek(const LexIO::SeekPos &pos) { return m_stream.LexSeek(pos); }
};

template <typename STREAM>
class NoCopyStream
{
    STREAM m_stream;

  public:
    NoCopyStream() = default;
    NoCopyStream(const NoCopyStream &) = delete;
    NoCopyStream(NoCopyStream &&) noexcept = default;
    NoCopyStream &operator=(const NoCopyStream &) = delete;
    NoCopyStream &operator=(NoCopyStream &&) noexcept = default;

    NoCopyStream(STREAM &&stream) : m_stream(stream) {}

    const STREAM &Stream() const & { return m_stream; }

    STREAM Stream() const && { return m_stream; }

    size_t LexRead(uint8_t *outDest, const size_t count) { return m_stream.LexRead(outDest, count); }

    LexIO::BufferView LexFillBuffer(const size_t size) { return m_stream.LexFillBuffer(size); }

    void LexConsumeBuffer(const size_t size) { return m_stream.LexConsumeBuffer(size); }

    size_t LexWrite(const uint8_t *src, const size_t count) { return m_stream.LexWrite(src, count); }

    void LexFlush() { m_stream.LexFlush(); }

    size_t LexSeek(const LexIO::SeekPos &pos) { return m_stream.LexSeek(pos); }
};

class ErrorStream
{
  public:
    ErrorStream() = default;
    ErrorStream(const ErrorStream &) = delete;
    ErrorStream(ErrorStream &&) noexcept = default;
    ErrorStream &operator=(const ErrorStream &) = delete;
    ErrorStream &operator=(ErrorStream &&) noexcept = default;

    size_t LexRead(uint8_t *, const size_t) { throw std::runtime_error("intended"); }

    LexIO::BufferView LexFillBuffer(const size_t) { throw std::runtime_error("intended"); }

    void LexConsumeBuffer(const size_t) { throw std::runtime_error("intended"); }

    size_t LexWrite(const uint8_t *, const size_t) { throw std::runtime_error("intended"); }

    void LexFlush() { throw std::runtime_error("intended"); }

    size_t LexSeek(const LexIO::SeekPos &) { throw std::runtime_error("intended"); }
};
