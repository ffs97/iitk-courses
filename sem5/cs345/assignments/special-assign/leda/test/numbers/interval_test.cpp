/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  interval_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/numbers/interval.h>
#include <math.h>
#include <assert.h>

#if defined(__aCC__)
#define LEDA_NO_FPU
#endif

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::flush;
using std::endl;
#endif


#if defined(__mipspro__) || defined(__aCC__)
inline double sin(int x) { return sin(double(x)); }
inline double cos(int x) { return sin(double(x)); }
#endif


template <class filtered_number>
void check_mul_equal
(filtered_number x, filtered_number y, VOLATILE_I double inf, VOLATILE_I double sup)
{
  filtered_number z=x*y;
  assert(z.lower_bound()==inf);
  assert (z.upper_bound()==sup);
  z = y*x;
  assert (z.lower_bound()==inf);
  assert (z.upper_bound()==sup);
}

void check_mul_equal
(interval_bound_relative x, interval_bound_relative y, 
 VOLATILE_I double inf, VOLATILE_I double sup)
{
  interval_bound_relative z=x*y;
  assert (z.lower_bound()<=inf);
  assert (z.upper_bound()>=sup);
  z = y*x;
  assert (z.lower_bound()<=inf);
  assert (z.upper_bound()>=sup);
}

void check_mul_equal
(interval_bound_absolute x, interval_bound_absolute y, 
 VOLATILE_I double inf, VOLATILE_I double sup)
{
  interval_bound_absolute z=x*y;
  assert (z.lower_bound()<=inf);
  assert (z.upper_bound()>=sup);
  z = y*x;
  assert (z.lower_bound()<=inf);
  assert (z.upper_bound()>=sup);
}


template <class filtered_number>
void check_div_equal
(filtered_number x, filtered_number y, 
 VOLATILE_I double inf, VOLATILE_I double sup)
{
  filtered_number z=x/y;
  assert (z.lower_bound()==inf);
  assert (z.upper_bound()==sup);
}

void check_div_equal
(interval_bound_relative x, interval_bound_relative y, 
 VOLATILE_I double inf, VOLATILE_I double sup)
{
  interval_bound_relative z;
  z=x/y;
  assert (z.lower_bound()<=inf);
  assert (z.upper_bound()>=sup);
}

void check_div_equal
(interval_bound_absolute x, interval_bound_absolute y, 
 VOLATILE_I double inf, VOLATILE_I double sup)
{
  interval_bound_absolute z;
  z=x/y;
  assert (z.lower_bound()<=inf);
  assert (z.upper_bound()>=sup);
}

