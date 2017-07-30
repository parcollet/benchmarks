
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

const auto iw_mesh = gf_mesh<imfreq>{beta, Fermion, n_iw};
const int Nc =3;
auto M4 = M4_t{{iw_mesh, iw_mesh, iw_mesh}, {Nc, Nc, Nc, Nc}};

auto GM1 = gf<imfreq>{iw_mesh, {Nc,Nc}};
auto GM2 = GM1;

auto M1 = gf<cartesian_product<imfreq,imfreq>, matrix_valued> { {iw_mesh, iw_mesh}, {Nc,Nc}};
auto M2 = M1;


//#define RHS_ZERO

static void pure_clef(benchmark::State &state) {
     auto iw4_ = iw1_;

 while (state.KeepRunning()) {
  escape(&M4);
#ifdef RHS_ZERO
  M4(iw1_, iw2_, iw3_)(i_, j_, k_, l_) << 0;
#else
  M4(iw1_, iw2_, iw3_)(i_, j_, k_, l_) << (M1(iw2_, iw1_)(j_, i_) * M2(iw4_, iw3_)(l_, k_) -  M1(iw2_, iw1_)(l_, i_) * M2(iw4_, iw3_)(j_, k_));
#endif
  clobber();
 }
}
BENCHMARK(pure_clef);


static void partial_clef(benchmark::State &state) {
 while (state.KeepRunning()) {
  for (auto iw1 : iw_mesh)
   for (auto iw2 : iw_mesh)
    for (auto iw3 : iw_mesh) {

     auto&& P = M4[{iw1, iw2, iw3}];

     auto iw4 = iw1;

     escape(&M4);
     escape(&P);
#ifdef RHS_ZERO
     P(i_, j_, k_, l_) << 2.0;
#else
     auto M12 = M1[iw2][iw1];
     auto M43 = M2[iw4][iw3];

     //P(i_, j_, k_, l_) << (M1[{iw2, iw1}](j_, i_) * M2[{iw4, iw3}](l_, k_) -  M1[{iw2, iw1}](l_, i_) * M2[{iw4, iw3}](j_, k_));
     auto ll = [&] (auto i, auto j, auto k, auto l) { 
                    //return  (M1[iw2][iw1](j, i) * M2[iw4][iw3](l, k) -  M1[iw2][iw1](l, i) * M2[iw4][iw3](j, k));
                    // return  eval ( (M1[iw2][iw1](j_, i_) * M2[iw4][iw3](l_, k_) -  M1[iw2][iw1](l_, i_) * M2[iw4][iw3](j_, k_)), 
                    return  eval ( (M12(j_, i_) * M43(l_, k_) -  M12(l_, i_) * M43(j_, k_)), 
		        i_ = i, j_ =j, k_=k, l_=l);
     };
    
     auto lll = triqs::clef::make_function((M1[{iw2, iw1}](j_, i_) * M2[{iw4, iw3}](l_, k_) -  M1[{iw2, iw1}](l_, i_) * M2[{iw4, iw3}](j_, k_)), 
         i_, j_, k_, l_);

     triqs_clef_auto_assign(P, ll);

#endif
     // triqs_clef_auto_assign(P, [](auto i, auto j, auto k, auto l) { return 2.0;});
     clobber();
    }
 }
}
BENCHMARK(partial_clef);

static void clef_unroll (benchmark::State &state) {
 int Nc = M4.target_shape()[0];
 while (state.KeepRunning()) {
  for (auto const &iw1 : iw_mesh)
   for (auto const &iw2 : iw_mesh)
    for (auto const &iw3 : iw_mesh) {
         escape(&M4);
    
     	 auto iw4 = iw1;
     auto&& X =  M4[{iw1, iw2, iw3}];
     escape(&X);
     auto ll = [&] (auto i, auto j, auto k, auto l) { 
                    X(i, j, k, l) = (M1[iw2][iw1](j, i) * M2[iw4][iw3](l, k) -  M1[iw2][iw1](l, i) * M2[iw4][iw3](j, k));
//                    X(i, j, k, l) = (M1[{iw2, iw1}](j, i) * M2[{iw4, iw3}](l, k) -  M1[{iw2, iw1}](l, i) * M2[{iw4, iw3}](j, k));
     };

     for (int i : range(Nc))
      for (int j : range(Nc))
       for (int k : range(Nc))
        for (int l : range(Nc)) {

	 ll(i,j,k,l); 
         clobber();
        }
    }
 }
}
BENCHMARK(clef_unroll);


