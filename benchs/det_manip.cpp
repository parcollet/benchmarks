#include "./common.hpp"
#include "./my_determinant.hpp"
#include <benchmark/benchmark.h>
#include <triqs/det_manip/det_manip.hpp>
using namespace triqs::arrays;
using namespace triqs;

struct fun {
 double operator()(double x, double y) const { return 1. / (x + y + 2); }
};


static void detmanip(benchmark::State& state) {
 const int N = state.range(0);
 fun f;

 triqs::det_manip::det_manip<fun> dets(f, N + 10);

 for (int j = 0; j < N; ++j) {
  double detratio = dets.try_insert(j, j, j, j);
  dets.complete_operation();
 }

 while (state.KeepRunning()) {
  auto d2 = dets;
  auto detratio = d2.try_insert(N, N, N, N);
  escape(&detratio);
  // dets.reject_last_try();
  d2.complete_operation();
 }
}

static void detmanip1(benchmark::State& state) {
 const int N = state.range(0);
 fun f;

 triqs::det_manip::det_manip<fun> dets(f, N + 10);

 for (int j = 0; j < N; ++j) {
  double detratio = dets.try_insert(j, j, j, j);
  dets.complete_operation();
 }

 while (state.KeepRunning()) {
  auto d2 = dets;
  benchmark::DoNotOptimize(d2);
  //auto detratio = d2.try_insert(N, N, N, N);
  //escape(&detratio);
  // dets.reject_last_try();
  //d2.complete_operation();
 }
}

static void detmanip_try_only(benchmark::State& state) {
 const int N = state.range(0);
 fun f;

 triqs::det_manip::det_manip<fun> dets(f, N + 10);

 for (int j = 0; j < N; ++j) {
  double detratio = dets.try_insert(j, j, j, j);
  dets.complete_operation();
 }

 while (state.KeepRunning()) {
  auto detratio = dets.try_insert(N, N, N, N);
  benchmark::DoNotOptimize(detratio);
  dets.reject_last_try();
 }
}


static void withdet(benchmark::State& state) {
 const int n = state.range(0);
 fun f;

 triqs::arrays::matrix<double> A(n + 1, n + 1);
 for (int j = 0; j < n + 1; ++j) {
  for (int k = 0; k < n + 1; ++k) { A(j, k) = f(j, k); }
 }
 double s = 0;

 while (state.KeepRunning()) {
  for (int k = 0; k < n + 1; ++k) {
   A(n, k) = f(n, k);
   A(k, n) = f(k, n);
  }
  //auto cA = A;
  //benchmark::DoNotOptimize(cA);
  benchmark::DoNotOptimize(s += determinant(A));
 }
 benchmark::ClobberMemory();
}


static void withmydet(benchmark::State& state) {
 const int n = state.range(0);
 fun f;

 triqs::arrays::matrix<double> A(n + 1, n + 1);
 for (int j = 0; j < n + 1; ++j) {
  for (int k = 0; k < n + 1; ++k) { A(j, k) = f(j, k); }
 }
 my_determinant my_det(n + 1);
 double s = 0;

 while (state.KeepRunning()) {
  for (int k = 0; k < n + 1; ++k) {
   A(n, k) = f(n, k);
   A(k, n) = f(k, n);
  }
  s += my_det.det(&A, n + 1);
  escape(&s);
 }
}


BENCHMARK(detmanip1)->Arg(6)->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(20)->Arg(100)->Arg(500);
BENCHMARK(detmanip)->Arg(6)->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(20)->Arg(100)->Arg(500);
BENCHMARK(detmanip_try_only)->Arg(6)->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(20)->Arg(100)->Arg(500);
BENCHMARK(withdet)->Arg(6)->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(20)->Arg(100)->Arg(500);
BENCHMARK(withmydet)->Arg(6)->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(20)->Arg(100)->Arg(500);


BENCHMARK_MAIN();
