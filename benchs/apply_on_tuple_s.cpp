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
#include <triqs/utility/tuple_tools.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"
using namespace triqs;
#include <functional>

struct fun {
 double operator()(int i, double x, double y, int k) { return k + i + x + 2 * y; }
};

static void apply(benchmark::State& state) {
 fun F;
 double s=0;
 auto t = std::make_tuple(1, 2.3, 4.3, 8);
 while (state.KeepRunning()) {
  escape(&s);
  s += triqs::tuple::apply(F, t);
  clobber();
 }
}
BENCHMARK(apply);

static void manual_code(benchmark::State& state) {
 fun F;
 double s=0;
 auto t = std::make_tuple(1, 2.3, 4.3, 8);
 while (state.KeepRunning()) {
  escape(&s);
  s += F(1, 2.3, 4.3, 8);
  clobber();
 }
}
BENCHMARK(manual_code);

BENCHMARK_MAIN();

