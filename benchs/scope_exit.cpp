/*******************************************************************************
 *
 * TRIQS: a Toolbox for Research in Interacting Quantum Systems
 *
 * Copyright (C) 2016 by O. Parcollet
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
#include <triqs/utility/scope_guard.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"

using namespace triqs::utility;
using namespace triqs;

static void no_scope_exit1(benchmark::State& state) {
 while (state.KeepRunning()) {
  int a = 0;
  escape(&a);
  {
  }
  clobber();
 }
}
BENCHMARK(no_scope_exit1);

static void no_scope_exit(benchmark::State& state) {
 while (state.KeepRunning()) {
  int a = 0;
  escape(&a);
  {
   a = 3;
  }
  clobber();
 }
}
BENCHMARK(no_scope_exit);

static void scope_exit(benchmark::State& state) {
 while (state.KeepRunning()) {
  int a = 0;
  escape(&a);
  {
   SCOPE_EXIT { a = 3; };
  }
  clobber();
 }
}
BENCHMARK(scope_exit);

BENCHMARK_MAIN();

