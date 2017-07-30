#include <triqs/arrays.hpp>
#include <triqs/arrays/linalg/det_and_inverse.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"
using namespace triqs::arrays;
using namespace triqs;

double super_fonction(matrix<double> const &a, matrix<double> const &b) { 
  double r = 0;
  auto d = first_dim(a); 
  escape(&d);
  for (int i = 0; i < d; ++i) r += a(i, i) + b(i, i);
  escape(&r);
  return r;
}