static void explicit_loops1(benchmark::State &state) {
 int Nc = M4.target_shape()[0];
 while (state.KeepRunning()) {
  for (auto const &iw1 : iw_mesh)
   for (auto const &iw2 : iw_mesh)
    for (auto const &iw3 : iw_mesh) {

     auto&& P = M4[{iw1, iw2, iw3}];
     	 auto iw4 = iw1;
     
 //for (int i=0; i < Nc ; ++i) 
  //   for (int j=0; j < Nc ; ++j) 
   //  for (int k=0; k < Nc ; ++k) 
   //  for (int l=0; l < Nc ; ++l) 
   
      for (int i : range(Nc))
      for (int j : range(Nc))
       for (int k : range(Nc))
        for (int l : range(Nc)) {
 

         escape(&M4);
     escape(&P);
#ifdef RHS_ZERO
         // M4[{iw1, iw2, iw3}](i, j, k, l) = 2.0 ;
         P(i, j, k, l) = 2.0 ;
#else
         M4[{iw1, iw2, iw3}](i, j, k, l) = (M1[{iw2, iw1}](j, i) * M2[{iw4, iw3}](l, k) -  M1[{iw2, iw1}](l, i) * M2[{iw4, iw3}](j, k));
#endif
         clobber();
        }
    }
 }
}
BENCHMARK(explicit_loops1);

static void explicit_loops2(benchmark::State &state) {
 int Nc = M4.target_shape()[0];
 while (state.KeepRunning()) {
  for (auto [iw1, iw2, iw3] : M4.mesh()) { 
     
     for (int i : range(Nc))
      for (int j : range(Nc))
       for (int k : range(Nc))
        for (int l : range(Nc)) {
 
     	 auto iw4 = iw1;

         escape(&M4);
#ifdef RHS_ZERO
         M4[{iw1, iw2, iw3}](i, j, k, l) = 2.0 ;
#else
         M4[{iw1, iw2, iw3}](i, j, k, l) = (M1[{iw2, iw1}](j, i) * M2[{iw4, iw3}](l, k) -  M1[{iw2, iw1}](l, i) * M2[{iw4, iw3}](j, k));
#endif
         clobber();
        }
    }
 }
}
BENCHMARK(explicit_loops2);


static void explicit_loops(benchmark::State &state) {
 int Nc = M4.target_shape()[0];
 while (state.KeepRunning()) {
  for (auto const &iw1 : iw_mesh)
   for (auto const &iw2 : iw_mesh)
    for (auto const &iw3 : iw_mesh) {

     auto P = M4[{iw1, iw2, iw3}];
     auto iw4 = iw1;

     for (int i : range(Nc))
      for (int j : range(Nc))
       for (int k : range(Nc))
        for (int l : range(Nc)) {

         escape(&M4);
         escape(&P);
#ifdef RHS_ZERO
         P(i, j, k, l) = 2.0;
#else
         P(i, j, k, l) = (M1[{iw2, iw1}](j, i) * M2[{iw4, iw3}](l, k) -  M1[{iw2, iw1}](l, i) * M2[{iw4, iw3}](j, k));
#endif
         //P(i, j, k, l) = 2.0;
         clobber();
        }
    }
 }
}
BENCHMARK(explicit_loops);

/*

static void explicit_loops_data(benchmark::State &state) {
 auto &M4_data = M4.data();
 int Nc = M4.target_shape()[0];

 while (state.KeepRunning())
  for (int iw1 : range(0, 2 * n_iw, 1))
   for (int iw2 : range(0, 2 * n_iw, 1))
    for (int iw3 : range(0, 2 * n_iw, 1))
     for (int i : range(Nc))
      for (int j : range(Nc))
       for (int k : range(Nc))
        for (int l : range(Nc)) {
         escape(&M4_data);
         M4_data(iw1, iw2, iw3, i, j, k, l) = 0.0;
         clobber();
        }
}
BENCHMARK(explicit_loops_data);

static void explicit_loops_data_resorted(benchmark::State &state) {
 auto &M4_data = M4.data();
 int Nc = M4.target_shape()[0];

 while (state.KeepRunning()) {
  for (int i : range(Nc))
   for (int j : range(Nc))
    for (int k : range(Nc))
     for (int l : range(Nc))
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
*/


BENCHMARK_MAIN();
