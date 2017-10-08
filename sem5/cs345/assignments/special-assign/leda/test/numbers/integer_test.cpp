/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  integer_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/numbers/integer.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::flush;
using std::endl;
#endif


void perform_arithmetic_ops(int i, int j) {

  integer xplus  =  integer::random(i);
  integer xminus = -integer::random(i);
  integer yplus  =  integer::random(j);
  integer yminus = -integer::random(j);

  integer z;
  z = xplus*yplus; z = xminus*yplus; z = xplus*yminus; z = xminus*yminus;
  z = xplus+yplus; z = xminus+yplus; z = xplus+yminus; z = xminus+yminus;

  z = xplus-yplus; z = xminus-yplus; z = xplus-yminus; z = xminus-yminus;

  if (yplus!=0) { z = xplus/yplus; z = xminus/yplus; }
  if (yminus!=0) { z = xplus/yminus; z = xminus/yminus; }

}



void test_muldiv(int i, int j) {

  integer x = integer::random(i)+2;
  integer y = integer::random(j)+2;
  integer z = x*y;

  integer r=0;
  integer q;

  q = z.div(y,r);     

if (r != 0 || q != x)
{ cout << endl;
  cout << "z = " << z << endl;
  cout << "y = " << y << endl;
  cout << "q = " << q << endl;
  cout << "r = " << r << endl;
}
                      assert (r==0 && q==x);
  q = (z+1).div(y,r); assert (r==1 && q==x);
  q = (z-1).div(y,r); assert (r==y-1 && q==x-1);

  z = x.square();     
  if (z/x != x)
  { cout << endl;
    x.hex_print(cout); cout << endl;
    z.hex_print(cout); cout << endl;
    (x*x).hex_print(cout); cout << endl;
   }
assert (z/x==x);

}



void test_sqrt(int i) {

  integer x = integer::random(i)+2;
  integer z = x.square();

  integer root=0;

  root=sqrt(z);   assert(root==x);
  root=sqrt(z+1); assert(root==x);
  if (z>0) {
    root=sqrt(z-1);
    assert(root==x-1);
  }
}


void test_shifting(int i) {

  integer x = integer::random(i)+1;
  integer z;
  integer lshift;
  for (int l=0;l<65;l++) {
    lshift = x<<l;
    assert (lshift.zeros()==x.zeros()+l);
    assert (log(lshift)==log(x)+l);
    z = lshift>>l;
    assert (z==x); 
  }

}


void test_andor(int i) {

  if (i<=2) return;
  integer pow_m = (integer(1)<<i);
  integer x = integer::random(i);
  integer y = integer::random(i);
  x = x%pow_m; y = y%pow_m;
  x |= pow_m;  y |= pow_m;
  integer nx = ~x;
  integer ny = ~y; 
  assert(~nx==x);
  assert(~ny==y);
  integer v = x&y;
  integer w = x|y;
  integer nv = ~v;
  integer nw = ~w;
  integer ior  = nx | ny;
  integer iand = nx & ny;
  assert ( nv == ior  );
  assert ( nw == iand );
 
}



void test_modops(int i, int j) {

  int l = (i<j?i:j);
  integer x = integer::random(i)+1;
  integer y = integer::random(j)+1;
  integer n = integer::random(l)+1;

  integer z, zmod;
  z = (x*y)%n;
  zmod = modMul(x,y,n);
  assert (z==zmod);
  z = x.square()%n;
  zmod = modSquare(x,n);
  assert (z==zmod);
}



integer modExpPlain(const integer & x, long e, const integer& n)
{
  integer z = 1, y = x;
  long e_prefix = e;

  while (e_prefix > 0) {
    if (e_prefix % 2) 
      z=(z*y)%n; 
    e_prefix = e_prefix / 2;
    y = (y*y)%n;
  }
  return z;
}

void test_modexp(int i) {

  integer _x = integer::random(i);
  integer  n = integer::random(i)+1;
  integer  x = _x%n;
 
  integer z = modExpPlain(x,i,n);
  integer zmod = modExp(x,i,n);
  assert(z==zmod);

}




