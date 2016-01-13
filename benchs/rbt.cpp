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
#include <triqs/mc_tools.hpp>
#include <triqs/arrays.hpp>
#include <triqs/arrays/linalg/det_and_inverse.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"
#include "./rbt.hpp"
#include "./rbt_iterators.hpp"
#include <map>

using namespace triqs::utility;
using namespace triqs::arrays;
using namespace triqs::mc_tools;
using namespace triqs;

static void with_map(benchmark::State& state) {
 const int L = state.range_x();
 std::map<int, int> m;
 random_generator rng;
 while (state.KeepRunning()) {
  for (int j = 0; j < L; ++j) {
   escape(&m);
   m.insert({rng(L), j});
   clobber();
  }
 }
}
BENCHMARK(with_map)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

struct _int {
 int x;
};

static void with_tree(benchmark::State& state) {
 const int L = state.range_x();
 random_generator rng;
 rb_tree<int, _int> m;
 while (state.KeepRunning()) {
  for (int j = 0; j < L; ++j) {
   escape(&m);
   int u = rng(L);
   m.insert(u, {j});
   clobber();
  }
 }
}
BENCHMARK(with_tree)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

static void just_rng(benchmark::State& state) {
 const int L = state.range_x();
 random_generator rng;
 std::map<int, int> m;
 int u;
 while (state.KeepRunning()) {
  for (int j = 0; j < L; ++j) {
   escape(&u);
   u = rng(L);
   clobber();
  }
 }
}
BENCHMARK(just_rng)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);


static void iter_map(benchmark::State& state) {
 const int L = state.range_x();
 random_generator rng;
 std::map<int, int> m;
 for (int j = 0; j < L; ++j) { m.insert({rng(L), j}); }
 int u;
 while (state.KeepRunning()) {
  for (auto const& x : m) {
   escape(&u);
   u += x.first;
   clobber();
  }
 }
}
BENCHMARK(iter_map)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

static void iter_tree(benchmark::State& state) {
 const int L = state.range_x();
 random_generator rng;
 rb_tree<int, _int> m;
 for (int j = 0; j < L; ++j) {
  int u = rng(L);
  m.insert(u, {j});
 }
 int u;
 while (state.KeepRunning()) {
  for (auto const& n : m) {
   escape(&u);
   u += n->key;
   clobber();
  }
 }
}
BENCHMARK(iter_tree)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

static void iter_tree_flatten(benchmark::State& state) {
 const int L = state.range_x();
 random_generator rng;
 rb_tree<int, _int> m;
 for (int j = 0; j < L; ++j) {
  int u = rng(L);
  m.insert(u, {j});
 }
 int u;
 while (state.KeepRunning()) {
  auto nodes = flatten2(m);
   escape(&nodes);
  /*for (auto const& n : nodes) {
   escape(&u);
   u += n->key;
   clobber();
  }*/
   clobber();  
 }
}
BENCHMARK(iter_tree_flatten)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);


BENCHMARK_MAIN();

