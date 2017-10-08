/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  residual_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




#include <LEDA/numbers/residual.h>
#include <LEDA/core/random.h>


#ifndef NO_FPU
#include <LEDA/numbers/fpu.h>
#endif

#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::flush;
using std::endl;
#endif


void check_equal(const residual& r, const integer& i)
{
  assert(r==i);
  assert(i==r.to_integer());
  assert(r.lagrange_sign()==sign(i));
  assert(r.garner_sign()==sign(i));
}

void manipulate(const residual& y, const residual& x)
{
  residual z=y;
  z++; assert(z==y+1);
  assert(z>y); assert(z>=y);
  z--; assert(z==y); assert(z>=y);
  --z; assert(z==y-1); assert(z<y);
  ++z; assert(z==y); assert(z<=y);
  z.absolute(z); z*=y.sign(); assert(z==y);
  z.negate(-z); assert(z==y);
  z/=x; z*=x; assert(z==y);
  residual v; v.inverse(x); 
  z*=v; z/=v; assert(z==y);
  z=z/x; z=z*x; assert(z==y);
  z.div(z,x); z.mul(z,x); assert(z==y);
  z+=x; z-=x; assert(z==y);
  z=z+x; z=z-x; assert(z==y);
  z.add(z,x); z.sub(z,x); assert(z==y);
  z+=0; z-=0; assert(z==y);
  z+=1234567.0; z-=1234567.0; assert(z==y);
  z+=residual(3); z-=residual(3.0); assert(z==y);
  z*=0; assert(z==0); assert(z.is_zero());
  z=x>>32;
  residual w=z;
  z<<=32; 
  z>>=32; assert(z==w);
  z=z<<32;
  z=z>>32; assert(z==w);
  const residual_sequence& p=residual::get_primetable();
  z*=p[0]; z/=p[0]; assert(z==w);
}


int main() {

#ifndef NO_FPU
  leda_fpu::set_defaults();
  // leda_fpu::round_up(); 
  // the FPU rounding mode shall not matter
#endif

  int b=32;
  int dim=200;

  cout << "\nStart program residual_test" << flush;

  residual::set_maximal_bit_length(1000000);
  residual::set_maximal_bit_length(b,residual::do_overflow_check);
  
  integer* AI = new integer[dim];
  residual* A = new residual[dim];
  int i;
 
  residual::set_maximal_bit_length(b,residual::do_overflow_check);
  for (i=0;i<dim;i++) {
    AI[i]=integer::random(b);
    A[i]=AI[i];  
    check_equal(A[i],AI[i]);
  }

  integer xi = integer::random(b);
  residual x=xi;
  check_equal(x,xi);

  residual y;
  integer yi;
  for (i=0;i<dim;i++) {
    if (i%8==0)
      cout << "." << flush;
    if (i%2) {
      residual::set_maximal_bit_length
       (b*(i+1),residual::no_overflow_check);
       y=y*x+A[i];
       yi=yi*xi+AI[i];
    }
    else {
      residual::set_maximal_bit_length
       (b*(i+1),residual::no_overflow_check);
      y=y*x-A[i]; 
      yi=yi*xi-AI[i];
    }
    check_equal(y,yi);
    manipulate(y,x);
  }

  cout << "\n... all tests successfully completed!\n";
  return 0;
}

