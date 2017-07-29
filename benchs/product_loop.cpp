
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

#define FORCEINLINE __inline__ __attribute__((always_inline))

FORCEINLINE int f1(std::vector<long> const &v) { 
 int r = 0;
 for (auto[x, y] : prod(v, v)) { r = r + y + x; }
 return r;
}


FORCEINLINE int f2(std::vector<long> const &v) { 
  int r = 0;
  for (auto &x : v)
   for (auto &y : v) {
    r = r + y * x;
   }
 return r;
}

FORCEINLINE int f3(std::vector<long> const &v) { 
 int r = 0;
 auto b1 = v.begin();
 auto b2 = v.begin();
 auto e1 = v.end();
 auto e2 = v.end();
 auto i1 = b1, i2 = b2;

 while (i2 != e2) {
  r = r + *i2 + *i1;
  ++i1;
  if (i1 == e1) {
   i1 = b1;
   ++i2;
  }
 }
 return r;
}


static void b_f1(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 while (state.KeepRunning()) {
  double r = 0;
  escape(&(r));
  r = f1(v);
  escape(&(r));
  clobber();
 }
}
BENCHMARK(b_f1);



static void b_f2(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 double r = 0;
 while (state.KeepRunning()) {
  escape(&r);
  r += f2(v);
  escape(&r);
  clobber();
 }
}
BENCHMARK(b_f2);


static void b_f3(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 while (state.KeepRunning()) {
  double r = 0;
  escape(&(r));
  r = f3(v);
  clobber();
 }
}
BENCHMARK(b_f3);



static void product_loops(benchmark::State &state) {
 const int N = state.range_x();
 std::vector<long> v(N, 2);
 while (state.KeepRunning()) {
  double r = 0;
  escape(&(r));
  for (auto[x, y] : prod(v, v)) {
   // escape(&x);
   // escape(&y);
   escape(&r);
   r = r + x + y;
   escape(&r);
   clobber();
  }
 }
}
BENCHMARK(product_loops)->Arg(1000);


static void explicit_loops2(benchmark::State &state) {
 const int N = state.range_x();
 std::vector<long> v(N, 2);
 escape(&v);
 while (state.KeepRunning()) {

  int r = 0;
  auto b1 = v.begin();
  auto b2 = v.begin();
  auto e1 = v.end();
  auto e2 = v.end();
  auto i1 = b1, i2 = b2;

  while (i2 != e2) {
   escape(&(r));
   r = r + *i2 + *i1;
   escape(&(r));
   ++i1;
   // escape(&(*i1));
   if (i1 == e1) {
    i1 = b1;
    ++i2;
   }
   // escape(&(*i2));
   clobber();
  }
 }
}
BENCHMARK(explicit_loops2)->Arg(1000);


static void explicit_loops(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 escape(&v);
 while (state.KeepRunning()) {
  int r = 0;
  for (auto &x : v)
   for (auto &y : v) {
    // escape(&v);
    // escape(&x);
    // escape(&y);
    escape(&(r));
    r = r + x + y;
    escape(&(r));
    clobber();
   }
 }
}
BENCHMARK(explicit_loops);

static void explicit_loopsf1(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 escape(&v);
 while (state.KeepRunning()) {
  int r = 0;
  for (int i = 0; i < 1000; ++i)
   for (int j = 0; j < 1000; ++j) {
    escape(&v[i]);
    escape(&v[j]);
    escape(&(r));
    r = r + v[i] + v[j];
    clobber();
   }
 }
}
BENCHMARK(explicit_loopsf1);



static void basic_loop1(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 escape(&v);
 while (state.KeepRunning()) {
  int r = 0;

  for (int i = 0; i < 1000; ++i) {
   escape(&v[i]);
   r = r + v[i] + v[i];
  }
  clobber();
 }
}
BENCHMARK(basic_loop1);


static void basic_loop2(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 escape(&v);
 while (state.KeepRunning()) {
  int r = 0;

  int i = 0;
  while (i < 1000) {
   escape(&v[i]);
   r = r + v[i] + v[i];
   ++i;
  }
  clobber();
 }
}
BENCHMARK(basic_loop2);



static void explicit_loops4(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 escape(&v);
 while (state.KeepRunning()) {

  int r = 0;
  int N = v.size();
  int i2 = 0;
  while (i2 != N) {

   int i1 = 0;
   while (i1 != N) {

    escape(&(i1));
    escape(&(i2));
    escape(&(v[i1]));
    escape(&(v[i2]));
    //   r = r + i1 + i2 ; //v[i1] + v[i2];
    r = r + v[i2] + v[i1];
    escape(&(r));
    ++i1;
   }
   ++i2;
  }
  clobber();
 }
}
BENCHMARK(explicit_loops4);


static void explicit_loops3(benchmark::State &state) {
 std::vector<long> v(1000, 2);
 escape(&v);
 while (state.KeepRunning()) {

  int r = 0;
  int N = v.size();
  int i2 = 0;
  int i1 = 0;

  while (i2 != N) {
   escape(&(i1));
   escape(&(i2));
   escape(&(v[i1]));
   escape(&(v[i2]));
   // r = r + i1 + i2 ; //v[i1] + v[i2];
   r = r + v[i1] + v[i2];
   escape(&(r));
   ++i1;
   if (i1 == N) {
    i1 = 0;
    ++i2;
   }
   clobber();
  }
 }
}
BENCHMARK(explicit_loops3);


BENCHMARK_MAIN();
