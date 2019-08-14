#include <benchmark/benchmark.h>
#include <complex>
#include <cstdlib>
#include <cstring>


using scalar_t = std::complex<double>;

static void calloc(benchmark::State &state) {
  std::size_t N = state.range(0);
  for (auto _ : state) {
    auto arr = (scalar_t*) std::calloc(N, sizeof(scalar_t));
    benchmark::DoNotOptimize(arr[1] = 42);
  }
}
BENCHMARK(calloc)->RangeMultiplier(1 << 5)->Range(1 << 10, 1 << 25);

static void malloc_memset(benchmark::State &state) {
  std::size_t N = state.range(0);
  for (auto _ : state) {
    int N_bytes = N * sizeof(scalar_t);
    auto arr = (scalar_t*) std::memset(std::malloc(N_bytes), 0, N_bytes);
    benchmark::DoNotOptimize(arr[1] = 42);
  }
}
BENCHMARK(malloc_memset)->RangeMultiplier(1 << 5)->Range(1 << 10, 1 << 25);

BENCHMARK_MAIN();
