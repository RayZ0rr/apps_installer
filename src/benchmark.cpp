#include <benchmark/benchmark.h>
#include "filetools.hpp"
#include "process.hpp"
#include "stringtools.hpp"
#include "misctools.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <chrono>

namespace UP = Utilities::Process;
namespace UMT = Utilities::MiscTools;

// Register the function as a benchmark
static void BM_Printer1(benchmark::State &state)
{
  for (auto _ : state) {
    const std::string &args_vect{ "fd . ~" };
    UP::CommandPrint1(args_vect);
  }
}
BENCHMARK(BM_Printer1);
// ------------------------------------------------
static void BM_Printer2(benchmark::State &state)
{
  for (auto _ : state) {
    const std::string &args_vect{ "fd . ~" };
    UP::CommandPrint2(args_vect);
  }
}
BENCHMARK(BM_Printer2);
// ------------------------------------------------
static void BM_Printer3(benchmark::State &state)
{
  for (auto _ : state) {
    const std::string &args_vect{ "fd . ~" };
    UP::CommandPrint3(args_vect);
  }
}
BENCHMARK(BM_Printer3);
// ------------------------------------------------
static void BM_Printer4(benchmark::State &state)
{
  for (auto _ : state) {
    const std::string &args_vect{ "fd . ~" };
    UP::CommandPrint4(args_vect);
  }
}
BENCHMARK(BM_Printer4);
// ------------------------------------------------
static void BM_Printer5(benchmark::State &state)
{
  for (auto _ : state) {
    UMT::RandomGen();
  }
}
BENCHMARK(BM_Printer5);
// ------------------------------------------------

BENCHMARK_MAIN();
