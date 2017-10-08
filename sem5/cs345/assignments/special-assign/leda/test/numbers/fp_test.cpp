/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fp_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/numbers/fp.h>
#include <assert.h>

#ifndef NO_FPU
#include <LEDA/numbers/fpu.h>
#endif


using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif

bool test_power()
{
  bool res = true;
  for(int i = -52; i<63; i++)
  {
    if( ldexp(1.0,i) != fp::power_two(i))
    {
      cout << "fp::power_two("<<i<<")="<<fp::power_two(i)<<endl;
      cout << "ldexp="<< ldexp(1.0,i)<<endl;
      res = false;
    }
  }
  return res;
}

#ifndef NO_FPU
bool test_fpu_rounding()
{
	bool ok = true;

	double d = fp::power_two(52);
	double eps = fp::power_two(-52);
	double r, x;
	// checking to nearest
	leda_fpu::round_nearest();
	r = d + eps;
	x = r-d;
	if( x != 0.0 ) ok = false;

	leda_fpu::round_up();
	r = d + eps;
	x = r-d;
	if( x != 1.0) ok = false;

	leda_fpu::round_down();
	r = d -eps;
	x = r-d;
	if( x != -0.5) ok = false;

	leda_fpu::round_nearest();
	r = d - eps;
	x = r-d;
	if( x != 0.0 ) ok = false;

	return ok;
}
#endif


int main() {

  cout << "\nStart program fp_test.\n";
  assert( fp::selftest_okay() );
  assert( test_power() );
  assert( test_fpu_rounding() );
  cout << "... all tests successfully completed!\n";
  return 0;

}


