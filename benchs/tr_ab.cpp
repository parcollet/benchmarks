#include <triqs/arrays.hpp>
#include <triqs/arrays/linalg/det_and_inverse.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"
using namespace triqs::arrays;
using namespace triqs;
//const int N1 = 2000 + int(8 * std::sin(0)), N2 = 300 + int(8 * std::sin(0));

static void with_explicit_code(benchmark::State& state) {
 const int N = state.range_x();
 matrix<double> A(N, N), B(N, N);
 while (state.KeepRunning()) {

  double r = 0;

  escape(&A);
  escape(&B);
  escape(&r);

  for (int i = 0; i < N; ++i) r += A(i, i) + B(i, i);
  
  clobber();
 }
}
BENCHMARK(with_explicit_code)->Arg(100)->Arg(200);//->Arg(400)->Arg(800);

static void with_trace(benchmark::State& state) {
 const int N1 = state.range_x();
matrix<double> A(N1, N1), B(N1, N1);
 while (state.KeepRunning()) {
  double r = 0;
  escape(&A);
  escape(&B);
  escape(&r);

  r = trace(A + B);
  
  clobber();
 }
}
BENCHMARK(with_trace)->Arg(100)->Arg(200);//->Arg(400)->Arg(800);

static void stupid(benchmark::State& state) {
 const int N1 = state.range_x();
matrix<double> A(N1, N1), B(N1, N1);
 while (state.KeepRunning()) {
  double r = 0;
  escape(&A);
  escape(&B);
  escape(&r);

  r = trace(matrix<double>{A + B});
 
  clobber();
 }
}
BENCHMARK(stupid)->Arg(100)->Arg(200);//->Arg(400)->Arg(800);

/*

template <typename M> auto trace2(M const& m) {
 auto r = typename M::value_type{};
 auto d = first_dimv2(m);
 escape(&d);
 // auto d = first_dimv2(m);
 for (int i = 0; i < d; ++i) r += m(i, i);
 escape(&r);
 return r;
}

static void with_trace2(benchmark::State& state) {
 const int N1 = state.range_x();
 matrix<double> A(N1, N1), B(N1, N1);
 while (state.KeepRunning()) {
  double r = 0;
  escape(&A);
  escape(&B);
  escape(&r);
  r = trace2(A + B);
  clobber();
 }
}
BENCHMARK(with_trace2);
*/

BENCHMARK_MAIN();

