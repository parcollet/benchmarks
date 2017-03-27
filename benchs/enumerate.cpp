
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
#include <benchmark/benchmark.h>
#include <triqs/gfs.hpp>
using namespace triqs::arrays;
using namespace triqs;

const int N = 100;

static void explicit_loops(benchmark::State &state) {

 auto arr = array<double, 1>(N);

 for (int i = 0; i < N; ++i) arr[i] += i;

}
BENCHMARK(explicit_loops);

static void enumerate_loops(benchmark::State &state) {

 auto arr = array<double, 1>(N);

 for (auto [i, a] : enumerate(arr)) a += i;

}
BENCHMARK(enumerate_loops);

BENCHMARK_MAIN();
