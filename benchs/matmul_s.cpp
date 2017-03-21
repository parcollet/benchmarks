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
#include<cmath>
#include <limits>

using namespace triqs::arrays;
using namespace triqs::clef;
using namespace triqs;


static void A_x_B(benchmark::State& state) {
 const int L = state.range_x();
 matrix<double> A(L, L), B(L, L), C(L, L);
 A() = 0.01;
 //B() =  std::numeric_limits<double>::quiet_NaN(); //1.2;
 B() =  1.2;
 C() = 0;
 while (state.KeepRunning()) { C = A * B; }
}
BENCHMARK(A_x_B)->Arg(30)->Arg(100)->Arg(300)->Arg(1000);


static void clone_A_x_B(benchmark::State& state) {
 const int L = state.range_x();
 matrix<double> A(L, L), B(L, L), C(L, L);
 A() = 0.01;
 B() = 1.2;
 C() = 0;
 while (state.KeepRunning()) { A = make_clone(A) * B; }
}
BENCHMARK(clone_A_x_B)->Arg(30)->Arg(100)->Arg(300)->Arg(1000);


static void A_x_B_gemm(benchmark::State& state) {
 const int L = state.range_x();
 matrix<double> A(L, L), B(L, L), C(L, L);
 A() = 0.01;
 B() = 1.2;
 C() = 0;
 while (state.KeepRunning()) { blas::gemm(1.0, A, B, 0.0, C); }
}
BENCHMARK(A_x_B_gemm)->Arg(30)->Arg(100)->Arg(300)->Arg(1000);


static void two_A_x_B(benchmark::State& state) {
 const int L = state.range_x();
 matrix<double> A(L, L), B(L, L), C(L, L);
 A() = 0.01;
 B() = 1.2;
 C() = 0;
 while (state.KeepRunning()) { C = 2 * A * B; }
}
BENCHMARK(two_A_x_B)->Arg(30)->Arg(100)->Arg(300)->Arg(1000);


static void two_A_x_B_gemm(benchmark::State& state) {
 const int L = state.range_x();
 matrix<double> A(L, L), B(L, L), C(L, L);
 A() = 0.01;
 B() = 1.2;
 C() = 0;
 while (state.KeepRunning()) { blas::gemm(2.0, A, B, 0.0, C); }
}
BENCHMARK(two_A_x_B_gemm)->Arg(30)->Arg(100)->Arg(300)->Arg(1000);


BENCHMARK_MAIN();
