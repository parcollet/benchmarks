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
using namespace triqs::arrays;
using namespace triqs::gfs;
using namespace triqs;

const int N = 1000;

using VALUE_TYPE = double;
// using VALUE_TYPE=int ;
// inline VALUE_TYPE fnt(size_t i) { return i*(i+2.0)*(i-8.0);}
inline VALUE_TYPE fnt(size_t i) { return i; } //*(i+2.0)*(i-8);}
// inline VALUE_TYPE fnt(size_t i) { return i*(i+2.0)*(i-8);}

static void with_g(benchmark::State& state) {
 const int N = state.range_x();
 double beta = 1;
 auto G = gf<imfreq>{{beta, Fermion, N}, {2, 2}};
 G() = 0;
 while (state.KeepRunning()) {
  // for (int i =0; i<N-1; ++i) G.on_mesh(i)(0,0) = fnt(i);
  for (int i = 0; i < N - 1; ++i) G[i](0, 0) = fnt(i);
 }
}
BENCHMARK(with_g)->Arg(30)->Arg(300)->Arg(3000);

static void direct_with_array(benchmark::State& state) {
 const int N = state.range_x();
 double beta = 1;
 auto G = gf<imfreq>{{beta, Fermion, N}, {2, 2}};
 G() = 0;
 auto V = G.data();
 while (state.KeepRunning()) {
  for (int i = 0; i < N - 1; ++i) V(i, 0, 0) = fnt(i);
 }
}
BENCHMARK(direct_with_array)->Arg(30)->Arg(300)->Arg(3000);

BENCHMARK_MAIN();