void test_double_conversion(int i) {

  if (i > 1020)
    return;
  double sinus = ldexp(sin(double(i)),i);
  if (sinus < 0)
    sinus = -sinus;
  double sinfl = floor(sinus);
  
  integer s,t;
  s = sinfl;
  t = s.to_double();
  assert (s.to_double()==sinfl);
  assert (s==t);
  s = sinus;
  assert (s == integer(sinfl));
 
}


void test_conversion() {
  unsigned long dig = ~1000UL;
  integer idig = dig;
  assert(idig>0);
  assert(idig[0]==dig);
}


void check_inoutput(int i) {

  integer r1 = integer::random(i);
  integer r2 = -integer::random(i);
  string s1 = r1.to_string();
  string s2 = r2.to_string();
  assert(r1==integer(s1));
  assert(r2==integer(s2));
  integer _r1, _r2;
  _r1.from_string(s1);
  _r2.from_string(s2);
  assert (r1==_r1);
  assert (r2==_r2);

}



void test_gcd() {
  integer a = "713623846352979940529142984724184618237952000";
  integer b = "178405961588244985132285746181186892047843328";
  integer GCD = gcd(a*b,b);
  assert (GCD == b);
  integer _a = (a*b)/b;
  assert (_a==a);
}


void KnuthTest() {

  int MAXLOOP=100;
  const digit MAX_DIGIT = ~digit(0);
  int i,m,n;

  for(m=1;m<=MAXLOOP;m++) {
    for(n=m+1;n<=MAXLOOP;n++) {
  
      integer t(MAX_DIGIT);
      t++;
      integer prod1(1), prod2(1);
      
      for(i=0;i<m;i++) prod1*=t;
      prod1--;
      for(i=0;i<n;i++) prod2*=t;
      prod2--;
 
      integer prod=prod1*prod2;
      assert(prod[0]==1); 
      for(i=1;i<=m-1;i++)
        assert(prod[i]==0); 
      for(i=m;i<=n-1;i++)
        assert(prod[i]==t-1);
      assert(prod[n]==t-2); 
      for(i=n+1;i<=n+m-1;i++)
        assert(prod[i]==t-1);
      
      prod/=prod1;
      prod/=prod2;
      assert (prod==1);
    }
    
    if(m%10==0) 
      cout<<"for m in ["<<m-10<<","<<m<<"] completed\n";
    cout << flush;
  }

}




int main() {

  cout << "\nStart program integer_test.\n";

  int i,j;
  
#ifdef INTEGER_PUB_CHECKS_ON
  integer::activate_checking();
#endif
 
  rand_int.set_seed(1234567);

  cout << "check basic arithmetic, phase 1: " << flush;
  for (i=0;i<150;i++) {
    test_conversion();
    test_double_conversion(i);
    for (j=0;j<150;j++) 
      perform_arithmetic_ops(i,j);
  }

  cout <<"okay. \nphase 2: " << flush;
  for (i=0;i<2000;i+=33) {
    cout << "." << flush;
    test_sqrt(i);
    for (j=0;j<2000;j+=34) {
      perform_arithmetic_ops(i,j);
      test_muldiv(i,j);
    }
  }
  test_gcd();
  cout << " done. \n" << flush;

  cout << "check binary operations: " << flush;
  for (i=0;i<150;i++) {
    test_shifting(i);
    test_andor(i);
  }
  cout << "... done. \n" << flush;
  
  cout << "check modular operations: " << flush;
  for (i=0;i<150;i++) {
    test_modexp(i);
  }
  for (i=0;i<150;i++)
    for (j=0;j<150;j++)
      test_modops(i,j);
  cout << "... done. \n" << flush;

  cout << "check in/output: " << flush;
  for (i=0;i<400;i++) 
    check_inoutput(i);
  for (i=100;i<5000;i*=2) 
    check_inoutput(i);
  cout << "... done. \n" << flush;

  cout << "Knuth test routine: \n" << flush;
  KnuthTest();
  cout << "... done. \n" << flush;

  cout << "... all tests successfully completed!\n";

  return 0;
}









