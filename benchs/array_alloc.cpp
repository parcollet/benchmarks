#include <benchmark/benchmark.h>
#include <triqs/arrays.hpp>
#include <complex>

using namespace triqs::arrays;
using scalar_t = std::complex<double>;

static void arr_alloc(benchmark::State &state) {
  std::size_t N = state.range(0);
  for (auto _ : state) {
    array<scalar_t, 1> A(N);
    benchmark::DoNotOptimize(A(1) = 1876);
  }
}
BENCHMARK(arr_alloc)->RangeMultiplier(1 << 5)->Range(1 << 10, 1 << 30);

static void arr_alloc_write(benchmark::State &state) {
  std::size_t N = state.range(0);
  for (auto _ : state) {
    array<scalar_t, 1> A(N);

    for (long i = 0; i < N; ++i)
        benchmark::DoNotOptimize(A(i) = 10 * i);
  }
}
BENCHMARK(arr_alloc_write)->RangeMultiplier(1 << 5)->Range(1 << 10, 1 << 30);

static void arr_zeros(benchmark::State &state) {
  std::size_t N = state.range(0);
  for (auto _ : state) {
    auto A = zeros<scalar_t>(mini_vector<int, 1>(N));
    benchmark::DoNotOptimize(A(1) = 1876);
  }
}
BENCHMARK(arr_zeros)->RangeMultiplier(1 << 5)->Range(1 << 10, 1 << 30);

BENCHMARK_MAIN();
