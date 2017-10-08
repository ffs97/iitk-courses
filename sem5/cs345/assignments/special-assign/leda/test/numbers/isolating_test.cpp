#include <LEDA/system/basic.h>
#include <LEDA/numbers/rational.h>
#include <LEDA/numbers/real.h>
#include <LEDA/numbers/polynomial.h>
#include <LEDA/numbers/isolating.h>

#define LEDA_VECTOR growing_array

using namespace leda;
#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif

int main() {

    cout << "\nStart program isolating_test. \n";

   {
      cout << "polynomial of degree 2 ";
      rational a(3,1);
      rational b(7,1);
       LEDA_VECTOR<rational> P(3);
       P[0] = -b; 
       P[1] = rational(0,1); 
       P[2] = a;
       polynomial<rational> PP(P);

       isolating<rational> IS(PP,STURM);
       assert(IS.number_of_real_roots() == 2);
       isolating<rational> IU(PP,USPENSKY);

       cout << " ... done. \n";
  }

  {

     cout << "a not squarefree polynomial ";
     // (x-1)^2 (x^3+2x+1) 
     LEDA_VECTOR<real> PP(6); 
     PP[0] = real(1);
     PP[1] = real(0); 
     PP[2] = real(-3);
     PP[3] = real(3);
     PP[4] = real(-2);
     PP[5] = real(1);
     polynomial<real> P(PP);

     isolating<real> IS(P,STURM);
     isolating<real> IU(P,USPENSKY_WITH_GCD);

     assert(IS.number_of_real_roots() == 2 && IU.number_of_real_roots() == 2);
     assert(!IS.is_exact_root(1) && IU.is_exact_root(1));
     assert(IS.left_boundary(1) < IU.right_boundary(1) && IS.right_boundary(1) > 1);

     cout << " ... done. \n";
  }

  {

     cout << "another not squarefree polynomial ";
     // (x^2+1)^2 (x-3)(x^3-2x+1)^3
     // 3. Nst.: 1 4. Nst.: 3
     LEDA_VECTOR<real> PP(15); 
     PP[0] = real(-3);
     PP[1] = real(19); 
     PP[2] = real(-48);
     PP[3] = real(65);
     PP[4] = real(-56);
     PP[5] = real(25);
     PP[6] = real(23);
     PP[7] = real(-48);
     PP[8] = real(31);
     PP[9] = real(-9);
     PP[10] = real(-8);
     PP[11] = real(15);
     PP[12] = real(-4);
     PP[13] = real(-3);
     PP[14] = real(1);

     polynomial<real> P(PP);

     isolating<real> IS = isolating<real>(P,STURM);
     // Sturm takes relatively long.
     //isolating IU = isolating(P,USPENSKY_WITH_GCD);
     // Uspensky takes toooo long.

     assert(IS.number_of_real_roots() == 4 );
     assert(IS.left_boundary(2) <= 1 );
     assert(IS.right_boundary(3) >= 3);

     cout << " ... done. \n";
  }

  {
  cout << "Wilkinson polynomial: "; 
  // takes very long for the Sturm version and for the Uspensky with gcd version
  // \prod_{i=1}^n (X-i)

      int n=20;
      real one(1);
      LEDA_VECTOR<real> PP(2);
      PP[0] = -one;
      PP[1] = one;
      polynomial<real> P(PP);
      for(int i=2; i<=n; i++) {
        PP[0] = real(-i);
        P = P*polynomial<real>(PP);
      }
      isolating<real> I(P,8,USPENSKY); // we begin to count at 0

      assert(I.number_of_real_roots() == 9);
      assert(I.is_exact_root(8));
      assert(I.left_boundary(8)==9);
     cout << " ... done. \n";

  } 
  
  cout << "...all tests successfully completed!\n";
  cout << "Time: " << used_time() << endl;
  return 0;
}


