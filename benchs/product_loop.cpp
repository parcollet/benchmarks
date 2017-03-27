
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
#include "./common.hpp"
#include "./product.hpp"
#include <benchmark/benchmark.h>
#include <vector>

static void explicit_loops(benchmark::State &state) {
 std::vector<int> v = {1, 2, 3, 4};

 int r = 0;
 for (auto &x : v)
  for (auto &y : v) {
   escape(&x);
   escape(&y);
   r = r + x + y;
   clobber();
  }
}
BENCHMARK(explicit_loops);

static void product_loops(benchmark::State &state) {
 std::vector<int> v = {1, 2, 3, 4};

 int r = 0;
 for (auto[x, y] : product(v, v)) {
  escape(&x);
  escape(&y);
  r = r + x + y;
  clobber();
 }
}
BENCHMARK(product_loops);

BENCHMARK_MAIN();
