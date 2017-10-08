/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bigfloat_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/





#include <LEDA/core/list.h>
#include <LEDA/numbers/bigfloat.h>
#include <LEDA/numbers/fp.h>

#if defined(__APPLE_CC__)
#define NO_FPU
#endif

#ifndef NO_FPU
#include <LEDA/numbers/fpu.h>
#endif

#include <LEDA/numbers/digit.h>

#include <math.h>
#include <assert.h>
#include <string.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::flush;
using std::endl;
#endif


static void check_special_basic() {

  assert(ispZero(bigfloat::pZero));
  assert(isnZero(bigfloat::nZero));
  assert(isZero(bigfloat::pZero));
  assert(isZero(bigfloat::nZero));

  assert(ispInf(bigfloat::pInf));
  assert(isnInf(bigfloat::nInf));
  assert(isInf(bigfloat::pInf));
  assert(isInf(bigfloat::nInf));

  assert(isNaN(bigfloat::NaN));

}

static void check_special_arithmetic(const bigfloat& x, const bigfloat& y) {

  double dx = to_double(x);
  double dy = to_double(y);

  // Note: Some platforms give NaN == NaN for doubles 
  // This is not IEEE-conform, but the fact remains

  assert(   fp::binary_equal(to_double(x+y),dx+dy) 
         || (isNaN(x+y) && fp::is_nan(dx+dy))
        );
  assert(   fp::binary_equal(to_double(x-y),dx-dy) 
         || (isNaN(x-y) && fp::is_nan(dx-dy))
        );
  assert(   fp::binary_equal(to_double(x*y),dx*dy) 
         || (isNaN(x*y) && fp::is_nan(dx*dy))
        );
  assert(   fp::binary_equal(to_double(x/y),dx/dy) 
         || (isNaN(x/y) && fp::is_nan(dx/dy))
        );
  assert(   fp::binary_equal(to_double(sqr(x)),dx*dx)
         || (isNaN(sqr(x)) && fp::is_nan(dx*dx))
        );

#if !defined(__BORLANDC__)
  assert(   fp::binary_equal(to_double(sqrt(x)),sqrt(dx))
         || (isNaN(sqrt(x)) && fp::is_nan(sqrt(dx)))
        );
#endif

}

static void check_special_arithmetic() {

list<bigfloat> S, T;
bigfloat x,y;

S.append(bigfloat::pZero);
S.append(bigfloat::nZero);
S.append(bigfloat::pInf);
S.append(bigfloat::nInf);
S.append(bigfloat::NaN);


forall(x,S) 
  forall(y,S) 
    check_special_arithmetic(x,y);

}


static void check_double_conversion() {

  int i,j;
  double powi = 1; 
  double x, y;
  double small = ldexp(1.0,-1000);
  double large = ldexp(1.0, 1000);

  for (i=1;i<=53;i++) // i=1..53
  {
    powi = powi*2;
    x = (powi-1)*small;
    y = -(powi-1)*large;
    for (j=24;j<=52+i;j++) // j= 24..52+i
    { 
      x = x/double(2);
      y = 2*y;
      assert(x == to_double(bigfloat(x)));
      assert(y == to_double(bigfloat(y)));
    }
  }

}

static void check_integer_conversion() {

// Holger Kappels test with to_integer and floor

  int i,j;
  bigfloat x;

  for(i=-100; i<100; i++) // i=-100..100
  {
    x = i;
    x = x/bigfloat(50);
    assert(2*abs(to_integer(x)-x) <= 1);
    assert (floor(x) <= x && x-floor(x) < 1);
    assert (ceil(x)  >= x && ceil(x)-x  < 1);
    bigfloat check = bigfloat(i+1)/bigfloat(50);
    assert(x <= check);
    assert(x > bigfloat(i-1)/bigfloat(50));
  }

// some more tests that compare with standard C floor and ceil

  for (i=1;i<200;i++)   // i=1..200
    for (j=0;j<100;j++) // j = 0..100
  {
    double dx = sin(double(i))*pow(2.0,j);
    x = dx;
    assert (floor(dx) == floor(dx));
    assert (ceil(dx)  == ceil(dx));
    bool exact;
    integer ix = to_integer(x,TO_NEAREST,exact);
    if (exact) assert (x==ix);
    else assert (x!=ix);
    bigfloat dist = abs(ix-x);
    assert (0<=dist && dist<=0.5);
  }

// now generate a case where rounding up overflows
  digit vec[2];
  vec[0]=vec[1]=~0UL;
  integer ones(2,vec);
  bigfloat check(ones,-long(DIGIT_LENGTH));
  integer result = to_integer(check);
  assert (result==(integer(1)<<DIGIT_LENGTH));
}



