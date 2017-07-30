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
#include <triqs/arrays.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"
#include <triqs/arrays/blas_lapack/gemm.hpp>
#include <triqs/utility/variant.hpp>
#include <alloca.h>

using namespace triqs::arrays;
using namespace triqs::clef;
using namespace triqs;

struct mat_1x1 {
 double data[1];
 double & operator()(int i, int j) { return data[0];}
 double operator()(int i, int j) const { return data[0];}
};

struct mat_2x2 {
 double data[4];
 void * data2;
 triqs::arrays::indexmaps::cuboid::map<2,void> I;
 triqs::arrays::storages::shared_block<double,true> S;
 double & operator()(int i, int j) { return data[1*i + 2*j];}
 double operator()(int i, int j) const { return data[1*i + 2*j];}

 
 mat_2x2(): I{{mini_vector<size_t,2>{2,2}}}, S(I.domain().number_of_elements()) {
  //data2 = triqs::arrays::storages::arrays_allocator.allocate(32).ptr;
  //escape(data2);
 }
 ~mat_2x2() { 
 // triqs::arrays::storages::arrays_allocator.deallocate({data2, 32}); 
  }
  
};

static void create_s(benchmark::State& state) {
  mat_2x2 a;
  std::cout  << sizeof(a.S)<< std::endl;
  std::cout  << sizeof(a.I)<< std::endl;
 while (state.KeepRunning()) {
  escape(&a);
  //escape(&a.S);
  //escape(&a.I);
  //a(0,0) = 10;
  //a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_s);


static void create_d(benchmark::State& state) {
  matrix<double> a(4,4);
  std::cout  << sizeof(a)<< std::endl;
 while (state.KeepRunning()) {
  escape(&a);
  //a(0,0) = 10;
  //a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_d);


static void move_d(benchmark::State& state) {
 while (state.KeepRunning()) {
  matrix<double> a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  matrix<double> b(std::move(a));
  escape(&b);
  clobber();
 }
}
BENCHMARK(move_d);

BENCHMARK_MAIN();

