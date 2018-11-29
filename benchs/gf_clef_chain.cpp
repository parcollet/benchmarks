#include <benchmark/benchmark.h>
#include <cmath>
#include <limits>
#include <triqs/arrays.hpp>

#include <triqs/clef.hpp>
#include <triqs/gfs.hpp>
#include <triqs/utility/timer.hpp>

using namespace triqs;
using namespace triqs::gfs;

// Index placeholders
clef::placeholder<0> i;
clef::placeholder<1> j;
clef::placeholder<2> k;
clef::placeholder<3> l;

// Frequency placeholders
clef::placeholder<4> w;
clef::placeholder<5> n1;
clef::placeholder<6> n2;
clef::placeholder<7> n3;

using g2_iw_t = gf<cartesian_product<imfreq, imfreq, imfreq>, tensor_valued<4>>;

int n = 20, m = 20;
int Ns = 2;
double beta = 1.0;


static void pure_clef(benchmark::State& state) {
 gf_mesh<imfreq> mesh_b{beta, Boson, n};
 gf_mesh<imfreq> mesh_f{beta, Fermion, m};
 auto G2_iw_ph = g2_iw_t{{mesh_b, mesh_f, mesh_f}, {Ns, Ns, Ns, Ns}};
 while (state.KeepRunning()) { //

  benchmark::DoNotOptimize(G2_iw_ph);
  G2_iw_ph(w, n1, n2)(i, j, k, l) << (i + j + k + l / 2.0);
  benchmark::ClobberMemory();
 }
}
BENCHMARK(pure_clef);


static void loop_mesh(benchmark::State& state) {
 gf_mesh<imfreq> mesh_b{beta, Boson, n};
 gf_mesh<imfreq> mesh_f{beta, Fermion, m};
 auto G2_iw_ph = g2_iw_t{{mesh_b, mesh_f, mesh_f}, {Ns, Ns, Ns, Ns}};
 while (state.KeepRunning()) { //
  benchmark::DoNotOptimize(G2_iw_ph);
  for (auto const& w : mesh_b)
   for (auto const& n1 : mesh_f)
    for (auto const& n2 : mesh_f) { G2_iw_ph[w, n1, n2](i, j, k, l) << (i + j + k + l / 2.0); }
  benchmark::ClobberMemory();
 }
}
BENCHMARK(loop_mesh);


static void loop_mesh2(benchmark::State& state) {
 gf_mesh<imfreq> mesh_b{beta, Boson, n};
 gf_mesh<imfreq> mesh_f{beta, Fermion, m};
 auto G2_iw_ph = g2_iw_t{{mesh_b, mesh_f, mesh_f}, {Ns, Ns, Ns, Ns}};
 while (state.KeepRunning()) { //
  benchmark::DoNotOptimize(G2_iw_ph);
  for (auto const& W : G2_iw_ph.mesh())
     { G2_iw_ph[W](i, j, k, l) << (i + j + k + l / 2.0); }
  benchmark::ClobberMemory();
 }
}
BENCHMARK(loop_mesh2);


static void loop_mesh_idx(benchmark::State& state) {
 gf_mesh<imfreq> mesh_b{beta, Boson, n};
 gf_mesh<imfreq> mesh_f{beta, Fermion, m};
 auto G2_iw_ph = g2_iw_t{{mesh_b, mesh_f, mesh_f}, {Ns, Ns, Ns, Ns}};
 while (state.KeepRunning()) { //

  int size_0 = G2_iw_ph.target_shape()[0];
  int size_1 = G2_iw_ph.target_shape()[1];
  int size_2 = G2_iw_ph.target_shape()[2];
  int size_3 = G2_iw_ph.target_shape()[3];
  benchmark::DoNotOptimize(G2_iw_ph);

  for (auto const& w : mesh_b)
   for (auto const& n1 : mesh_f)
    for (auto const& n2 : mesh_f)
     for (int i : range(size_0))
      for (int j : range(size_1))
       for (int k : range(size_2))
        for (int l : range(size_3)) { G2_iw_ph[w, n1, n2](i, j, k, l) = (i + j + k + l / 2.0); }
  benchmark::ClobberMemory();
 }
}
BENCHMARK(loop_mesh_idx);


BENCHMARK_MAIN();