static double split(double a, int s, double& alo) {

// Shewchuk Th.17., p.18 ('Arbitrary fp arithmetic')

  double c = (1+pow(2.0,s))*a;
  double abig = c-a;
  double ahi  = c-abig;
         alo  = a-ahi;
  return ahi;
}

static void check_rounding_float() {

  int i, j;
  bigfloat x, y, z;
  double dx;
  double fx;
  bool exact;

  for (i=1; i<= 200; i++) // i=1..200 
    for (j=0; j <= 51; j++ ) // j=0..51 
  {
    double low;
    dx = split(sin(double(i)),j,low);

    dx = sin(double(i));

    fx = dx;
    x = bigfloat(dx);
    y = round(x,24);
     
    //assert(bigfloat(fx) == y);
    assert(dx == to_double(x));

    z = round(y,24,TO_NEAREST,exact,1);
    assert(z == y && !exact);
    z = round(y,24,TO_NEAREST,exact,-1);
    assert(z == y && !exact);
    z = round(y,24,TO_NEAREST,exact,0);
    assert(z == y && exact);

    z = round(y,24,TO_P_INF,exact,1);
    assert(z > y && !exact);
    z = round(y,24,TO_P_INF,exact,-1);
    assert(z == y && !exact);

    z = round(y,24,TO_N_INF,exact,1);
    assert(z == y && !exact);
    z = round(y,24,TO_N_INF,exact,-1);
    assert(z < y && !exact);

    z = round(y,24,TO_ZERO,exact,1);
    if (sign(z) > 0)
      assert(z == y && !exact);
    else
      assert(z > y && !exact);
    z = round(y,24,TO_ZERO,exact,-1);
    if (sign(z) > 0)
      assert(z < y && !exact);
    else
      assert(z == y && !exact);

    z = round(y,24,TO_INF,exact,1);
    if (sign(z) > 0)
      assert(z > y && !exact);
    else
      assert(z == y && !exact);
    z = round(y,24,TO_INF,exact,-1);
    if (sign(z) > 0)
      assert(z == y && !exact);
    else
      assert(z < y && !exact);
  }
}

static void check_rounding_overflow() {

  digit vec[3];
  vec[1]=~0UL;
  sz_t i,j,k;
  bool exact;
  for (i=0;i<DIGIT_LENGTH;i++)
    for (j=0;j<=DIGIT_LENGTH;j++)
  {
     vec[0]=(~0UL)-(1UL<<i)+1;
       // i trailing zeros, rest ones
     vec[2]=(j==DIGIT_LENGTH?~0UL:(1UL<<j)-1);
       // j trailing ones, rest zeros
     integer ones(3,vec);
     bigfloat check = ones;
     for (k=DIGIT_LENGTH;k<2*DIGIT_LENGTH-i+j;k++) {
       bigfloat rd = round(check,k,TO_P_INF,exact,0);
       bigfloat power = ipow2(2*DIGIT_LENGTH+j);
       assert (rd==power);
       assert (!exact);
       rd = round(check,k,TO_NEAREST,exact,0);
       assert (rd==power);
       rd = round(check,k,TO_N_INF,exact,0);
       assert (rd==round(power,k,TO_N_INF,exact,-1));
     }
  }
}



