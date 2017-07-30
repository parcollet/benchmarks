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
#include "./common.hpp"
using namespace triqs::arrays;
using namespace triqs;

static void simple_loop(benchmark::State& state) {
 const int N = state.range_x();
 while (state.KeepRunning()) {
  long a = 0;
  for (int i = 0; i < N; i++) {
   escape(&a);
   a += i;
   clobber();
  }
 }
}
BENCHMARK(simple_loop)->Arg(30)->Arg(300)->Arg(3000)->Arg(30000);

static void loop_with_range(benchmark::State& state) {
 const int N = state.range_x();
 while (state.KeepRunning()) {
  long a = 0;
  for (auto i : range(0,N)) {
   escape(&a);
   a += i;
   clobber();
  }
 }
}
BENCHMARK(loop_with_range)->Arg(30)->Arg(300)->Arg(3000)->Arg(30000);


BENCHMARK_MAIN();

