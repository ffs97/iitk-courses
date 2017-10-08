/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/numbers/fpu.h>
#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


bool glob_check=0;

void divbyzero_checker(fcr_type exception) {
  glob_check = 1;
}

int main()
{

  cout << "\nStart program fpu_test.\n";

  leda_fpu::set_defaults();

  leda_fpu::diagnosis();
  
  leda_fpu::set_defaults();

  leda_fpu::set_exception_policy(ieee_all_exceptions,leda_fpu::silence);
  leda_fpu::set_exception_policy(ieee_underflow,leda_fpu::retrospective);

  leda_fpu::raise_inexact_exception();
  leda_fpu::raise_invalid_exception();
  assert (leda_fpu::exceptions_raised(ieee_invalid));
  leda_fpu::set_exception_policy(ieee_invalid,leda_fpu::ignore);
  assert (!leda_fpu::exceptions_raised(ieee_invalid));

  leda_fpu::raise_underflow_exception();
  assert (leda_fpu::exceptions_raised(ieee_underflow));
  leda_fpu::raise_overflow_exception();
  assert (leda_fpu::exceptions_raised(ieee_overflow));
  assert (leda_fpu::exceptions_raised(ieee_underflow));
  leda_fpu::unraise_exceptions(ieee_overflow);
  assert (leda_fpu::exceptions_raised(ieee_underflow));
  assert (!leda_fpu::exceptions_raised(ieee_overflow));

 
  if (leda_fpu::traps_recoverable(ieee_divbyzero)) {
    leda_fpu::set_immediate_handler(divbyzero_checker);
    leda_fpu::set_exception_policy(ieee_divbyzero,leda_fpu::immediate); 
    leda_fpu::raise_divbyzero_exception();
    assert (!leda_fpu::exceptions_raised(ieee_divbyzero));
#if !defined(i386)
    assert(glob_check==1);
#endif
  }
  else {
    leda_fpu::raise_divbyzero_exception();
    assert (leda_fpu::exceptions_raised(ieee_divbyzero));
  }

  assert (leda_fpu::exceptions_raised(ieee_underflow));
  assert (leda_fpu::exceptions_raised(ieee_inexact));
  assert (!leda_fpu::exceptions_raised(ieee_invalid));
  assert (!leda_fpu::exceptions_raised(ieee_overflow));

  cout << "... all tests successfully completed!\n";
 
  return 0;
}






