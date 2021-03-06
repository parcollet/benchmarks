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
#include <triqs/clef.hpp>
#include "./clef_common.hpp"
#include <benchmark/benchmark.h>
#include "./common.hpp"
using namespace triqs;

static void eval_exp(benchmark::State& state) {
 F1 f(2);
 double y = 2;
 auto expr = f(x_) + 2 * y_;
 double s = 0;
 while (state.KeepRunning()) {
  escape(&s);
  s += eval(expr, x_ = 1 / 10000.0, y_ = y);
  clobber();
 }
}
BENCHMARK(eval_exp);


static void eval_manual(benchmark::State& state) {
 F1 f(2);
 double x = 1, y = 2;
 double s = 0;
 while (state.KeepRunning()) {
  escape(&s);
  x = 1 / 10000.0;
  s += f(x) + 2 * y;
  clobber();
 }
}
BENCHMARK(eval_manual);

/*
static void eval_exp1(benchmark::State& state) {
 F1 f(2);
 triqs::clef::function<double(double)> f7;
 f7(x_) = f(x_) + 2 * x_;
 triqs::clef::function<double(double)> f2;
 f2(x_) = f(x_) + 2 * x_;
 double s = 0;
 while (state.KeepRunning()) {
  escape(&s);
  s += f2(1 / 10000.0);
  clobber();
 }
}
BENCHMARK(eval_exp1);
*/

BENCHMARK_MAIN();