template <class filtered_number>
void check_filtered_number(filtered_number z)
{
  z.set_range(-1,1);
  
{
  filtered_number x,y;
  x.set_range(1,2); 
  y.set_range(2,3);
  check_mul_equal(x,y,2,6);
  x.set_midpoint(1.5,0.5);
  y.set_midpoint(2.5,0.5);
  check_mul_equal(x,y,2,6);
  x.set_range(-2,-1); 
  check_mul_equal(x,y,-6,-2);
  y.set_range(-3,-2);
  check_mul_equal(x,y,2,6);
  x.set_range(3,7);
  y.set_range(-2,5);
  check_mul_equal(x,y,-14,35);
  x.set_range(-4,5);
  y.set_range(-6,5);
  check_mul_equal(x,y,-30,25);
  y.set_range(-5,3);
  check_mul_equal(x,y,-25,20);
  
  x.set_range(3,5);
  y.set_range(1,2);
  check_div_equal(x,y,1.5,5);
  y=-y;
  check_div_equal(x,y,-5,-1.5);
  x=-x;
  check_div_equal(x,y,1.5,5);
  x.set_range(-2,3); 
  y.set_range(1,2);
  check_div_equal(x,y,-2,3);
  x = -x; y = -y;
  check_div_equal(x,y,-2,3);
  y.set_range(-1,1);
  check_div_equal(x,y,-fp::pInf,fp::pInf);
  x.set_range(1,1);
  check_div_equal(x,y,-fp::pInf,fp::pInf);
  x=-x;
  check_div_equal(x,y,-fp::pInf,fp::pInf);
}

  
{
  filtered_number 
    one = 1, two = 2, three = 3, four = 4, 
    five = 5, six = 6, seven = 7, nine = 9,
    twelve = 12, twenty = 20, 
    twenty_five = 25,
    twenty_nine = 29;
  
  filtered_number root_two = sqrt(two); 
  filtered_number root_three = sqrt(three); 
  filtered_number root_five = sqrt(five);
  filtered_number root_six = sqrt(six);
  filtered_number root_seven = sqrt(seven);
  filtered_number root_twenty_nine = sqrt(twenty_nine);

  filtered_number third_root_two = root(two,3);
  filtered_number third_root_four = root(four,3);
  filtered_number third_root_five = root(five,3);
  filtered_number third_root_twenty = root(twenty,3);
  filtered_number third_root_twenty_five = root(twenty_five,3);
  
  filtered_number test;
  test = sqrt(9+4*root_two)/(1+2*root_two)-1;
  assert(!test.sign_is_known());
  // Davenport 
  test = (sqrt(5+2*root_six) + sqrt(5-2*root_six))/(2*sqrt(three))-1;
  assert(!test.sign_is_known());
  // Zippel 
  test = (sqrt(16-2*root_twenty_nine+2*sqrt(55-10*root_twenty_nine)))/
    (sqrt(22+2*root_five)-sqrt(11+2*root_twenty_nine) + root_five)-1;
  assert(!test.sign_is_known());
  // Shanks
  test = (sqrt((112+70*root_two)+(46+34*root_two)*root_five))/
    ((5+4*root_two)+(3+root_two)*root_five)-1;
  assert(!test.sign_is_known());
  // Borodin
  test = sqrt(3+root_two)*sqrt(6+root_seven)/(3+root_two+root_seven)-1;
  assert(test.sign_is_known());
  // test is nonzero

  test = 3*sqrt(third_root_five-third_root_four)
         -third_root_two-third_root_twenty+third_root_twenty_five;
  assert(!test.sign_is_known());
  // Ramanujan 1
  test =  root(nine*(seven*third_root_twenty-19),6) 
         -third_root_five + root(two,3);
  assert(!test.sign_is_known());
  // Ramanujan 2
  test = root(root_five+2,3)-root(root_five-2,3)-1;
  assert(!test.sign_is_known());
  // Landau
  test = sqrt(3+2*root_three)-one/four*root(twelve,4)*(2+sqrt(twelve));
  assert(!test.sign_is_known());
  // Borodin 1
  test = sqrt(4+3*root_two) - root(two,4)*(1+root_two);
  assert(!test.sign_is_known());
  // Borodin 2
  test = sqrt(4+3.01*root_two) - root(two,4)*(1+root_two);
  assert(test.sign_is_known());
  // test is nonzero

  filtered_number b = 4;
  test = (sqrt(b+1) - sqrt(b))*(sqrt(b+1) + sqrt(b)) - 1;
  assert(!test.sign_is_known());
  
}

  
#ifndef interval_safe_mode
#ifdef IEEE_FPU_H
  leda_fpu::round_up();
#endif
#endif
{
  filtered_number x, y;
  
  // underflow:
  x = ldexp(1.0,-1000);
  assert(x.sign_is_known());
  assert(x.sign()==1);

#if !(defined(linux)&&defined(alpha)&&defined(__GNUC__)) && !defined(__BORLANDC__)
  // caution: this does not work on g++/linux/DEC alpha
  y = x*x; // underflows!
  assert(!(y.sign_is_known()));
  x.set_range(-ldexp(1.0,-1000),ldexp(1.0,-1000));
  y = x*x;
  assert(!(y.sign_is_known()));
  y = (-x)*x;
  assert(!(y.sign_is_known()));
  y = x*(-y);
  assert(!(y.sign_is_known()));
#endif
  
  // constructor from integers:
  integer I=1;
  I<<=100;
  I=sqrt(I);
  assert(filtered_number(I).get_double_error()==0);
  assert(I==integer(filtered_number(I).to_double()));
  
  // constructor from rationals:
  rational R(2,3);
  filtered_number T;
  y=3*T-2;
  if (y.sign_is_known())
    assert(sign(y)!=0);
  
  // interval(infinity) should be always [-oo,+oo]
  y=filtered_number(fp::nInf);
  assert(y.lower_bound()==fp::nInf);
  if(y.upper_bound()!=fp::pInf) 
    error_handler(1,"error?");
  
#ifdef with_exactness_check
  // exactness of operations
  x=4;
#ifdef SQRT_ROUNDING_OKAY
  y=sqrt(x);
  assert(y.get_double_error()==0);
#else
  y=2;
#endif
  assert(y.lower_bound()-2==0);
  assert(y.upper_bound()-2==0);
  y/=2;
  assert(y.get_double_error()==0);
  assert(y.lower_bound()-1==0);
  assert(y.upper_bound()-1==0);
#endif
  
  // +0,-0, Infinity, NaN...
  
  // NaN, +Infifnity, -Infinity are mapped to [-Infinfity,+Infinity]
  x = filtered_number(fp::NaN);
  assert(x.lower_bound()==-fp::pInf);
  assert(x.upper_bound()==+fp::pInf);
  x = filtered_number(fp::pInf);
  assert(x.lower_bound()==-fp::pInf);
  assert(x.upper_bound()==+fp::pInf);
  x = filtered_number(-fp::pInf);
  assert(x.lower_bound()==-fp::pInf);
  assert(x.upper_bound()==+fp::pInf);
  
#ifdef interval_avoid_nan   
  // the following calculations should not produce a NaN!
  x = sqrt(filtered_number(2));
  x*=x; x-=2;
  assert(x.sign_is_known()==false);
  filtered_number w = 1/x;
  filtered_number z = x*w;
  assert(z.lower_bound()==-fp::pInf);
  assert(z.upper_bound()==+fp::pInf);
  z = w*x;
  assert(z.lower_bound()==-fp::pInf);
  assert(z.upper_bound()==+fp::pInf);
  z = w*(-x);
  assert(z.lower_bound()==-fp::pInf);
  assert(z.upper_bound()==+fp::pInf);
  z = w/fp::pInf;
  assert(z.lower_bound()==-fp::pInf);
  assert(z.upper_bound()==+fp::pInf);
#endif
}

}



