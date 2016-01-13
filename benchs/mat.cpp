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
 double & operator()(int i, int j) { return data[1*i + 2*j];}
 double operator()(int i, int j) const { return data[1*i + 2*j];}
};

struct mat_3x3 {
 double data[9];
 double & operator()(int i, int j) { return data[1*i + 3*j];}
 double operator()(int i, int j) const { return data[1*i + 3*j];}
};


struct mat_stack { 
 const int N;
 double *data = (double*)alloca(sizeof(*data) * N * N); // allocate on STACK
 mat_stack(int i) : N(i) {}
 double & operator()(int i, int j) { return data[1*i + N*j];}
 double operator()(int i, int j) const { return data[1*i + N*j];}
};


mat_1x1 mult(mat_1x1 a, mat_1x1 b) { 
  mat_1x1 r;
  r(0,0) = a(0,0) * b(0,0);
  return r;
}

mat_2x2 mult(mat_2x2 a, mat_2x2 b) { 
  mat_2x2 r;
  r(0,0) = a(0,0) * b(0,0) + a(0,1) * b(1,0);
  r(1,0) = a(1,0) * b(0,0) + a(1,1) * b(1,0);
  r(0,1) = a(0,0) * b(0,1) + a(0,1) * b(1,1);
  r(1,1) = a(1,0) * b(0,1) + a(1,1) * b(1,1);
  return r;
}

mat_3x3 mult(mat_3x3 a, mat_3x3 b) { 
  mat_3x3 r;
  r(0,0) = a(0,0) * b(0,0) + a(0,1) * b(1,0) + a(0,2) * b(2,0);
  r(1,0) = a(1,0) * b(0,0) + a(1,1) * b(1,0) + a(1,2) * b(2,0);
  r(2,0) = a(2,0) * b(0,0) + a(2,1) * b(1,0) + a(2,2) * b(2,0);
 
  r(0,1) = a(0,0) * b(0,1) + a(0,1) * b(1,1) + a(0,2) * b(2,1);
  r(1,1) = a(1,0) * b(0,1) + a(1,1) * b(1,1) + a(1,2) * b(2,1);
  r(2,1) = a(2,0) * b(0,1) + a(2,1) * b(1,1) + a(2,2) * b(2,1);
  
  r(0,2) = a(0,0) * b(0,2) + a(0,1) * b(1,2) + a(0,2) * b(2,2);
  r(1,2) = a(1,0) * b(0,2) + a(1,1) * b(1,2) + a(1,2) * b(2,2);
  r(2,2) = a(2,0) * b(0,2) + a(2,1) * b(1,2) + a(2,2) * b(2,2);
  
  return r;
}

mat_stack mult(mat_stack const &a, mat_stack const &b) { 
  mat_stack r(a.N);
  r(0,0) = a(0,0) * b(0,0) + a(0,1) * b(1,0) + a(0,2) * b(2,0);
  r(1,0) = a(1,0) * b(0,0) + a(1,1) * b(1,0) + a(1,2) * b(2,0);
  r(2,0) = a(2,0) * b(0,0) + a(2,1) * b(1,0) + a(2,2) * b(2,0);
 
  r(0,1) = a(0,0) * b(0,1) + a(0,1) * b(1,1) + a(0,2) * b(2,1);
  r(1,1) = a(1,0) * b(0,1) + a(1,1) * b(1,1) + a(1,2) * b(2,1);
  r(2,1) = a(2,0) * b(0,1) + a(2,1) * b(1,1) + a(2,2) * b(2,1);
  
  r(0,2) = a(0,0) * b(0,2) + a(0,1) * b(1,2) + a(0,2) * b(2,2);
  r(1,2) = a(1,0) * b(0,2) + a(1,1) * b(1,2) + a(1,2) * b(2,2);
  r(2,2) = a(2,0) * b(0,2) + a(2,1) * b(1,2) + a(2,2) * b(2,2);
  
  return r;
}