static void check_exactness() {

   bool exact;
   bigfloat x,y,result,root;
 
   result = div(bigfloat(4),bigfloat(2),2,TO_NEAREST,exact);
   assert(result == 2 && exact);
   result = div(bigfloat(4),bigfloat(3),2,TO_NEAREST,exact);
   assert(!exact);

   result = div(bigfloat(4),bigfloat(2),2,TO_P_INF,exact);
   assert(result == 2 && exact);
   result = div(bigfloat(4),bigfloat(3),2,TO_P_INF,exact);
   assert(!exact);

   result = div(bigfloat(4),bigfloat(2),2,TO_N_INF,exact);
   assert(result == 2 && exact);
   result = div(bigfloat(4),bigfloat(3),2,TO_N_INF,exact);
   assert(!exact);
  
   result = div(bigfloat(4),bigfloat(2),2,TO_INF,exact);
   assert(result == 2 && exact);
   result = div(bigfloat(4),bigfloat(3),2,TO_INF,exact);
   assert(!exact);

   result = div(bigfloat(4),bigfloat(2),2,TO_ZERO,exact);
   assert(result == 2 && exact);
   result = div(bigfloat(4),bigfloat(3),2,TO_ZERO,exact);
   assert(!exact);

   result = sub(x,x,2,TO_NEAREST,exact);
   assert(result == 0 && exact);
   result = sub(x,x,2,TO_N_INF,exact);
   assert(result == 0 && exact);
   result = sub(x,x,2,TO_P_INF,exact);
   assert(result == 0 && exact);
   result = sub(x,x,2,TO_ZERO,exact);
   assert(result == 0 && exact);
   result = sub(x,x,2,TO_INF,exact);
   assert(result == 0 && exact);

   root = sqrt(bigfloat(4),2,TO_NEAREST,exact);
   assert(root == 2);
   assert(exact);
   root = sqrt(bigfloat(5),2,TO_NEAREST,exact);
   assert(!exact);

   root = sqrt(bigfloat(4),2,TO_ZERO,exact);
   assert(root == 2 && exact);
   root = sqrt(bigfloat(5),2,TO_ZERO,exact);
   assert(!exact);

   root = sqrt(bigfloat(4),2,TO_N_INF,exact);
   assert(root == 2 && exact);
   root = sqrt(bigfloat(5),2,TO_N_INF,exact);
   assert(!exact);

   root = sqrt(bigfloat(4),2,TO_P_INF,exact);
   assert(root == 2 && exact);
   root = sqrt(bigfloat(5),2,TO_P_INF,exact);
   assert(!exact);

   root = sqrt(bigfloat(4),2,TO_INF,exact);
   assert(root == 2 && exact);
   root = sqrt(bigfloat(5),2,TO_INF,exact);
   assert(!exact);

}


static void check_pattern_arithmetic() {

  double x, y;
  int exp;
  bigfloat x_bf, y_bf;
  int i, j;

  for (i=1;i<=25;i++) // i=1..25
  { 
    for (j=1;j<=i;j++)                    // j=1..i
      for (exp=1;exp<=53 + j - i;exp++)   // exp=1..53
    {
      x = (pow(2.0,i)-1)*pow(2.0,exp);
      y = pow(2.0,j)-1;

      // x = 11  ...  1 * 2^exp
      //     [i times ]

      // y = 11  ...  1
      //     [j times ]

      x_bf = x; y_bf = y;

      assert(x==to_double(x_bf));
      assert(y==to_double(y_bf));
      assert(x+y == to_double(add(x_bf,y_bf,53,TO_NEAREST)));
      assert(y-x == to_double(sub(y_bf,x_bf,53,TO_NEAREST))); 
      if (exp == 0) // here exp not important
        assert(x*y == to_double(mul(x_bf,y_bf,53,TO_NEAREST)));
      if (exp == 0) // here exp not important
        assert(x/y == to_double(div(x_bf,y_bf,53,TO_NEAREST)));
    } // for j, exp

  } // for i

}