template <class filtered_number>
void runtime_filtered_add
(filtered_number X, int repetitions, double x, double y, 
 float& _addtime, float& addtime)
{
  X.set_range(-1,1);
  
  int i; 
  float time; 
  double z=0;
  filtered_number _x, _y, _z;
  double err = ldexp(1.0,-1000);
  double nerr=-err;
  double corr = 1+err;
  filtered_number _corr = corr;_corr*= _corr;
  filtered_number _err = err; _err*=_corr;
  filtered_number _nerr = -_err;
  
  _x = x; _x*=_corr; _x+=_err;
  _y = y; _y*=_corr; _y+= _err;
  
  time=used_time();
  for (i=1;i<=repetitions;i++)
    {
      _z = _y+_nerr; _y=_x+_err;   _x=_z+_err;
      _z = _y+_err;  _y=_x+_nerr;  _x=_z+_err;
      _z = _y+_err;  _y=_x+_err;   _x=_z+_nerr;
      _z = _z+_err;
    }
  _addtime+=used_time(time);
  time=used_time();
  for (i=1;i<=3*repetitions;i++)
    {
      z = y+nerr; y=x+err;   x=z+err;
      z = y+err;  y=x+nerr;  x=z+err;
      z = y+err;  y=x+err;   x=z+nerr;
      z = z+err;
    }
  addtime+=used_time(time)/3;
  if (fp::is_infinite(z))
    error_handler(1,"error");
}


template <class filtered_number>
void runtime_filtered_sub
(filtered_number X, int repetitions, double x, double y, 
 float& _subtime, float& subtime)
{
  X.set_range(-1,1);
  
  int i; 
  float time; 
  double z=0;
  filtered_number _x, _y, _z;
  double err = ldexp(1.0,-1000);
  double nerr=-err;
  double corr = 1+err;
  filtered_number _corr = corr;_corr*= _corr;
  filtered_number _err = err; _err*=_corr;
  filtered_number _nerr = -_err;
  
  _x = x; _x*=_corr; _x+=_err;
  _y = y; _y*=_corr; _y+= _err;
  
  time=used_time();
  for (i=1;i<=repetitions;i++)
    {
      _z = _y-_nerr; _y=_x-_err;   _x=_z-_err;
      _z = _y-_err;  _y=_x-_nerr;  _x=_z-_err;
      _z = _y-_err;  _y=_x-_err;   _x=_z-_nerr;
      _z = _z-_err;
    }
  _subtime+=used_time(time);
  time=used_time();
  for (i=1;i<=3*repetitions;i++)
    {
      z = y-nerr; y=x-err;   x=z-err;
      z = y-err;  y=x-nerr;  x=z-err;
      z = y-err;  y=x-err;   x=z-nerr;
      z = z-err;
    }
  subtime+=used_time(time)/3;
  
  if (fp::is_infinite(z))
    error_handler(1,"error");
}


