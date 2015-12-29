/*******************************************************************************
 *
 * TRIQS: a Toolbox for Research in Interacting Quantum Systems
 *
 * Copyright (C) 2011-2016 by O. Parcollet
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
#include <triqs/arrays/linalg/det_and_inverse.hpp>
#include <benchmark/benchmark.h>

using namespace triqs::arrays;
using namespace triqs;
const int N1 = 200, N2 = 300;

static void BM_plain_for_no_ptr_const(benchmark::State& state) {
 triqs::arrays::array<double, 2> A(N1, N2, FORTRAN_LAYOUT);
 while (state.KeepRunning()) {
  for (int j = 0; j < N2; ++j)
   for (int i = 0; i < N1; ++i) A(i, j) = 1876;
 }
}
BENCHMARK(BM_plain_for_no_ptr_const);

static void BM_plain_for_no_ptr(benchmark::State& state) {
 triqs::arrays::array<double, 2> A(N1, N2, FORTRAN_LAYOUT);
 while (state.KeepRunning()) {
  for (int j = 0; j < N2; ++j)
   for (int i = 0; i < N1; ++i) A(i, j) = 10 * i + j;
 }
}
BENCHMARK(BM_plain_for_no_ptr);

static void assign_to_const(benchmark::State& state) {
 triqs::arrays::array<double, 2> A(N1, N2, FORTRAN_LAYOUT);
 auto V = make_view(A);
 while (state.KeepRunning()) {
  // make_view(A) = 1867;
  V = 1867;
 }
}
BENCHMARK(assign_to_const);


static void assign_to_const_with_iterators(benchmark::State& state) {
 triqs::arrays::array<double, 2> A(N1, N2, FORTRAN_LAYOUT);
 auto V = make_view(A);
 while (state.KeepRunning()) {
  for (auto it = A.begin(); it; ++it) { *it = 1876; }
 }
}
BENCHMARK(assign_to_const_with_iterators);



BENCHMARK_MAIN();

