#include <triqs/arrays.hpp>
#include <triqs/arrays/linalg/det_and_inverse.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"
using namespace triqs::arrays;
using namespace triqs;

 template <typename M> auto trace2(M const& m) {
  auto r = typename M::value_type{};
  auto d = first_dimv2(m);
  escape(&d);
  //auto d = first_dimv2(m);
  for (int i = 0; i < d; ++i) r += m(i, i);
  escape(&r);
  return r;
 }
double super_fonction(matrix<double> const &a, matrix<double> const &b) { return trace2(a + b); }