template <class filtered_number>
void runtime_filtered_mul
(filtered_number X, int repetitions, double x, double y, 
 float& _multime, float& multime)
{
  X.set_range(-1,1);
  
  int i; 
  float time; 
  double z=0;
  filtered_number _x, _y, _z;
  double err = ldexp(1.0,-1000);
  double corr = 1+err;
  double ncorr=-corr;
  filtered_number _corr = corr;_corr*= _corr;
  filtered_number _ncorr = -_corr;
  filtered_number _err = err; _err*=_corr;
  
  _x = x; _x*=_corr; _x+=_err;
  _y = y; _y*=_corr; _y+= _err;
  
  time=used_time();
  for (i=1;i<=repetitions;i++)
    {
      _z = _y*_ncorr; _y=_x*_corr;  _x=_z*_corr;
      _z = _y*_corr;  _y=_x*_ncorr; _x=_z*_corr;
      _z = _y*_corr;  _y=_x*_corr;  _x=_z*_ncorr;
      _z = _z*_corr;
    } 
  _multime+=used_time(time);
  
  time=used_time();
  for (i=1;i<=5*repetitions;i++)
    {
      z = y*ncorr; y=x*corr;  x=z*corr; 
      z = y*corr;  y=x*ncorr; x=z*corr;
      z = y*corr;  y=x*corr;  x=z*ncorr;
      z = z*corr;
    }
  multime+=used_time(time)/5;
  
  if (fp::is_infinite(z))
    error_handler(1,"error");
}


template <class filtered_number>
void runtime_filtered_div
(filtered_number X, int repetitions, double x, double y, 
 float& _divtime, float& divtime)
{
  X.set_range(-1,1);
  
  int i; 
  float time; 
  double z=0;
  filtered_number _x, _y, _z;
  double err = ldexp(1.0,-1000);
  double corr = 1+err;
  double ncorr=-corr;
  filtered_number _corr = corr;_corr*= _corr;
  filtered_number _ncorr = -_corr;
  filtered_number _err = err; _err*=_corr;
  
  _x = x; _x*=_corr; _x+=_err;
  _y = y; _y*=_corr; _y+= _err;
  
  time=used_time();
  for (i=1;i<=repetitions;i++)
    {
      _z = _y/_ncorr; _y=_x/_corr;  _x=_z/_corr; 
      _z = _y/_corr;  _y=_x/_ncorr; _x=_z/_corr; 
      _z = _y/_corr;  _y=_x/_corr;  _x=_z/_ncorr; 
      _z = _z/_corr;
    }
  _divtime+=used_time(time);
  
  time=used_time();
  for (i=1;i<=3*repetitions;i++)
    {
      z = y/ncorr; y=x/corr;  x=z/corr; 
      z = y/corr;  y=x/ncorr; x=z/corr;
      z = y/corr;  y=x/corr;  x=z/ncorr;
      z = z/corr;
    }
  divtime+=used_time(time)/3;
  
  if (fp::is_infinite(z))
    error_handler(1,"error");
}


template <class filtered_number>
void runtime_filtered_sqrt
(filtered_number X, int repetitions, double x, double y, 
 float& _sqrttime, float& sqrttime)
{
  X.set_range(-1,1);
  
  int i; 
  float time; 
  double z=0;
  filtered_number _x, _y, _z;
  double err = ldexp(1.0,-1000);
  double corr = 1+err;
  filtered_number _corr = corr;_corr*= _corr;
  filtered_number _err = err; _err*=_corr;
  
  x = fabs(x); y=fabs(y);
  _x = x; _x*=_corr; _x+=_err;
  _y = y; _y*=_corr; _y+= _err;
  
  time=used_time();
  for (i=1;i<=repetitions/2;i++)
    {
      _z=sqrt(_y); _y=sqrt(_x); _x=sqrt(_z); 
      _z=sqrt(_y); _y=sqrt(_x); _x=sqrt(_z);
      _z=sqrt(_y); _y=sqrt(_x); _x=sqrt(_z);
      _z=sqrt(_z*_z);
    }
  
  _sqrttime+=used_time(time);
  time=used_time();
  for (i=1;i<=3*repetitions/2;i++)
    {
      z=sqrt(y); y=sqrt(x); x = sqrt(z);
      z=sqrt(y); y=sqrt(x); x = sqrt(z);
      z=sqrt(y); y=sqrt(x); x = sqrt(z);
      z=sqrt(z*z);
    }
  sqrttime+=used_time(time)/3;
  
  if (fp::is_infinite(z))
    error_handler(1,"error");
}
   


