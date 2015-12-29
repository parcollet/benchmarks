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
#include <benchmark/benchmark.h>
using namespace std;
using namespace triqs::arrays;

static void expression_template(benchmark::State& state) {
 const int N = state.range_x();
 matrix<double> A(N, N);
 for (int i = 0; i < N; ++i)
  for (int j = 0; j < N; ++j) A(i, j) = 10 * i + j;
 auto B = A, C = A, D = A;

 while (state.KeepRunning()) { A = B + 3 * C + D; }
}
BENCHMARK(expression_template)->Arg(30)->Arg(300);


static void bare_loop_with_access(benchmark::State& state) {
 const int N = state.range_x();
 matrix<double> A(N, N);
 for (int i = 0; i < N; ++i)
  for (int j = 0; j < N; ++j) A(i, j) = 10 * i + j;
 auto B = A, C = A, D = A;

 while (state.KeepRunning()) {
  for (int i = 0; i < N; ++i)
   for (int j = 0; j < N; ++j) A(i, j) = B(i, j) + 3 * C(i, j) + D(i, j);
 }
}
BENCHMARK(bare_loop_with_access)->Arg(30)->Arg(300);

BENCHMARK_MAIN();

