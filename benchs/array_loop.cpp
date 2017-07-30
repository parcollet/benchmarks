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
using namespace triqs::arrays;
using namespace triqs::clef;
using namespace triqs;

static void simple_loop(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, N, N);
 while (state.KeepRunning()) {
  for (int i = 0; i < N; ++i)
   for (int j = 0; j < N; ++j)
    for (int k = 0; k < N; ++k) A(i, j, k) = i - j + N * k;
 }
}
BENCHMARK(simple_loop)->Arg(30)->Arg(300);

static void with_clef(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, N, N); //, make_memory_layout(1,0,2));
 placeholder<0> i_;
 placeholder<1> j_;
 placeholder<2> k_;
 while (state.KeepRunning()) { A(i_, j_, k_) << i_ - j_ + N * k_; }
}
BENCHMARK(with_clef)->Arg(30)->Arg(300);

static void foreach_lambda(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, N, N); //, make_memory_layout(1,0,2));
 placeholder<0> i_;
 placeholder<1> j_;
 placeholder<2> k_;
 while (state.KeepRunning()) { 
   assign_foreach(A, [N](long i, long j, long k) { return i - j + N * k;});
 }
}
BENCHMARK(foreach_lambda)->Arg(30)->Arg(300);


static void loop_2(benchmark::State& state) {
 const int N = state.range_x();
 long ind[3];
 long Ns[3] = {N, N, N};
 array<double, 3> A(N, N, N);
 auto l = [&A, N](int i, int j, int k) { A(i, j, k) = i - j + N * k; };
 auto ll = [&A, N, &ind]() {  A(ind[0], ind[1], ind[2]) = ind[0] - ind[1] + N * ind[2]; };

 while (state.KeepRunning()) {
  for (ind[0] = 0; ind[0] < Ns[0]; ++ind[0])
   for (ind[1] = 0; ind[1] < Ns[0]; ++ind[1])
    for (ind[2] = 0; ind[2] < Ns[0]; ++ind[2]) 
     ll();
     // A(ind[0], ind[1], ind[2]) = ind[0] - ind[1] + N * ind[2];
  //l(ind[0], ind[1], ind[2]);
 }
}
BENCHMARK(loop_2)->Arg(30)->Arg(300);


static void loop_reorder(benchmark::State& state) {
 const int N = state.range_x();
 int p[3];
 for (int u = 0; u < 3; ++u) p[u] = (u + 2) % 3; // = {2, 0, 1};
 // int p[] = {0,1,2};
 long ind[3];
 long Ns[3] = {N, N, N};
 array<double, 3> A(N, N, N);

 auto l = [&A](int i, int j, int k) { A(i, j, k) = i - j + 2 * k; };

 while (state.KeepRunning()) {
  for (ind[0] = 0; ind[0] < Ns[0]; ++ind[0])
   for (ind[1] = 0; ind[1] < Ns[1]; ++ind[1])
    for (ind[2] = 0; ind[2] < Ns[2]; ++ind[2]) l(ind[p[1]], ind[p[2]], ind[p[0]]);
 }
}
BENCHMARK(loop_reorder)->Arg(30)->Arg(300);

static void loop_reorder2(benchmark::State& state) {
 const int N = state.range_x();
 int p[] = {0, 1, 2};
 int ind[3];
 int Ns[3] = {N, N, N};
 array<double, 3> A(N, N, N);

 auto l = [&A](int i, int j, int k) { A(i, j, k) = i - j + 2 * k; };

 while (state.KeepRunning()) {
  for (ind[p[0]] = 0; ind[p[0]] < Ns[p[0]]; ++ind[p[0]])
   for (ind[p[1]] = 0; ind[p[1]] < Ns[p[1]]; ++ind[p[1]])
    for (ind[p[2]] = 0; ind[p[2]] < Ns[p[2]]; ++ind[p[2]]) l(ind[0], ind[1], ind[2]);
 }
}
BENCHMARK(loop_reorder2)->Arg(30)->Arg(300);

static void iter2(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, N, N);
 auto l = [&A](int i, int j, int k) { A(i, j, k) = i - j + 2 * k; };

 while (state.KeepRunning()) {
  for (auto it = A.begin(); it; ++it) { l(it.indices()[0], it.indices()[1], it.indices()[2]); }
 }
}
BENCHMARK(iter2)->Arg(30);

// a very bad idea ... 15 x slower...
static void iter_bad(benchmark::State& state) {
 const int N = state.range_x();
 array<double, 3> A(N, N, N);
 auto l = [&A](int i, int j, int k) { A(i, j, k) = i - j + 2 * k; };
 long m = N * N * N;
 long N1 = N * N;

 while (state.KeepRunning()) {
  for (long ind = 0; ind < m; ++ind) { l(ind % N, (ind / N) % N, (ind / N1) % N); }
 }
}
BENCHMARK(iter_bad)->Arg(30);


BENCHMARK_MAIN();

