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
#include <triqs/gfs.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"
#include <iostream>

struct obj {
 int operator()(int i, int j, int k, int l) { return i + 10 * j + 100 * k + 1000 * l; }
};

template <typename R, typename A> struct proxy {
 R r;
 A& a;
 template <typename... Args> int operator()(Args&&... args) {
  auto f = [this, args...](auto... x) { return this->a(x..., args...); };
  return r(f);
 }
};

template <typename A, typename... T> auto make_proxy(A& a, T&&... x) {
 auto r = [=](auto f) { return f(x...); };
 return proxy<decltype(r), A>{r, a};
}

static void simple(benchmark::State& state) {
 int a;
 while (state.KeepRunning()) {
  escape(&a);
  a = obj{}(1,2,3,4);
  clobber();
 }
}
BENCHMARK(simple);

static void delay(benchmark::State& state) {
 obj X;
 int a;
 while (state.KeepRunning()) {
  escape(&a);
  auto p = make_proxy(X, 1, 2);
  a = p(3,4);
  clobber();
 }
}
BENCHMARK(delay);

BENCHMARK_MAIN();




