
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
using namespace triqs::clef;
using namespace triqs;

const int n_iw = 32;
const double beta = 10;

placeholder<0> iw1_;
placeholder<1> iw2_;
placeholder<2> iw3_;
placeholder<3> i_;
placeholder<4> j_;
placeholder<5> k_;
placeholder<6> l_;

using M4_t = gf<cartesian_product<imfreq, imfreq, imfreq>, tensor_valued<4>>;
using M_t = gf<cartesian_product<imfreq, imfreq>, tensor_valued<2>>;

const auto iw_mesh = gf_mesh<imfreq>{beta, Fermion, n_iw};
const auto iw_mesh_large = gf_mesh<imfreq>{beta, Fermion, 3 * n_iw};

const int Nc = 2;

static void pure_clef(benchmark::State &state) {
 auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {2, 2, 2, 2}};
 auto M = M_t{{iw_mesh_large, iw_mesh}, {2, 2}};

 while (state.KeepRunning()) {
  escape(&M4);
  M4(iw1_, iw2_, iw3_)(i_, j_, k_, l_) << 0.0;
  clobber();
 }
}
BENCHMARK(pure_clef);

static void partial_clef(benchmark::State &state) {
 auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {2, 2, 2, 2}};
 // auto M  = M_t{{iw_mesh_large, iw_mesh}, {2, 2}};

 while (state.KeepRunning()) {
  for (auto iw1 : iw_mesh)
   for (auto iw2 : iw_mesh)
    for (auto iw3 : iw_mesh) {

     auto P = M4[{iw1, iw2, iw3}];
     for (int u = 0; u < 1; ++u) {
          escape(&M4);
      escape(&P);
      //M4[{iw1, iw2, iw3}](i_, j_, k_, l_) << 2.0;
     
      P(i_, j_, k_, l_) << 2.0;
      //triqs_clef_auto_assign(P, [](auto i, auto j, auto k, auto l) { return 2.0;});
      
      clobber();
     }
    }
 }
}
BENCHMARK(partial_clef);

static void explicit_loops(benchmark::State &state) {
 auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {2,2,2,2}};

 int Nc = M4.target_shape()[0];
 while (state.KeepRunning()) {
  for (auto const &iw1 : iw_mesh)
   for (auto const &iw2 : iw_mesh)
    for (auto const &iw3 : iw_mesh) {
     auto P = M4[{iw1, iw2, iw3}];
     for (int u = 0; u < 1; ++u) {

      for (int i : range(Nc))
       for (int j : range(Nc))
        for (int k : range(Nc))
         for (int l : range(Nc)) {

          escape(&M4);
          escape(&P);
          //M4[{iw1, iw2, iw3}](i, j, k, l) = 2.0;
          P(i, j, k, l) = 2.0;
          clobber();
         }
     }
    }
 }
}
BENCHMARK(explicit_loops);

static void explicit_loops_data(benchmark::State &state) {
 auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {2, 2, 2, 2}};
 auto &M4_data = M4.data();
 int Nc = M4.target_shape()[0];

 while (state.KeepRunning()) {
  for (int iw1 : range(0, 2 * n_iw, 1))
   for (int iw2 : range(0, 2 * n_iw, 1))
    for (int iw3 : range(0, 2 * n_iw, 1))
     for (int u = 0; u < 1; ++u) {
     for (int i : range(Nc))
      for (int j : range(Nc))
       for (int k : range(Nc))
        for (int l : range(Nc)) {
         escape(&M4_data);
         M4_data(iw1, iw2, iw3, i, j, k, l) = 0.0;
         clobber();
	}}
 }
}
BENCHMARK(explicit_loops_data);

static void explicit_loops_data_resorted(benchmark::State &state) {
 auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {2, 2, 2, 2}};
 auto &M4_data = M4.data();

 while (state.KeepRunning()) {
  for (int i : range(2))
   for (int j : range(2))
    for (int k : range(2))
     for (int l : range(2))
      for (int iw1 : range(0, 2 * n_iw, 1))
       for (int iw2 : range(0, 2 * n_iw, 1))
        for (int iw3 : range(0, 2 * n_iw, 1)) {
         escape(&M4_data);
         M4_data(iw1, iw2, iw3, i, j, k, l) = 0.0;
         clobber();
        }
 }
}
BENCHMARK(explicit_loops_data_resorted);

BENCHMARK_MAIN();
