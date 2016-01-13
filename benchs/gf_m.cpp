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
using namespace triqs::arrays;
using namespace triqs::gfs;
using namespace triqs;

const int N = 1000;

using M4t = gf<cartesian_product<imtime, imtime, imtime>, tensor_valued<4>>;
using M2t = gf<cartesian_product<imtime, imtime>, tensor_valued<2>>;


static void brack1(benchmark::State& state) {
 double beta = 10;
 auto G = gf<imfreq>{{beta, Fermion, N}, {2, 2}};
 G() = 0;
 const int N_tau = 60, Nbl1 = 1, Nbl2 = 1;
 auto chi = M4t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl1, Nbl2, Nbl2}};
 auto chi2 = M2t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl2}};

 while (state.KeepRunning()) {

  for (int i = 0; i < get_target_shape(chi)[0]; i++)
   for (int j = 0; j < get_target_shape(chi)[1]; j++)
    for (int k = 0; k < get_target_shape(chi)[2]; k++)
     for (int l = 0; l < get_target_shape(chi)[3]; l++)
      for (int t1 = 0; t1 < N_tau; t1++)
       for (int t2 = 0; t2 < N_tau; t2++)
        for (int t3 = 0; t3 < N_tau; t3++) {
         escape(&chi);
         chi[{t1, t2, t3}](i, j, k, l) = 9;
         clobber();
        }
 }
}
BENCHMARK(brack1);


static void brack2(benchmark::State& state) {
 double beta = 10;
 auto G = gf<imfreq>{{beta, Fermion, N}, {2, 2}};
 G() = 0;
 const int N_tau = 60, Nbl1 = 1, Nbl2 = 1;
 auto chi = M4t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl1, Nbl2, Nbl2}};
 auto chi2 = M2t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl2}};

 while (state.KeepRunning()) {

  for (int i = 0; i < get_target_shape(chi)[0]; i++)
   for (int j = 0; j < get_target_shape(chi)[1]; j++)
    for (int k = 0; k < get_target_shape(chi)[2]; k++)
     for (int l = 0; l < get_target_shape(chi)[3]; l++)
      for (int t1 = 0; t1 < N_tau; t1++)
       for (int t2 = 0; t2 < N_tau; t2++)
        for (int t3 = 0; t3 < N_tau; t3++) {
         escape(&chi);
         chi.data()(t1, t2, t3, i, j, k, l) = 9;
         clobber();
        }
 }
}
BENCHMARK(brack2);



static void with_brack(benchmark::State& state) {
 double beta = 10;
 auto G = gf<imfreq>{{beta, Fermion, N}, {2, 2}};
 G() = 0;
 const int N_tau = 60, Nbl1 = 1, Nbl2 = 1;
 auto chi = M4t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl1, Nbl2, Nbl2}};
 auto chi2 = M2t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl2}};

 while (state.KeepRunning()) {

  for (int i = 0; i < get_target_shape(chi)[0]; i++)
   for (int j = 0; j < get_target_shape(chi)[1]; j++)
    for (int k = 0; k < get_target_shape(chi)[2]; k++)
     for (int l = 0; l < get_target_shape(chi)[3]; l++)
      for (int t1 = 0; t1 < N_tau; t1++)
       for (int t2 = 0; t2 < N_tau; t2++)
        for (int t3 = 0; t3 < N_tau; t3++) {
         escape(&chi);
         escape(&chi2);
         chi[{t1, t2, t3}](i, j, k, l) += chi2[{t1, t2}](i, j) * chi2[{t3, 0}](k, l) - chi2[{t1, 0}](i, l) * chi2[{t3, t2}](k, j);
         clobber();
        }
 }
}
BENCHMARK(with_brack);


static void with_data(benchmark::State& state) {
 double beta = 10;
 auto G = gf<imfreq>{{beta, Fermion, N}, {2, 2}};
 G() = 0;
 const int N_tau = 60, Nbl1 = 1, Nbl2 = 1;
 auto chi = M4t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl1, Nbl2, Nbl2}};
 auto chi2 = M2t{{{beta, Fermion, N_tau}, {beta, Fermion, N_tau}}, {Nbl1, Nbl2}};

 while (state.KeepRunning()) {

  for (int i = 0; i < get_target_shape(chi)[0]; i++)
   for (int j = 0; j < get_target_shape(chi)[1]; j++)
    for (int k = 0; k < get_target_shape(chi)[2]; k++)
     for (int l = 0; l < get_target_shape(chi)[3]; l++)
      for (int t1 = 0; t1 < N_tau; t1++)
       for (int t2 = 0; t2 < N_tau; t2++)
        for (int t3 = 0; t3 < N_tau; t3++) {
         escape(&chi);
         escape(&chi2);
         chi.data()(t1, t2, t3, i, j, k, l) +=
             chi2.data()(t1, t2, i, j) * chi2.data()(t3, 0, k, l) - chi2.data()(t1, 0, i, l) * chi2.data()(t3, t2, k, j);
         clobber();
        }
 }
}
BENCHMARK(with_data);



BENCHMARK_MAIN();

