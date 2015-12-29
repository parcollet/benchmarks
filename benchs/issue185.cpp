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
using namespace triqs;
using namespace triqs::gfs;
using namespace triqs::arrays;


static void wgf(benchmark::State& state) {
 const int Nfreq = state.range_x();
 double beta = 10;
 auto g = gf<imfreq, scalar_valued>{{beta, Fermion, Nfreq}};
 while (state.KeepRunning()) {
  escape(&g);
  for (int n = 0; n < Nfreq; ++n) {
   //g[n] += 1;
   g.data()(n) += 1;
   clobber();
  }
 }
}
BENCHMARK(wgf)->Arg(100)->Arg(1000)->Arg(10000);

static void tvec_complex(benchmark::State& state) {
 const int Nfreq = state.range_x();
 arrays::vector<std::complex<double>> V(Nfreq);
 while (state.KeepRunning()) {
  escape(&V);
  for (int n = 0; n < Nfreq; ++n) {
   V[n] += 1;
   clobber();
  }
 }
}
BENCHMARK(tvec_complex)->Arg(100)->Arg(1000)->Arg(10000);

static void vec_complex(benchmark::State& state) {
 const int Nfreq = state.range_x();
 std::vector<std::complex<double>> V(Nfreq);
 while (state.KeepRunning()) {
  escape(&V);
  for (int n = 0; n < Nfreq; ++n) {
   V[n] += 1;
   clobber();
  }
 }
}
BENCHMARK(vec_complex)->Arg(100)->Arg(1000)->Arg(10000);

BENCHMARK_MAIN();
