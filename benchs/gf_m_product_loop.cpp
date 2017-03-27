
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
using namespace triqs::gfs;
using namespace triqs;

const int n_iw = 32;
const double beta = 10;

using M4_t = gf<cartesian_product<imfreq, imfreq, imfreq>, tensor_valued<4>>;

const auto iw_mesh = gf_mesh<imfreq>{beta, Fermion, n_iw};

const int Nc = 2;

static void explicit_loops(benchmark::State &state) {
 auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {2, 2, 2, 2}};

 int Nc = M4.target_shape()[0];
 while (state.KeepRunning()) {
  for (auto const &iw1 : iw_mesh)
   for (auto const &iw2 : iw_mesh)
    for (auto const &iw3 : iw_mesh) {

     auto P = M4[{iw1, iw2, iw3}];

     for (int i : range(Nc))
      for (int j : range(Nc))
       for (int k : range(Nc))
        for (int l : range(Nc)) {

         escape(&M4);
         escape(&P);
         P(i, j, k, l) = 2.0;
         clobber();
        }
    }
 }
}
BENCHMARK(explicit_loops);

static void compact_loops(benchmark::State &state) {
 auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {2, 2, 2, 2}};
 auto &M4_data = M4.data();
 int Nc = M4.target_shape()[0];


 while (state.KeepRunning())
  // Work directly with cartesian_product mesh??
  for (auto[iw1, iw2, iw3] : product(iw_mesh, iw_mesh, iw_mesh)) {

   auto P = M4[{iw1, iw2, iw3}];

   // for (auto[i, j, k, l] : prod_range(Nc, Nc, Nc, Nc)) { ???
   for (auto[i, j, k, l] : product(range(Nc), range(Nc), range(Nc), range(Nc))) {
    escape(&M4_data);
    M4_data(iw1, iw2, iw3, i, j, k, l) = 0.0;
    clobber();
   }
  }
}
BENCHMARK(explicit_loops_data);

BENCHMARK_MAIN();