#ifndef NO_FPU
static void check_double_mode(
volatile double dx, volatile double dy, rounding_modes rmode) {

  bigfloat x=dx, y=dy;

  leda_fpu::round_nearest();
  bigfloat exact_add = add(x,y,53,EXACT);
  bigfloat exact_sub = sub(x,y,53,EXACT);
  bigfloat exact_mul = mul(x,y,53,EXACT);

  switch (rmode) {
    case TO_NEAREST:
      leda_fpu::round_nearest();
      break;
    case TO_P_INF:
      leda_fpu::round_up();
      break;
    case TO_N_INF:
      leda_fpu::round_down();
      break;
    case TO_ZERO:
      leda_fpu::round_tozero();
      break;
    default:
      error_handler(1,"error: mode not IEEE754");
  }

  assert (round(exact_add,53,rmode) == dx+dy);
  assert (add(x,y,53,rmode) == dx+dy);
  assert (round(exact_sub,53,rmode) == dx-dy);
  assert (sub(x,y,53,rmode) == dx-dy);
  assert (round(exact_mul,53,rmode) == dx*dy);
  assert (mul(x,y,53,rmode) == dx*dy);
  assert (div(x,y,53,rmode) == dx/dy);

  leda_fpu::round_nearest();

}
#endif

static void check_double_arithmetic() {

   int i, j;
   bigfloat x, y;
   double dx, dy;

   for (i=1; i<= 50; i++) { // i=1..50 
      
     for (j=2; j<= 50; j++) { // j=2..50

       dx = sin(double(i));
       x = bigfloat(dx);

       dy = cos(double(j));
       y = bigfloat(dy);

       // conversion:
       assert(to_double(x) == dx);
       assert(to_double(y) == dy);

       // comparison:

       assert((x > y)  == (dx > dy));
       assert((x < y)  == (dx < dy));
       assert((x >= y) == (dx >= dy));
       assert((x <= y) == (dx <= dy));

       // arithmetic:
#ifndef NO_FPU
       check_double_mode(dx,dy,TO_NEAREST);
       check_double_mode(dx,dy,TO_P_INF);
       check_double_mode(dx,dy,TO_N_INF);
       check_double_mode(dx,dy,TO_ZERO);
#endif
    }
  }
}




static void check_sqrt() {

  int prec;
  bigfloat seven = 7, sqrt7;
  bigfloat sqrt_low,sqrt_high;
  bigfloat sum;
  integer exponent, sig;

  for (prec=7;prec<=100000;prec*=3) // prec=7..100000,prec*=3
  {

    sqrt7 = sqrt(seven,prec,TO_NEAREST);
    exponent = sqrt7.get_exponent();
    sig = sqrt7.get_significant()-1;
    // sqrt_low  = bigfloat(sig,exponent);
    sqrt_low = sqrt(seven,prec,TO_N_INF);
    sig = sqrt7.get_significant()+1;
    // sqrt_high = bigfloat(sig,exponent);
    sqrt_high = sqrt(seven,prec,TO_P_INF);

    // first we test whether sqrt_low is less than sqrt(7) and whether
    // sqrt_high is more than sqrt(7)
 
    assert(mul(sqrt_low ,sqrt_low ,53,EXACT) < seven);
    assert(mul(sqrt_high,sqrt_high,53,EXACT) > seven);

    // now we test whether sqrt7 really is nearer to sqrt(7) than 
    // sqrt_low and sqrt_high
 
    if  (mul(sqrt7,sqrt7,53,EXACT) >= seven)
    {
      // compare sqrt7 - sqrt(7) and sqrt(7) - sqrt_low
      sum = add(sqrt7,sqrt_low,1,EXACT);
      assert(mul(sum,sum,53,EXACT) <= bigfloat(28));
    }
    else
    {
      // compare sqrt(7) - sqrt7 and sqrt_high - sqrt(7)
      sum = add(sqrt7,sqrt_high,1,EXACT);
      assert(mul(sum,sum,53,EXACT) >= bigfloat(28));
    }

  } // for

}



static void check_long_exponent_arithmetic() {

  double dx = sin(1.0);
  double dy = sin(2.0);
  integer xexp = integer(1)<<100;
  integer yexp = (integer(1)<<10)+(integer(1)<<100);
  bigfloat x(bigfloat(dx).get_significant(),xexp);
  bigfloat y(bigfloat(dy).get_significant(),yexp);
  assert (x+x==2*x);
  bigfloat::set_rounding_mode(EXACT);
  assert ((x+y)-y==x);

  bigfloat z = y+x;
  bigfloat w = x+y;
  assert (z==w);
  z = z-x;
  assert (z==y);
  assert ((y+x)-x==y);
  yexp = integer(1)<<200;
  y = bigfloat(bigfloat(dy).get_significant(),yexp);
  assert ((x*y)/x==y);
  assert (sqr(x)/x==x);

  assert (sqrt(y*y,1000,TO_NEAREST)==sign(y)*y);
  assert ((y*x)/y==x);

  bigfloat::set_rounding_mode(TO_NEAREST);

}



