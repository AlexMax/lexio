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

#include <benchmark/benchmark.h>

#include "lexio/lexio.hpp"
#include <sstream>

//******************************************************************************

constexpr size_t READ_ITERS = 128;

static void Bench_ReadStringStream(benchmark::State &state)
{
    std::stringstream stream;
    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        stream.write((const char *)&data[0], sizeof(data));
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        stream.seekg(0);
        std::memset(&data[0], 0x00, sizeof(data));
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            stream.read((char *)&data[0], sizeof(data));
        }
        benchmark::DoNotOptimize((char *)&data[0]);
    }
}
BENCHMARK(Bench_ReadStringStream);

static void Bench_ReadLexIO(benchmark::State &state)
{
    LexIO::VectorStream stream;
    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        LexIO::Write(stream, data);
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        LexIO::Rewind(stream);
        std::memset(&data[0], 0x00, sizeof(data));
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            LexIO::Read(data, stream);
        }
        benchmark::DoNotOptimize((char *)&data[0]);
    }
}
BENCHMARK(Bench_ReadLexIO);

//******************************************************************************

constexpr size_t WRITE_ITERS = 128;

static void Bench_WriteStringStream(benchmark::State &state)
{
    std::stringstream stream;
    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};

    for (auto _ : state)
    {
        state.PauseTiming();
        std::string buffer;
        buffer.reserve(sizeof(data) * WRITE_ITERS);
        stream.str(std::move(buffer));
        state.ResumeTiming();

        for (size_t i = 0; i < WRITE_ITERS; i++)
        {
            stream.write((const char *)&data[0], sizeof(data));
        }
    }
}
BENCHMARK(Bench_WriteStringStream);

static void Bench_WriteLexIO(benchmark::State &state)
{
    LexIO::VectorStream stream;
    uint8_t data[] = {'X', 'Y', 'Z', 'Z', 'Y'};

    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<uint8_t> buffer;
        buffer.reserve(sizeof(data) * WRITE_ITERS);
        stream = LexIO::VectorStream{std::move(buffer)};
        state.ResumeTiming();

        for (size_t i = 0; i < WRITE_ITERS; i++)
        {
            LexIO::Write(stream, data);
        }
    }
}
BENCHMARK(Bench_WriteLexIO);

//******************************************************************************

static void Bench_TryReadU8(benchmark::State &state)
{
    LexIO::VectorStream stream;
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        LexIO::WriteU8(stream, uint8_t('X'));
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        LexIO::Rewind(stream);
        uint8_t data = 0;
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            LexIO::TryReadU8(data, stream);
        }
        benchmark::DoNotOptimize(data);
    }
}
BENCHMARK(Bench_TryReadU8);

static void Bench_ReadU8(benchmark::State &state)
{
    LexIO::VectorStream stream;
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        LexIO::WriteU8(stream, uint8_t('X'));
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        LexIO::Rewind(stream);
        uint8_t data = 0;
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            data = LexIO::ReadU8(stream);
        }
        benchmark::DoNotOptimize(data);
    }
}
BENCHMARK(Bench_ReadU8);

//******************************************************************************

static void Bench_TryReadU16LE(benchmark::State &state)
{
    LexIO::VectorStream stream;
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        LexIO::TryWriteU16LE(stream, 0xBEEF);
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        LexIO::Rewind(stream);
        uint16_t data = 0;
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            LexIO::TryReadU16LE(data, stream);
        }
        benchmark::DoNotOptimize(data);
    }
}
BENCHMARK(Bench_TryReadU16LE);

static void Bench_ReadU16LE(benchmark::State &state)
{
    LexIO::VectorStream stream;
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        LexIO::WriteU16LE(stream, 0xBEEF);
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        LexIO::Rewind(stream);
        uint16_t data = 0;
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            data = LexIO::ReadU16LE(stream);
        }
        benchmark::DoNotOptimize(data);
    }
}
BENCHMARK(Bench_ReadU16LE);

//******************************************************************************

static void Bench_TryReadU32LE(benchmark::State &state)
{
    LexIO::VectorStream stream;
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        LexIO::TryWriteU32LE(stream, 0xDEADBEEF);
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        LexIO::Rewind(stream);
        uint32_t data = 0;
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            LexIO::TryReadU32LE(data, stream);
        }
        benchmark::DoNotOptimize(data);
    }
}
BENCHMARK(Bench_TryReadU32LE);

static void Bench_ReadU32LE(benchmark::State &state)
{
    LexIO::VectorStream stream;
    for (size_t i = 0; i < READ_ITERS; i++)
    {
        LexIO::WriteU32LE(stream, 0xDEADBEEF);
    }

    for (auto _ : state)
    {
        state.PauseTiming();
        LexIO::Rewind(stream);
        uint32_t data = 0;
        state.ResumeTiming();

        for (size_t i = 0; i < READ_ITERS; i++)
        {
            data = LexIO::ReadU32LE(stream);
        }
        benchmark::DoNotOptimize(data);
    }
}
BENCHMARK(Bench_ReadU32LE);

BENCHMARK_MAIN();