template <class filtered_number>
void runtime_filtered_number(int repetitions, int jbound, filtered_number X)
{
  
  X.set_range(-1,1);
  
  double j; 
  float addtime=0, subtime=0, multime=0, divtime=0, sqrttime=0,
    _addtime=0, _subtime=0, _multime=0, _divtime=0, _sqrttime=0;
  double x,y;
  cout << "j = ";
  for (j=1;j<=jbound;j++)
    {
      cout << j << " " << flush;
      
      x = sin(j*j-4)-cos(j*j+7); 
      y = sin(j*j-3)-cos(j*j+16); 
      
      runtime_filtered_add(X,repetitions,x,y,_addtime,addtime);
      runtime_filtered_sub(X,repetitions,x,y,_subtime,subtime);
      runtime_filtered_mul(X,repetitions,x,y,_multime,multime);
      runtime_filtered_div(X,repetitions,x,y,_divtime,divtime);
      y = 1/x;
      runtime_filtered_sqrt(X,repetitions,x,y,_sqrttime,sqrttime);
    }
  
  cout << "\n\naddition: " << "\n";
  cout << (100000*_addtime)/(repetitions*jbound);
  cout << ", " << _addtime/addtime << "\n\n";
  cout << "subtraction: " << "\n";
  cout << (100000*_subtime)/(repetitions*jbound);
  cout << ", " << _subtime/subtime<< "\n\n";
  cout << "multiplication: " << "\n";
  cout << (100000*_multime)/(repetitions*jbound);
  cout << ", " << _multime/multime << "\n\n";
  cout << "division: " << "\n";
  cout << (100000*_divtime)/(repetitions*jbound);
  cout << ", " << _divtime/divtime << "\n\n";
  cout << "square root: " << "\n";
  cout << (100000*_sqrttime)/(repetitions*jbound);
  cout << ", " << _sqrttime/sqrttime << "\n\n";
}



int main()
{

  cout << "\nStart program interval_test.\n";

#ifndef LEDA_NO_FPU
  interval_round_inside Iin;
  interval_round_outside Iout;
#endif
  interval_bound_relative Irel;
  interval_bound_absolute Iabs;

#ifndef LEDA_NO_FPU
  cout << "check interval_round_inside: ";
  check_filtered_number(Iin);
  cout << "Okay\n" << flush;
  leda_fpu::round_up();
  cout << "check interval_round_outside: ";
  check_filtered_number(Iout);
  leda_fpu::round_nearest();
  cout << "Okay\n" << flush;
#endif

// caution: the following two require on alpha the g++ flag -mieee
// (because 'infinity arithmetic' does not work otherwise)

  cout << "check interval_bound_relative: ";
  check_filtered_number(Irel);
  cout << "Okay\n" << flush;

  cout << "check interval_bound_absolute: ";
  check_filtered_number(Iabs);
  cout << "Okay\n" << flush;


  int repetitions;
  cout << "Wieviele Wiederholungen? ";
  cin >> repetitions;
  int jbound;
  cout << "Wieviele Testzahlen? ";
  cin >> jbound;
 
#ifndef LEDA_NO_FPU
  cout << "interval_round_inside: \n";
  runtime_filtered_number(repetitions,jbound,Iin);
  cout << "interval_round_outside: \n";
  runtime_filtered_number(repetitions,jbound,Iout);
#endif
  cout << "interval_bound_relative: \n";
  runtime_filtered_number(repetitions/10,jbound,Irel);
  cout << "interval_bound_absolute: \n";
  runtime_filtered_number(repetitions,jbound,Iabs);


  cout << "... all tests successfully completed!\n";

  return 0;
}