static void check_inoutput(int i) {

  double log10_2 = log10(2.0);
  sz_t bin_prec = i;
  sz_t dec_prec = 1+sz_t(bin_prec*log10_2);
  integer irand = abs(integer::random(i));
  bigfloat root = sqrt(bigfloat(irand),bin_prec+1);

  bigfloat::set_output_precision(dec_prec);
  bigfloat::set_input_precision (bin_prec);

  bigfloat bin;
  string str = root.to_string();
  bin.from_string(str);
  assert(bin.to_string()==str);
  assert(abs(bin-root)<=abs(root)*ipow2(2-long(bin_prec)));

}

static void check_inoutput() {
  for (int i = 37; i<= 20000; i*=3)
    check_inoutput(i);
}

static bigfloat power10(int i, sz_t prec) {

  bigfloat power = 10.0;
  bool is_negative = (i<0);
  if (is_negative) i=-i;
  for (int j=0;j<i;j++)
    power=sqr(power,prec);
  if (is_negative)
    power=div(1.0,power,prec);
  return power;

}

static void check_special_inoutput(int i) {

  double log10_2 = log10(2.0);
  sz_t dec_prec = (i>0 ? 1<<i : 1<<(-i));
  sz_t bin_prec = sz_t(dec_prec/log10_2);
  bigfloat power = power10(i,bin_prec);
  bigfloat::set_output_precision(dec_prec-1);
  bigfloat::set_input_precision (bin_prec);

  bigfloat bin;
  string str = power.to_string();
  bin.from_string(str);
  integer I = (i>0 ? 1<<i : 1<<(-i));
  string Istr = I.to_string();
  char* istr = Istr.cstring();
  char* check_str = new char[68];
  check_str[0]='1';
  check_str[1]='e';
  if (i>0) check_str[2]='+';
  else check_str[2]='-';
  check_str[3]='\0';
  strcat(check_str,istr);
  assert(check_str==str);
  assert(bin.to_string()==str);
  assert(abs(bin-power)<=abs(power)*ipow2(1-long(bin_prec)));
  delete [] check_str;

}

static void check_special_inoutput() {

  int M = 12; 

  for (int i=M; i >= -M; i--)
  { if (i == 0) continue;
    check_special_inoutput(i);
   }

  assert (bigfloat(0).to_string() == "0");
  assert (bigfloat::pInf.to_string() == "pInf");
  assert (bigfloat::nInf.to_string() == "nInf");
  assert (bigfloat::NaN.to_string() == "NaN");

}


int main() {

#ifndef NO_FPU
  leda_fpu::set_defaults();
#endif

  cout << "\nStart program bigfloat_test.\n";

  cout << "check special values:" << flush;
  check_special_basic();
  check_special_arithmetic();
  cout << "...done.\n" << flush;

  cout << "check conversions: " << flush;
  check_double_conversion();
  check_integer_conversion();
  cout << "...done.\n" << flush;

  cout << "check rounding: " << flush;
  check_rounding_float();
  check_rounding_overflow();
  cout << "...done.\n" << flush;

  cout << "check arithmetic: " << flush;
  check_exactness();
  check_pattern_arithmetic();
  check_double_arithmetic();
  cout << "...done.\n" << flush;

  cout << "check sqrt: " << flush;
  check_sqrt();
  cout << "...done.\n" << flush;

  cout << "check long exponent arithmetic: " << flush;
  check_long_exponent_arithmetic();
  cout << "...done.\n" << flush;

  cout << "check input/output: " << flush;
  check_inoutput();
  check_special_inoutput();
  cout << "...done.\n" << flush;

  cout << "...all tests successfully completed!\n";

  return 0;
}

