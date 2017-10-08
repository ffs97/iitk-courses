/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rational_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/numbers/rational.h>
#include <assert.h>
#include <math.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::flush;
using std::endl;
#endif


rational rat(int i, int j) {

  integer x = integer::random(i);
  integer y = integer::random(j);
  if (y.iszero()) y=1;
  return rational(x,y);

}

void check_addsub(rational x, rational y, rational w) {

  rational z;
  z=(x+y)+w;
  z=(z-x)-y;
  assert(z==w);

}

void check_muldiv(rational x, rational y, rational w) {

  rational z;
  z=(x*y)*w;
  if (x==0||y==0||w==0)
    assert(z==0);
  else {
    z=(z/x)/y;
    assert(z==w);
  }

}

void check_arithmetic_ops(int in, int id, int jn, int jd) {

  rational xp=rat(in,id);
  rational yp=rat(jn,jd);
  rational xm=-xp;
  rational ym=-yp;
  rational w=rat(in+id,jn+jd);

  check_addsub(xp,yp,w);
  check_addsub(xm,yp,w);
  check_addsub(xp,ym,w);
  check_addsub(xm,ym,w);

  check_muldiv(xp,yp,w);
  check_muldiv(xm,yp,w);
  check_muldiv(xp,ym,w);
  check_muldiv(xm,ym,w);

}

void check_simplifications(int i, int j, int k) {

  integer common = integer::random(i);
  integer num = integer::random(j);
  integer den = integer::random(k);

  if (common.iszero()) common=1;
  if (num.iszero()) num=1;
  if (den.iszero()) den=1;

  rational x(common*num,common*den);
  rational y = x;
  y.simplify(common);
  assert (y.numerator()  <=num);
  assert (y.denominator()<=den);
  y=x;
  y.normalize();
  assert (y.numerator()  <=num);
  assert (y.denominator()<=den);

  integer ggt = gcd(num,den);
  y=rational(num,den);
  y.simplify(ggt);
  num=y.numerator();
  den=y.denominator();
  assert (gcd(num,den)==1);
  y=rational(num*common,den*common);
  y.normalize();
  assert (y.numerator()==num);
  assert (y.denominator()==den); 

}


void check_double_conversion(int k) {

  integer x=3;
  x<<=2000;
  integer y=7;
  y<<=2000;
  rational z(x,y);
  double max_error = ldexp(3*3.0/7.0,-53);
  double act_error = fabs(z.to_double()-3.0/7.0);
  assert (act_error<max_error);

  int i,j;
  for (i=0;i<k;i++) {
    for (j=0;j<53;j++) {
      double sinus = floor(ldexp(sin(double(i)),j));
      rational Sinus(sinus);
      assert (Sinus.to_double()==sinus);
    }
  }

  for (i=-1074;i<-1000;i++) {
    double sinus = ldexp(sin(1.0),i);
    rational Sinus(sinus);
    assert (Sinus.to_double()==sinus);
  }
}


int main() {

  cout << "\nStart program rational_test.\n";

  int i,j,k,l;

  cout<<"check arithmetic operations:"<<flush;;
  for (i=0;i<128;i+=11) {
    cout << "."<<flush;
    for (j=0;j<128;j+=13)
      for (k=0;k<128;k+=15)
        for (l=0;l<128;l+=17)
           check_arithmetic_ops(i,j,k,l);
  }
  cout << "done.\n"<<flush;

  cout<<"check simplifications:"<<flush;;
  for (i=1;i<128;i+=7) {
    cout << "."<<flush;
    for (j=1;j<128;j+=11)
      for (k=1;k<128;k+=13)
	check_simplifications(i,j,k);
  }
  cout << "done.\n"<<flush;

  cout<<"miscellaneous checks:"<<flush;;
  string s=rational(3,7).tostring();
  assert (s=="3/7");

  check_double_conversion(100);
  cout << "done.\n"<<flush;

  cout << "... all tests successfully completed!\n";

  return 0;

}

