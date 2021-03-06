#pragma once
#include <triqs/arrays.hpp>

struct my_determinant {
  int n;
  std::vector<int> ipiv;

  my_determinant(int size) : n(size), ipiv(size) {}

  double det(triqs::arrays::matrix<double> *M, int dim) {
    int info=0;
    triqs::arrays::lapack::f77::getrf(dim, dim, (*M).data_start(), n, &ipiv[0], info);
    if (info < 0) std::cout << "Problem" << std::endl; // replace by triqs error management

    double _det = 1;
    for (int i = 0; i < dim; i++) _det *= (*M)(i, i);

    bool flip = false; // compute the sign of the permutation
    for (int i = 0; i < dim; i++) {
      if (ipiv[i] != i + 1) flip = !(flip);
    }

    _det = (flip ? -_det : _det);

    return _det;
  }
};

