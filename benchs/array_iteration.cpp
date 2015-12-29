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
#include <triqs/arrays/linalg/det_and_inverse.hpp>
#include <benchmark/benchmark.h>

using namespace triqs::arrays;
using namespace triqs;
const int N1 = 200, N2 = 300;

static void foreach (benchmark::State& state) {
 triqs::arrays::matrix<double> a(N1, N2, FORTRAN_LAYOUT);
 auto l = [&a](auto x0, auto x1) { a(x0, x1) = 10 * x0 + x1; };
 while (state.KeepRunning()) {
  foreach (a, l)
   ;
 }
}
BENCHMARK(foreach);


static void iterators(benchmark::State& state) {
 triqs::arrays::matrix<double> a(N1, N2, FORTRAN_LAYOUT);
 while (state.KeepRunning()) {
  for (auto it = a.begin(); it; ++it) { *it = 10 * it.indices()[0] + it.indices()[1]; }
 }
}
BENCHMARK(iterators);


static void pointer_generic_form(benchmark::State& state) {
 triqs::arrays::matrix<double> A(N1, N2, FORTRAN_LAYOUT);
 using int_type = int;
 const int_type s0 = A.indexmap().strides()[0];
 const int_type s1 = A.indexmap().strides()[1];
 const int_type l0 = A.indexmap().lengths()[0];
 const int_type l1 = A.indexmap().lengths()[1];
 while (state.KeepRunning()) {
  double* restrict p = &(A(0, 0));
  for (int_type i0 = 0; i0 < l0; ++i0)
   for (int_type i1 = 0; i1 < l1; ++i1) p[i0 * s0 + i1 * s1] = 10 * i0 + i1;
 }
}
BENCHMARK(pointer_generic_form);


static void foreach_manual_simple1(benchmark::State& state) {
 triqs::arrays::array<double, 2> a(N1, N2, FORTRAN_LAYOUT);
 auto l = [&a](auto x0, auto x1) { a(x0, x1) = 10 * x0 + x1; };
 mini_vector<int, 2> t;
 const int u[2] = {1, 0}; //=1, u1=0;
 while (state.KeepRunning()) {
  for (t[u[0]] = 0; t[u[0]] < a.shape(u[0]); ++t[u[0]])
   for (t[u[1]] = 0; t[u[1]] < a.shape(u[1]); ++t[u[1]])
    // l(t[u[1]], t[u[0]]);
    a(t[u[1]], t[u[0]]) = 10 * t[u[1]] + t[u[0]];
 }
}
BENCHMARK(foreach_manual_simple1);

static void foreach_manual_simple(benchmark::State& state) {
 triqs::arrays::array<double, 2> a(N1, N2, FORTRAN_LAYOUT);
 auto l = [&a](auto x0, auto x1) { a(x0, x1) = 10 * x0 + x1; };
 mini_vector<int, 2> t;
 const int u[2] = {1, 0}; //=1, u1=0;
 while (state.KeepRunning()) {
  for (int i = 0; i < a.shape(u[0]); ++i)
   for (int j = 0; j < a.shape(u[1]); ++j) l(i, j);
 }
}
BENCHMARK(foreach_manual_simple);


BENCHMARK_MAIN();

