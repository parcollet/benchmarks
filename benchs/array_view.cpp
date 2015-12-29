/*******************************************************************************
 *
 * TRIQS: a Toolbox for Research in Interacting Quantum Systems
 *
 * Copyright (C) 2011 by O. Parcollet
 *
 * TRIQS is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * TRIQS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * TRIQS. If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#include <triqs/arrays.hpp>
#include <triqs/arrays/matrix_tensor_proxy.hpp>
#include <benchmark/benchmark.h>
using namespace triqs::arrays;
using namespace triqs::clef;
using namespace triqs;

// using VALUE_TYPE=double ;
using VALUE_TYPE = int;
inline VALUE_TYPE fnt(size_t i) { return i * (i + 2.0) * (i - 8.0); }
// inline VALUE_TYPE fnt(size_t i) { return i;} //*(i+2.0)*(i-8);}
// inline VALUE_TYPE fnt(size_t i) { return i*(i+2.0)*(i-8);}

static void view(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, 2, 2);
 A() = 0;
 auto _ = range();
 while (state.KeepRunning()) {
  for (int i = 0; i < N - 1; ++i) A(i, _, _)(0, 0) = fnt(i);
 }
}
BENCHMARK(view)->Arg(30)->Arg(300);

static void proxy(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, 2, 2);
 A() = 0;
 while (state.KeepRunning()) {
  for (int i = 0; i < N - 1; ++i) make_tensor_proxy(A, i)(0, 0) = fnt(i);
 }
}
BENCHMARK(proxy)->Arg(30)->Arg(300);


static void direct(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, 2, 2);
 A() = 0;
 while (state.KeepRunning()) {
  for (int i = 0; i < N - 1; ++i) A(i, 0, 0) = fnt(i);
 }
}
BENCHMARK(direct)->Arg(30)->Arg(300);


static void direct_1d(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 1> A(N);
 A() = 0;

 while (state.KeepRunning()) {
  for (int i = 0; i < N - 1; ++i) A(i) = fnt(i);
 }
}
BENCHMARK(direct_1d)->Arg(30)->Arg(300);


BENCHMARK_MAIN();

