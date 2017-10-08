#include <LEDA/system/basic.h>
#include <LEDA/numbers/bigfloat.h>
#include <LEDA/numbers/real.h>
#include <LEDA/numbers/polynomial.h>
#include <LEDA/numbers/approximating.h>

#define LEDA_VECTOR growing_array

using namespace leda;
#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif

int main() {

  cout << "\nStart program approximating_test. \n";

  {
     cout << "polynomial of degree 3 ";
     LEDA_VECTOR<bigfloat> V(4);
     V[0] = bigfloat(-2);
     V[1] = bigfloat(0);
     V[2] = bigfloat(0);
     V[3] = bigfloat(1);
     polynomial<bigfloat> P(V);

     bigfloat l = 1;
     bigfloat r = 1.5;
     int n = 100;

     approximating A(P,l,r,n);
     bigfloat a = A.approximation(); // approximation

     real alpha = root(real(2),3); // exact root
     real app(a);
     assert(abs(alpha-app) <= ipow2(-n));
     cout << " ... done.\n";
  }

  {
  cout << "Wilkinson polynomial: "; 
  // \prod_{i=1}^n (X-i)

      int n=10;
      bigfloat one(1);
      LEDA_VECTOR<bigfloat> PP(2);
      PP[0] = -one;
      PP[1] = one;
      polynomial<bigfloat> P(PP);
      for(int i=2; i<=n; i++) {
        PP[0] = bigfloat(-i);
        P = P*polynomial<bigfloat>(PP);
      }

      bigfloat l= 0.9;
      bigfloat r = 1.1;
      int m = 150;


      approximating A(P,l,r,m);
      bigfloat::set_precision(150);
      bigfloat::set_rounding_mode(EXACT);
      bigfloat a = A.approximation(); // approximation

 
      assert(abs(a-1) <= ipow2(-m));
      cout << " ... done.\n";

  }


  cout << "...all tests successfully completed!\n";
  cout << "Time: " << used_time() << endl;
  return 0;
}


