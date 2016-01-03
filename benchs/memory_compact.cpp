#include <triqs/arrays.hpp>
#include <benchmark/benchmark.h>
#include "./common.hpp"

using namespace triqs::arrays;
using namespace triqs;

struct cache_t {
  double dtau_l = 0, dtau_r = 0; // difference in tau of this node and left and right sub-trees
  //int block_table[N];
  std::vector<int> block_table; // number of blocks limited to 2^15
  //std::vector<arrays::matrix<double>> matrices; // partial product of operator/time evolution matrices
  //std::vector<double> matrix_lnorms; // -ln(norm(matrix))
  //std::vector<bool> matrix_norm_valid; // is the norm of the matrix still valid?
  //cache_t(int n_blocks) : block_table(n_blocks) , matrix_lnorms(n_blocks), matrices(n_blocks), matrix_norm_valid(n_blocks) {}
  cache_t(int n_blocks) : block_table(n_blocks) {}//, matrix_lnorms(n_blocks), matrices(n_blocks), matrix_norm_valid(n_blocks) {}
 };

int main() { 

 std::vector<cache_t> Cv(10,{20});

 auto & C = Cv[0];
 
 void * p = &C;
 std::cout  << " & C" << p <<std::endl;
 std::cout  << " size " << sizeof(C) <<std::endl;
 std::cout  << " & C.dtau_l " << (long)&C.dtau_l - long(p) <<std::endl;
 std::cout  << " & C.dtau_r " << (long)&C.dtau_r - long(p) <<std::endl;
 std::cout  << " & C.block_table " << (long)&C.block_table - long(p) <<std::endl;
 std::cout  << " & C.block_table " << (long)&C.block_table[0] - long(p) <<std::endl;
 std::cout  << " & C.block_table " << (long)&C.block_table[9] - long(p) <<std::endl;
 //std::cout  << " & C.matrices " << (long)&C.matrices - long(p) <<std::endl;


}