static void create_variant(benchmark::State& state) {
 while (state.KeepRunning()) {
  triqs::utility::variant<mat_2x2,mat_3x3> a;
  escape(&a);
  //a(0,0) = 10;
  //a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_variant);


static void create_s(benchmark::State& state) {
 while (state.KeepRunning()) {
  mat_2x2 a;
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_s);


static void create_d(benchmark::State& state) {
 while (state.KeepRunning()) {
  matrix<double> a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
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


struct matrix2 {
 int n1 = 0, n2 = 0;
 double* data = nullptr;
 matrix2() = default;
 matrix2(int n1, int n2) : n1(n1), n2(n2){ data = new double[n1 * n2]; }
 matrix2(matrix2 const&) = delete;
 matrix2(matrix2&& m) : n1(m.n1), n2(m.n2) {
  data = m.data;
  m.data = nullptr;
 }
 ~matrix2() {
  if (data) delete data;
 }
 double& operator()(int i, int j) { return data[1 * i + n1 * j]; }
 double operator()(int i, int j) const { return data[1 * i + n1 * j]; }
};

struct matrix2b {
 int n1 = 0, n2 = 0;
 triqs::arrays::storages::shared_block<double, true> data;
 matrix2b() = default;
 matrix2b(int n1, int n2) : n1(n1), n2(n2), data(n1*n2) {} 
 matrix2b(matrix2b const&) = delete;
 matrix2b(matrix2b&& m) = default; // : n1(m.n1), n2(m.n2), data(std::move(m.data)) {}
 double& operator()(int i, int j) { return data[1 * i + n1 * j]; }
 double operator()(int i, int j) const { return data[1 * i + n1 * j]; }
};

struct matrix2c {
 int n1 = 0, n2 = 0;
 triqs::arrays::storages::mem_block<double> data;
 matrix2c() = default;
 matrix2c(int n1, int n2) : n1(n1), n2(n2), data(n1*n2) {}
 matrix2c(matrix2c const&) = delete;
 matrix2c(matrix2c&& m)  = default;
 double& operator()(int i, int j) { return data.p[1 * i + n1 * j]; }
 double operator()(int i, int j) const { return data.p[1 * i + n1 * j]; }
};


static void create_d2(benchmark::State& state) {
 while (state.KeepRunning()) {
 matrix2 a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_d2);


static void move_d2(benchmark::State& state) {
 while (state.KeepRunning()) {
  matrix2 a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  matrix2 b(std::move(a));
  escape(&b);
  clobber();
 }
}
BENCHMARK(move_d2);


static void create_d2b(benchmark::State& state) {
 while (state.KeepRunning()) {
  matrix2b a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_d2b);


static void move_d2b(benchmark::State& state) {
 while (state.KeepRunning()) {
  matrix2b a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  matrix2b b(std::move(a));
  escape(&b);
  clobber();
 }
}
BENCHMARK(move_d2b);

static void create_d2c(benchmark::State& state) {
 while (state.KeepRunning()) {
  matrix2c a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_d2c);


static void move_d2c(benchmark::State& state) {
 while (state.KeepRunning()) {
  matrix2c a(2,2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  matrix2c b(std::move(a));
  escape(&b);
  clobber();
 }
}
BENCHMARK(move_d2c);



static void create_mat_stack(benchmark::State& state) {
 while (state.KeepRunning()) {
  mat_stack a(2);
  escape(&a);
  a(0,0) = 10;
  a(1,1) = 20;
  clobber();
 }
}
BENCHMARK(create_mat_stack);

static void mul_s1(benchmark::State& state) {
 while (state.KeepRunning()) {
  mat_1x1 a,b;
   escape(&a);
  escape(&b);
  auto c= mult(a,b);
  escape(&c);
  clobber();
 }
}
BENCHMARK(mul_s1);

static void mul_d1(benchmark::State& state) {
 matrix<double> a(1,1), b(1,1), c(1,1);
 while (state.KeepRunning()) {
  escape(&a);
  escape(&b);
  //auto d= a*b;
  escape(&c);
  blas::gemm(1.0, a, b,1,c);
  escape(&c);
  //escape(&d);
  clobber();
 }
}
BENCHMARK(mul_d1);



static void mul_s2(benchmark::State& state) {
 while (state.KeepRunning()) {
  mat_2x2 a,b;
   escape(&a);
  escape(&b);
  auto c= mult(a,b);
  escape(&c);
  clobber();
 }
}
BENCHMARK(mul_s2);

static void mul_d2(benchmark::State& state) {
 matrix<double> a(2,2), b(2,2), c(2,2);
 while (state.KeepRunning()) {
  escape(&a);
  escape(&b);
  //auto d= a*b;
  escape(&c);

  /*c(0,0) = a(0,0) * b(0,0) + a(0,1) * b(1,0);
  c(1,0) = a(1,0) * b(0,0) + a(1,1) * b(1,0);
  c(0,1) = a(0,0) * b(0,1) + a(0,1) * b(1,1);
  c(1,1) = a(1,0) * b(0,1) + a(1,1) * b(1,1);
 */

  blas::gemm(1.0, a, b,1,c);
  escape(&c);
  //escape(&d);
  clobber();
 }
}
BENCHMARK(mul_d2);



static void mul_s3(benchmark::State& state) {
 while (state.KeepRunning()) {
  mat_3x3 a,b,c;
  escape(&a);
  escape(&b);
  c= mult(a,b);
  escape(&c);
  clobber();
 }
}
BENCHMARK(mul_s3);

static void mul_d3(benchmark::State& state) {
 matrix<double> a(3,3), b(3,3), c(3,3);
 while (state.KeepRunning()) {
  escape(&a);
  escape(&b);
  //auto d= a*b;
  escape(&c);
  blas::gemm(1.0, a, b,1,c);
  escape(&c);
  //escape(&d);
  clobber();
 }
}
BENCHMARK(mul_d3);



BENCHMARK_MAIN();

