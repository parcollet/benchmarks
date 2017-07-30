#include <triqs/utility/scope_guard.hpp>
#include "./common.hpp"

int f(int i) { return i+1;}

int main() { 

int a = 5;
  escape(&a);
  {
   SCOPE_EXIT { a = f(3); };
 //  a = 3;
  }
  clobber();


}

