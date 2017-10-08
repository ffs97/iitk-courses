/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  interval.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_INTERVAL_H
#define LEDA_INTERVAL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600260
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/numbers/fp.h>
#include <LEDA/numbers/interval_constants.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/real.h>
#include <LEDA/numbers/rational.h>

#ifndef LEDA_NO_FPU
#include <LEDA/numbers/fpu.h>
#endif

LEDA_BEGIN_NAMESPACE


#ifndef VOLATILE_I
#if defined(mips) || defined(i386)
#define VOLATILE_I volatile
#else
#define VOLATILE_I
#endif
#endif



#ifndef LEDA_NO_FPU

#ifndef INTERVAL_ROUND_INSIDE_H
#define INTERVAL_ROUND_INSIDE_H
 
#define interval_safe_mode
#undef INTERVAL
#define INTERVAL interval_round_inside

#undef interval_inline_addition
#undef interval_inline_multiplicaton 



#ifndef ROUND_H
#define ROUND_H

struct _round
{
  VOLATILE_I double INF;
  VOLATILE_I double SUP;

#ifndef interval_avoid_defaults
  _round(const _round& x) { INF = x.INF; SUP = x.SUP; }
  _round& operator=(const _round& x) 
  { INF = x.INF; SUP = x.SUP; return *this; }
#endif

  _round() {}
  _round(double inf, double sup) { INF = inf; SUP = sup; }
};

#endif



#undef IMP
#define IMP _round


#undef ADD_INLINE

#ifdef interval_inline_addition
#define ADD_INLINE inline
#else
#define ADD_INLINE 
#endif

#undef MUL_INLINE

#ifdef interval_inline_multiplication
#define MUL_INLINE inline
#else
#define MUL_INLINE 
#endif

#undef interval_avoid_defaults

#ifndef with_exactness_check
#define with_exactness_check
#endif

/*{\Manpage {interval} {} {Interval Arithmetic in LEDA} {x}}*/

class __exportC INTERVAL
{
/*{\Mdefinition
An instance of the data type |\Mtype| represents a real interval 
$I=[a,b]$. 
The basic interval operations $+,-,*,/,\sqrt{\phantom{x}}$ are available.
Type |\Mtype| can be used to approximate exact real arithmetic operations 
by inexact interval operations, as follows.
Each input number $x_i$ is converted into the interval $\{x_i\}$ 
and all real operations are replaced by interval operations.
If $x$ is the result of the exact real calculation and $I$ the 
interval computed by type |\Mtype|, it is guaranteed that $I$ contains $x$.
$I$ can be seen as a more or less accurate approximation of $x$.
In many cases the computed interval $I$ is small enough to provide 
a useful approximation of |x| and the {\em exact} sign of |x|.
There are four different implementations of |interval|s (consult the 
implementation section below for details):
\begin{itemize}
\item Class |interval_bound_absolute|
\item Class |interval_bound_relative|
\item Class |interval_round_inside|
\item Class |interval_round_outside|, which is usually the fastest but 
requires that the IEEE754 rounding mode |ieee_positive| is activated, 
e.g.~by using the LEDA class |fpu|.
\end{itemize}
The interface of all |interval| variants are identical.
However, note that the types |interval_round_inside| and 
|interval_round_outside| are only available on some explicitly 
supported UNIX platforms, currently including SPARC, MIPS, i386 
(PC's compatible to 80386 or higher), and ALPHA. 
For all platforms, the name |\Mtype| stands for 
the default implementation |interval_bound_absolute|.
}*/


protected:

  IMP I;

  INTERVAL(double x, double y) : I(x,y) {}
  
public:
  
/*{\Moptions ack=no }*/
/*{\Moptions warnings=no }*/

  inline INTERVAL();
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the interval $\{0\}$ }*/

  inline INTERVAL(VOLATILE_I double a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with $\{a\}$ }*/

  inline INTERVAL(int a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with $\{a\}$ }*/

  inline INTERVAL(long a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with $\{a\}$ }*/

  inline INTERVAL(unsigned int a);
  inline INTERVAL(unsigned long a);

  INTERVAL(const integer& a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the smallest possible interval containing |a| }*/

  INTERVAL(const bigfloat& a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the smallest possible interval containing |a| }*/

  INTERVAL(const real& a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the smallest possible interval containing |a| }*/

  INTERVAL(const rational& a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the smallest possible interval containing |a| }*/

  INTERVAL(const INTERVAL& x) { I = x.I; }
  INTERVAL& operator=(const INTERVAL& x) { I = x.I; return *this; }

/*{\Moperations 2 4.7 }*/

/*{\Mtext The arithmetic operations |+,-,*,/,sqrt,+=,-=,*=,/=| and the stream 
   operators are all available.
   {\bf Important: } 
   If the advanced implementation |interval_round_outside| is used, the user 
   has to guarantee that for each |interval| operation the IEEE754 rounding 
   mode ''towards $+\infty$'' is active. This can be achieved by calling the 
   function |fpu::round_up()|. To avoid side effects with library functions 
   that require the default IEEE754 rounding mode |to_nearest|, the function 
   |fpu::round_nearest()| can be used to reset the rounding mode. 
  }*/

  friend __exportF double to_double(const INTERVAL&);

  double to_double() const;
/*{\Mop returns the midpoint of the interval |\Mvar| as an approximation for 
        the exact real number represented by |\Mvar|. }*/

  double get_double_error() const;
/*{\Mop returns the diameter of the interval |\Mvar| which is the maximal 
        error of the approximation |\Mvar.to_double()| of the exact real 
        number represented by |\Mvar|. }*/

  inline bool is_a_point() const;
/*{\Mop returns true if and only if the interval |\Mvar| consists of a single 
        point. }*/

  inline bool is_finite() const;
/*{\Mop returns true if and only if the interval |\Mvar| is a finite 
        interval. }*/

  bool contains(double x) const;
/*{\Mop returns true if and only if the interval |\Mvar| contains the number |x|}*/

  inline double upper_bound() const;
/*{\Mop returns the upper bound of the interval |\Mvar|. }*/

  inline double lower_bound() const;
/*{\Mop returns the lower bound of the interval |\Mvar|. }*/

  void set_range(VOLATILE_I double x, VOLATILE_I double y);
/*{\Mop sets the current interval to $[x,y]$. }*/

  void set_midpoint(VOLATILE_I double num, VOLATILE_I double error);
/*{\Mop sets the current interval to a superset of $[num-error,num+error]$,
        i.e., to an interval with midpoint |num| and radius |error|. }*/

  inline bool sign_is_known() const;
/*{\Mop returns true if and only if all numbers in the interval |\Mvar| have 
        the same sign}*/

  inline int sign() const;
/*{\Mop returns the sign of all numbers in the interval |\Mvar| if this sign 
        is unique; aborts with an error message if |\Mvar.sign_is_known()| 
        gives false }*/

  friend __exportF int sign(const INTERVAL& x) { return x.sign(); }
  
friend ADD_INLINE __exportF INTERVAL operator+(const INTERVAL&,const INTERVAL&);
friend ADD_INLINE __exportF INTERVAL operator-(const INTERVAL&,const INTERVAL&);
friend MUL_INLINE __exportF INTERVAL operator*(const INTERVAL&,const INTERVAL&);
friend MUL_INLINE __exportF INTERVAL operator/(const INTERVAL&,const INTERVAL&);

  friend __exportF INTERVAL operator-(const INTERVAL&);
 
  inline void operator+=(const INTERVAL& y); 
  inline void operator-=(const INTERVAL& y); 
  void operator*=(const INTERVAL& y) { *this=*this*y; }
  void operator/=(const INTERVAL& y) { *this=*this/y; }

  friend __exportF INTERVAL sqrt(const INTERVAL& x);
  friend __exportF INTERVAL root(const INTERVAL& x, int d);
  friend __exportF INTERVAL  abs(const INTERVAL& x);

  friend __exportF ostream& operator<<(ostream& O, const INTERVAL& x);
  friend __exportF istream& operator>>(istream& I, INTERVAL&) { return I; }

  friend inline bool operator==(const INTERVAL& x, const INTERVAL& y)
  { return x.I.INF == y.I.INF && x.I.SUP == y.I.SUP; }

  friend inline bool operator!=(const INTERVAL& x, const INTERVAL& y)
  { return x.I.INF != y.I.INF || x.I.SUP != y.I.SUP; }

  inline void dummy();
  /*{\Mimplementation
    The types |interval_round_inside| and |interval_round_outside| represent 
    intervals directly by (the negative of) its lower bound and its upper 
    bound as |double|s. 
    Here all arithmetic operations require that the IEEE754 rounding mode 
    ''towards $+\infty$'' is active. For type |interval_round_inside| this 
    is done {\em inside} each operation, and for type |interval_round_outside| 
    the user has to do this manually ''from outside the operations'' by an 
    explicit call of |fpu::round_up()|. 

    The types |interval_bound_absolute| and |interval_bound_relative| represent
    intervals by their |double| midpoint |NUM| and diameter |ERROR|. The 
    interpretation is that |NUM| is the numerical approximation of a real 
    number and |ERROR| is a bound for the absolute, respectively relative 
    error of |NUM|. }*/
};


#ifndef inline_signtest
#define inline_signtest
#define IS_NEGATIVE(x) x<0.0
// fp::sign_bit_set(x)  does not help
#endif

#define interval_avoid_nan

inline void INTERVAL::dummy() {} 
// this seems to improve compiler optimization 
// on several compilers (I don't know why)

#ifdef interval_inline_addition
inline 
   INTERVAL operator+(const INTERVAL& x, const INTERVAL& y)
    { 
#ifdef interval_safe_mode
      leda_fpu::round_up();
      VOLATILE_I double inf=x.I.INF+y.I.INF;
      VOLATILE_I double sup=x.I.SUP+y.I.SUP;
      leda_fpu::round_nearest();
      return INTERVAL(inf,sup);
#else
      return INTERVAL(x.I.INF+y.I.INF,x.I.SUP+y.I.SUP);
#endif
    }
 
inline 
    INTERVAL operator-(const INTERVAL& x, const INTERVAL& y)
    { 
#ifdef interval_safe_mode
      leda_fpu::round_up();
      VOLATILE_I double inf = x.I.INF+y.I.SUP;
      VOLATILE_I double sup = x.I.SUP+y.I.INF;
      leda_fpu::round_nearest();
      return INTERVAL(inf,sup);
#else
      return INTERVAL(x.I.INF+y.I.SUP,x.I.SUP+y.I.INF);
#endif
    }

#endif
#ifdef interval_inline_multiplication
inline 
    INTERVAL operator*(const INTERVAL& x, const INTERVAL& y)
    {
      VOLATILE_I double inf, sup;
#ifdef interval_safe_mode
      leda_fpu::round_up();
#endif
      if (IS_NEGATIVE(x.I.INF)) {
	// x > 0
        if (IS_NEGATIVE(y.I.INF)) {
           // y > 0
           inf = (-x.I.INF)*y.I.INF;
           sup = x.I.SUP*y.I.SUP;
        }
        else {
          if (IS_NEGATIVE(y.I.SUP)) {
            // y <= 0
            inf = x.I.SUP*y.I.INF;
            sup = (-x.I.INF)*y.I.SUP;
          }
          else {
            inf = x.I.SUP*y.I.INF;
            sup = x.I.SUP*y.I.SUP;
          }  
        }
      }
      else {
	if (IS_NEGATIVE(x.I.SUP)) {
	  // x <= 0
          if (IS_NEGATIVE(y.I.INF)) {
	    // y > 0
	    inf = y.I.SUP*x.I.INF;
	    sup = (-y.I.INF)*x.I.SUP;
          }
	  else {
	    if (IS_NEGATIVE(y.I.SUP)) {
	      // y <= 0
	      inf = (-x.I.SUP)*y.I.SUP;
	      sup = x.I.INF*y.I.INF; 
	    }
	    else {
      	      inf = x.I.INF*y.I.SUP;
              sup = x.I.INF*y.I.INF;
            }
	  }
        }
        else {
	  // sign of x unclear
	  if (IS_NEGATIVE(y.I.INF)) {
	    // y > 0
	    inf = y.I.SUP*x.I.INF;
            sup = y.I.SUP*x.I.SUP;
	  }
	  else {
	    if (IS_NEGATIVE(y.I.SUP)) {
	      // y <= 0 
	      inf = y.I.INF*x.I.SUP;
	      sup = y.I.INF*x.I.INF;
	    }
	    else {
	      // signs of x and y both unkown
	      VOLATILE_I double tmp;
	      inf = x.I.SUP*y.I.INF;
	      tmp = x.I.INF*y.I.SUP;
	      if (tmp > inf)
	        inf = tmp;
  	      sup = x.I.SUP*y.I.SUP;
	      tmp = x.I.INF*y.I.INF;
	      if (tmp > sup)
	        sup = tmp;
	    }
	  }
	}
      }
#ifdef interval_safe_mode
      leda_fpu::round_nearest();
#endif
#ifdef interval_avoid_nan
      if (fp::is_infinite(inf+sup))
        inf=sup=fp::pInf;
#endif
      return INTERVAL(inf,sup);
    }

inline 
  INTERVAL operator/(const INTERVAL& x, const INTERVAL& y)
    { 
      VOLATILE_I double inf, sup;
#ifdef interval_safe_mode
      leda_fpu::round_up();
#endif
      if (IS_NEGATIVE(y.I.INF)) {
	 // y > 0
	 if (IS_NEGATIVE(x.I.INF)) {
	    // x > 0
	    inf = x.I.INF/y.I.SUP;
	    sup = x.I.SUP/(-y.I.INF);
	 }
	 else {
	   if (IS_NEGATIVE(x.I.SUP)) {
	     // x < 0
	     inf = x.I.INF/(-y.I.INF);
	     sup = x.I.SUP/y.I.SUP;
	   }
	   else {
	     // sign of x unclear
	     inf = (-x.I.INF)/y.I.INF;
	     sup = (-x.I.SUP)/y.I.INF;
	   }
	 }
       }
       else {
	 if (IS_NEGATIVE(y.I.SUP)) {
	   // y < 0
	   if (IS_NEGATIVE(x.I.INF)) {
	     // x > 0
	     inf = (-x.I.SUP)/y.I.SUP;
	     sup = x.I.INF/y.I.INF;
	   }
	   else {
	     if (IS_NEGATIVE(x.I.SUP)) {
	       // x < 0
	       inf = x.I.SUP/y.I.INF;
	       sup = (-x.I.INF)/y.I.SUP;
	     }
	     else {
	       // sign of x is unclear
	       inf = (-x.I.SUP)/y.I.SUP;
	       sup = (-x.I.INF)/y.I.SUP;
	     }
	   }
         }
	 else {
	   // sign of y unclear
	   inf = sup = fp::pInf;
	 }
      }
#ifdef interval_avoid_nan
      if (fp::is_infinite(inf+sup))
	inf = sup = fp::pInf;
#endif
#ifdef interval_safe_mode
      leda_fpu::round_nearest();
#endif
      return INTERVAL(inf,sup);
    } 

#endif


inline INTERVAL::INTERVAL()
{
  I.SUP = I.INF = 0;
}

inline INTERVAL::INTERVAL(VOLATILE_I double x)
{
  if (fp::is_finite(x))
    {
      I.SUP = 0+x;
      I.INF = 0-x;
    }
  else
    I.SUP = I.INF = fp::pInf;
}

inline INTERVAL::INTERVAL(int x)
{
  I.SUP = x; I.INF = 0-x;
}

inline INTERVAL::INTERVAL(long x)
{
  I.SUP = x; I.INF = 0-x;
}

inline INTERVAL::INTERVAL(unsigned int x)
{
  I.SUP = x; I.INF = 0-x;
}

inline INTERVAL::INTERVAL(unsigned long x)
{
  I.SUP = x; I.INF = 0-x;
}


inline bool INTERVAL::is_finite() const {
  return (fp::is_finite(I.INF) && fp::is_finite(I.SUP));
}

inline bool INTERVAL::is_a_point() const {
  return (-I.INF == I.SUP);
}


inline double INTERVAL::upper_bound() const
{
  return I.SUP;
}

inline double INTERVAL::lower_bound() const
{
  return -I.INF;
}



inline void INTERVAL::operator+=(const INTERVAL& y) 
{
#ifdef interval_safe_mode
  leda_fpu::round_up();
#endif
  I.INF+=y.I.INF;
  I.SUP+=y.I.SUP;
#ifdef interval_safe_mode
  leda_fpu::round_nearest();
#endif
}
inline void INTERVAL::operator-=(const INTERVAL& y) 
{
#ifdef interval_safe_mode
  leda_fpu::round_up();
#endif
  I.INF+=y.I.SUP;
  I.SUP+=y.I.INF;
#ifdef interval_safe_mode
  leda_fpu::round_nearest();
#endif
}



inline bool INTERVAL::sign_is_known() const
{
  if (I.INF < 0.0 || I.SUP < 0.0)
    return true;
  if (-I.INF==I.SUP)
    return true;
  return false;
}

inline int INTERVAL::sign() const
{  
  if (sign_is_known())
    return fp::sign(I.SUP);
  else
    LEDA_EXCEPTION(1,"error: sign cannot be determined");
  return 0;
}

#endif


#ifndef INTERVAL_ROUND_OUTSIDE_H
#define INTERVAL_ROUND_OUTSIDE_H

#undef interval_safe_mode
#undef INTERVAL
#define INTERVAL interval_round_outside

#define interval_inline_multiplication
#define interval_inline_addition



#ifndef ROUND_H
#define ROUND_H

struct _round
{
  VOLATILE_I double INF;
  VOLATILE_I double SUP;

#ifndef interval_avoid_defaults
  _round(const _round& x) { INF = x.INF; SUP = x.SUP; }
  _round& operator=(const _round& x) 
  { INF = x.INF; SUP = x.SUP; return *this; }
#endif

  _round() {}
  _round(double inf, double sup) { INF = inf; SUP = sup; }
};

#endif



#undef IMP
#define IMP _round


#undef ADD_INLINE
#ifdef interval_inline_addition
#define ADD_INLINE inline
#else
#define ADD_INLINE 
#endif

#undef MUL_INLINE
#ifdef interval_inline_multiplication
#define MUL_INLINE inline
#else
#define MUL_INLINE 
#endif

#undef interval_avoid_defaults
#ifndef with_exactness_check
#define with_exactness_check
#endif

/*{\Xanpage {interval} {} {Interval Arithmetic in LEDA} {x}}*/

class __exportC INTERVAL
{
/*{\Xdefinition
An instance of the data type |\Xtype| represents a real interval 
$I=[a,b]$. 
The basic interval operations $+,-,*,/,\sqrt{\phantom{x}}$ are available.
Type |\Xtype| can be used to approximate exact real arithmetic operations 
by inexact interval operations, as follows.
Each input number $x_i$ is converted into the interval $\{x_i\}$ 
and all real operations are replaced by interval operations.
If $x$ is the result of the exact real calculation and $I$ the 
interval computed by type |\Xtype|, it is guaranteed that $I$ contains $x$.
$I$ can be seen as a more or less accurate approximation of $x$.
In many cases the computed interval $I$ is small enough to provide 
a useful approximation of |x| and the {\em exact} sign of |x|.
There are four different implementations of |interval|s:
\begin{itemize}
\item Class |interval_round_inside|, which is the default implementation 
that can be accessed by the name |interval|
\item Classes |interval_bound_absolute| and |interval_bound_relative|
\item Class |interval_round_outside| which is usually the fastest but 
requires that the IEEE754 rounding mode |ieee_positive| is activated, 
e.g. by using the LEDA class |fpu|.
\end{itemize}
The interface of all |interval| variants are identical.
However, note that the types |interval_round_inside| and 
|interval_round_outside| are only available on some explicitly 
supported UNIX platforms, currently including SPARC, MIPS, i386 
(PC's compatible to 80386 or higher), and ALPHA. 
For all platforms, the name |\Xtype| stands for 
the default implementation |interval_bound_absolute|.
}*/


protected:

  IMP I;

  INTERVAL(double x, double y) : I(x,y) {}
  
public:
  
/*{\Xoptions ack=no }*/
/*{\Xoptions warnings=no }*/

  inline INTERVAL();
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the interval $\{0\}$ }*/

  inline INTERVAL(VOLATILE_I double a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with $\{a\}$ }*/

  inline INTERVAL(int a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with $\{a\}$ }*/

  inline INTERVAL(long a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with $\{a\}$ }*/

  inline INTERVAL(unsigned int a);

  inline INTERVAL(unsigned long a);

  INTERVAL(const integer& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/

  INTERVAL(const bigfloat& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/

  INTERVAL(const real& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/

  INTERVAL(const rational& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/


  INTERVAL(const INTERVAL& x) { I = x.I; }
  INTERVAL& operator=(const INTERVAL& x) { I = x.I; return *this; }

/*{\Xoperations 2 4.7 }*/
/*{\Xtext The arithmetic operations |+,-,*,/,sqrt,+=,-=,*=,/=| and the stream 
   operators are all available.
   {\bf Important: } 
   If the advanced implementation |interval_round_outside| is used, the user has to guarantee that for each |interval| operation the IEEE754 rounding mode ''towards $+\infty$'' is active. This can be achieved by calling the function |fpu::round_up()|. To avoid side effects with library functions that require the default IEEE754 rounding mode |to_nearest|, the function |fpu::round_nearest()| can be used to reset the rounding mode. 
   }*/
  friend __exportF double to_double(const INTERVAL&);
  double to_double() const;
/*{\Xop returns the midpoint of the interval |\Xvar| as an approximation for the exact real number represented by |\Xvar|. }*/
  double get_double_error() const;
/*{\Xop returns the diameter of the interval |\Xvar| which is the maximal error of the approximation |\Xvar.to_double()| of the exact real number represented by |\Xvar|. }*/
  inline bool is_a_point() const;
/*{\Xop returns true if and only if the interval |\Xvar| consists of a single 
point. }*/
  inline bool is_finite() const;
/*{\Xop returns true if and only if the interval |\Xvar| is a finite interval. }*/
  bool contains(double x) const;
/*{\Xop returns true if and only if the interval |\Xvar| contains the number |x|}*/
  inline double upper_bound() const;
/*{\Xop returns the upper bound of the interval |\Xvar|. }*/
  inline double lower_bound() const;
/*{\Xop returns the lower bound of the interval |\Xvar|. }*/
  void set_range(VOLATILE_I double x, VOLATILE_I double y);
/*{\Xop sets the current interval to $[x,y]$. }*/
  void set_midpoint(VOLATILE_I double num, VOLATILE_I double error);
/*{\Xop sets the current interval to a superset of $[num-error,num+error]$,
    i.e., to an interval with midpoint |num| and radius |error|. }*/
  inline bool sign_is_known() const;
/*{\Xop returns true if and only if all numbers in the interval |\Xvar| have the same sign}*/
  inline int sign() const;
/*{\Xop returns the sign of all numbers in the interval |\Xvar| if this sign is unique; aborts with an error message if |\Xvar.sign_is_known()| gives false }*/
  friend __exportF int sign(const INTERVAL& x) { return x.sign(); }
  
friend ADD_INLINE __exportF INTERVAL operator+(const INTERVAL&,const INTERVAL&);
friend ADD_INLINE __exportF INTERVAL operator-(const INTERVAL&,const INTERVAL&);
friend MUL_INLINE __exportF INTERVAL operator*(const INTERVAL&,const INTERVAL&);
friend MUL_INLINE __exportF INTERVAL operator/(const INTERVAL&,const INTERVAL&);

  friend __exportF INTERVAL operator-(const INTERVAL&);
 
  inline void operator+=(const INTERVAL& y); 
  inline void operator-=(const INTERVAL& y); 
  void operator*=(const INTERVAL& y) { *this=*this*y; }
  void operator/=(const INTERVAL& y) { *this=*this/y; }

  friend __exportF INTERVAL sqrt(const INTERVAL& x);
  friend __exportF INTERVAL root(const INTERVAL& x, int d);
  friend __exportF INTERVAL  abs(const INTERVAL& x);

  friend __exportF ostream& operator<<(ostream& O, const INTERVAL& x);
  friend __exportF istream& operator>>(istream& I, INTERVAL&) { return I; }

  friend inline bool operator==(const INTERVAL& x, const INTERVAL& y)
  { return x.I.INF == y.I.INF && x.I.SUP == y.I.SUP; }

  friend inline bool operator!=(const INTERVAL& x, const INTERVAL& y)
  { return x.I.INF != y.I.INF || x.I.SUP != y.I.SUP; }

  inline void dummy();
  /*{\Ximplementation
    The types |interval_round_inside| and |interval_round_outside| represent intervals directly by (the negative of) its lower bound and its upper bound as |double|s. 
Here all arithmetic operations require that the IEEE754 rounding mode ''towards $+\infty$'' is active. For type |interval_round_inside| this is done {\em inside} each operation, and for type |interval_round_outside| the user has to do this manually ''from outside the operations'' by an explicit call of |fpu::round_up()|. 

The types |interval_bound_absolute| and |interval_bound_relative| represent intervals by their |double| midpoint |NUM| and diameter |ERROR|. The interpretation is that |NUM| is the numerical approximation of a real number and |ERROR| is a bound for the absolute, respectively relative error of |NUM|.

}*/
};


#ifndef inline_signtest
#define inline_signtest
#define IS_NEGATIVE(x) x<0.0
// define IS_NEGATIVE(x) fp::sign_bit_set(x) 
// this does not help
#endif

#define interval_avoid_nan

inline void INTERVAL::dummy() {} 
// this seems to improve compiler optimization 
// on several compilers (I don't know why)

#ifdef interval_inline_addition
inline 
   INTERVAL operator+(const INTERVAL& x, const INTERVAL& y)
    { 
#ifdef interval_safe_mode
      leda_fpu::round_up();
      VOLATILE_I double inf=x.I.INF+y.I.INF;
      VOLATILE_I double sup=x.I.SUP+y.I.SUP;
      leda_fpu::round_nearest();
      return INTERVAL(inf,sup);
#else
      return INTERVAL(x.I.INF+y.I.INF,x.I.SUP+y.I.SUP);
#endif
    }
 
inline 
    INTERVAL operator-(const INTERVAL& x, const INTERVAL& y)
    { 
#ifdef interval_safe_mode
      leda_fpu::round_up();
      VOLATILE_I double inf = x.I.INF+y.I.SUP;
      VOLATILE_I double sup = x.I.SUP+y.I.INF;
      leda_fpu::round_nearest();
      return INTERVAL(inf,sup);
#else
      return INTERVAL(x.I.INF+y.I.SUP,x.I.SUP+y.I.INF);
#endif
    }

#endif
#ifdef interval_inline_multiplication
inline 
    INTERVAL operator*(const INTERVAL& x, const INTERVAL& y)
    {
      VOLATILE_I double inf, sup;
#ifdef interval_safe_mode
      leda_fpu::round_up();
#endif
      if (IS_NEGATIVE(x.I.INF)) {
	// x > 0
        if (IS_NEGATIVE(y.I.INF)) {
           // y > 0
           inf = (-x.I.INF)*y.I.INF;
           sup = x.I.SUP*y.I.SUP;
        }
        else {
          if (IS_NEGATIVE(y.I.SUP)) {
            // y <= 0
            inf = x.I.SUP*y.I.INF;
            sup = (-x.I.INF)*y.I.SUP;
          }
          else {
            inf = x.I.SUP*y.I.INF;
            sup = x.I.SUP*y.I.SUP;
          }  
        }
      }
      else {
	if (IS_NEGATIVE(x.I.SUP)) {
	  // x <= 0
          if (IS_NEGATIVE(y.I.INF)) {
	    // y > 0
	    inf = y.I.SUP*x.I.INF;
	    sup = (-y.I.INF)*x.I.SUP;
          }
	  else {
	    if (IS_NEGATIVE(y.I.SUP)) {
	      // y <= 0
	      inf = (-x.I.SUP)*y.I.SUP;
	      sup = x.I.INF*y.I.INF; 
	    }
	    else {
      	      inf = x.I.INF*y.I.SUP;
              sup = x.I.INF*y.I.INF;
            }
	  }
        }
        else {
	  // sign of x unclear
	  if (IS_NEGATIVE(y.I.INF)) {
	    // y > 0
	    inf = y.I.SUP*x.I.INF;
            sup = y.I.SUP*x.I.SUP;
	  }
	  else {
	    if (IS_NEGATIVE(y.I.SUP)) {
	      // y <= 0 
	      inf = y.I.INF*x.I.SUP;
	      sup = y.I.INF*x.I.INF;
	    }
	    else {
	      // signs of x and y both unkown
	      VOLATILE_I double tmp;
	      inf = x.I.SUP*y.I.INF;
	      tmp = x.I.INF*y.I.SUP;
	      if (tmp > inf)
	        inf = tmp;
  	      sup = x.I.SUP*y.I.SUP;
	      tmp = x.I.INF*y.I.INF;
	      if (tmp > sup)
	        sup = tmp;
	    }
	  }
	}
      }
#ifdef interval_safe_mode
      leda_fpu::round_nearest();
#endif
#ifdef interval_avoid_nan
      if (fp::is_infinite(inf+sup))
        inf=sup=fp::pInf;
#endif
      return INTERVAL(inf,sup);
    }

inline 
  INTERVAL operator/(const INTERVAL& x, const INTERVAL& y)
    { 
      VOLATILE_I double inf, sup;
#ifdef interval_safe_mode
      leda_fpu::round_up();
#endif
      if (IS_NEGATIVE(y.I.INF)) {
	 // y > 0
	 if (IS_NEGATIVE(x.I.INF)) {
	    // x > 0
	    inf = x.I.INF/y.I.SUP;
	    sup = x.I.SUP/(-y.I.INF);
	 }
	 else {
	   if (IS_NEGATIVE(x.I.SUP)) {
	     // x < 0
	     inf = x.I.INF/(-y.I.INF);
	     sup = x.I.SUP/y.I.SUP;
	   }
	   else {
	     // sign of x unclear
	     inf = (-x.I.INF)/y.I.INF;
	     sup = (-x.I.SUP)/y.I.INF;
	   }
	 }
       }
       else {
	 if (IS_NEGATIVE(y.I.SUP)) {
	   // y < 0
	   if (IS_NEGATIVE(x.I.INF)) {
	     // x > 0
	     inf = (-x.I.SUP)/y.I.SUP;
	     sup = x.I.INF/y.I.INF;
	   }
	   else {
	     if (IS_NEGATIVE(x.I.SUP)) {
	       // x < 0
	       inf = x.I.SUP/y.I.INF;
	       sup = (-x.I.INF)/y.I.SUP;
	     }
	     else {
	       // sign of x is unclear
	       inf = (-x.I.SUP)/y.I.SUP;
	       sup = (-x.I.INF)/y.I.SUP;
	     }
	   }
         }
	 else {
	   // sign of y unclear
	   inf = sup = fp::pInf;
	 }
      }
#ifdef interval_avoid_nan
      if (fp::is_infinite(inf+sup))
	inf = sup = fp::pInf;
#endif
#ifdef interval_safe_mode
      leda_fpu::round_nearest();
#endif
      return INTERVAL(inf,sup);
    } 

#endif


inline INTERVAL::INTERVAL()
{
  I.SUP = I.INF = 0;
}

inline INTERVAL::INTERVAL(VOLATILE_I double x)
{
  if (fp::is_finite(x))
    {
      I.SUP = 0+x;
      I.INF = 0-x;
    }
  else
    I.SUP = I.INF = fp::pInf;
}

inline INTERVAL::INTERVAL(int x)
{
  I.SUP = x; I.INF = 0-x;
}

inline INTERVAL::INTERVAL(long x)
{
  I.SUP = x; I.INF = 0-x;
}

inline INTERVAL::INTERVAL(unsigned int x)
{
  I.SUP = x; I.INF = 0-x;
}

inline INTERVAL::INTERVAL(unsigned long x)
{
  I.SUP = x; I.INF = 0-x;
}


inline bool INTERVAL::is_finite() const {
  return (fp::is_finite(I.INF) && fp::is_finite(I.SUP));
}

inline bool INTERVAL::is_a_point() const {
  return (-I.INF == I.SUP);
}


inline double INTERVAL::upper_bound() const
{
  return I.SUP;
}

inline double INTERVAL::lower_bound() const
{
  return -I.INF;
}



inline void INTERVAL::operator+=(const INTERVAL& y) 
{
#ifdef interval_safe_mode
  leda_fpu::round_up();
#endif
  I.INF+=y.I.INF;
  I.SUP+=y.I.SUP;
#ifdef interval_safe_mode
  leda_fpu::round_nearest();
#endif
}
inline void INTERVAL::operator-=(const INTERVAL& y) 
{
#ifdef interval_safe_mode
  leda_fpu::round_up();
#endif
  I.INF+=y.I.SUP;
  I.SUP+=y.I.INF;
#ifdef interval_safe_mode
  leda_fpu::round_nearest();
#endif
}



inline bool INTERVAL::sign_is_known() const
{
  if (I.INF < 0.0 || I.SUP < 0.0)
    return true;
  if (-I.INF==I.SUP)
    return true;
  return false;
}

inline int INTERVAL::sign() const
{  
  if (sign_is_known())
    return fp::sign(I.SUP);
  else
    LEDA_EXCEPTION(1,"error: sign cannot be determined");
  return 0;
}



#endif

#endif




#ifndef INTERVAL_BOUND_ABSOLUTE_H
#define INTERVAL_BOUND_ABSOLUTE_H

#define interval_inline_addition
#define interval_inline_multiplication
 
#undef INTERVAL
#define INTERVAL interval_bound_absolute

#undef interval_relative_bound



#ifndef BOUND_H
#define BOUND_H

struct _bound
{
  VOLATILE_I double NUM;
  VOLATILE_I double ERROR;

#ifndef interval_avoid_defaults
  _bound(const _bound& x) { NUM = x.NUM; ERROR = x.ERROR; }
  _bound& operator=(const _bound& x) 
  { NUM = x.NUM; ERROR = x.ERROR; return *this; }
#endif

  _bound() {}
  _bound(double num, double error) { NUM = num; ERROR = error; }
};

#endif


#undef IMP
#define IMP _bound


#undef ADD_INLINE
#ifdef interval_inline_addition
#define ADD_INLINE inline
#else
#define ADD_INLINE 
#endif

#undef MUL_INLINE
#ifdef interval_inline_multiplication
#define MUL_INLINE inline
#else
#define MUL_INLINE 
#endif

#undef interval_avoid_defaults
#ifndef with_exactness_check
#define with_exactness_check
#endif

/*{\Xanpage {interval} {} {Interval Arithmetic in LEDA} {x}}*/

class __exportC INTERVAL
{
/*{\Xdefinition
An instance of the data type |\Xtype| represents a real interval 
$I=[a,b]$. 
The basic interval operations $+,-,*,/,\sqrt{\phantom{x}}$ are available.
Type |\Xtype| can be used to approximate exact real arithmetic operations 
by inexact interval operations, as follows.
Each input number $x_i$ is converted into the interval $\{x_i\}$ 
and all real operations are replaced by interval operations.
If $x$ is the result of the exact real calculation and $I$ the 
interval computed by type |\Xtype|, it is guaranteed that $I$ contains $x$.
$I$ can be seen as a more or less accurate approximation of $x$.
In many cases the computed interval $I$ is small enough to provide 
a useful approximation of |x| and the {\em exact} sign of |x|.
There are four different implementations of |interval|s:
\begin{itemize}
\item Class |interval_round_inside|, which is the default implementation 
that can be accessed by the name |interval|
\item Classes |interval_bound_absolute| and |interval_bound_relative|
\item Class |interval_round_outside| which is usually the fastest but 
requires that the IEEE754 rounding mode |ieee_positive| is activated, 
e.g. by using the LEDA class |fpu|.
\end{itemize}
The interface of all |interval| variants are identical.
However, note that the types |interval_round_inside| and 
|interval_round_outside| are only available on some explicitly 
supported UNIX platforms, currently including SPARC, MIPS, i386 
(PC's compatible to 80386 or higher), and ALPHA. 
For all platforms, the name |\Xtype| stands for 
the default implementation |interval_bound_absolute|.
}*/


protected:

  IMP I;

  INTERVAL(double x, double y) : I(x,y) {}
  
public:
  
/*{\Xoptions ack=no }*/
/*{\Xoptions warnings=no }*/
  inline INTERVAL();
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the interval $\{0\}$ }*/
  inline INTERVAL(VOLATILE_I double a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with $\{a\}$ }*/
  inline INTERVAL(int a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with $\{a\}$ }*/
  inline INTERVAL(long a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with $\{a\}$ }*/
  inline INTERVAL(unsigned int a);
  inline INTERVAL(unsigned long a);

  INTERVAL(const integer& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/

  INTERVAL(const bigfloat& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/

  INTERVAL(const real& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/

  INTERVAL(const rational& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
with the smallest possible interval containing |a| }*/


  INTERVAL(const INTERVAL& x) { I = x.I; }

  INTERVAL& operator=(const INTERVAL& x) { I = x.I; return *this; }

/*{\Xoperations 2 4.7 }*/
/*{\Xtext The arithmetic operations |+,-,*,/,sqrt,+=,-=,*=,/=| and the stream 
   operators are all available.
   {\bf Important: } 
   If the advanced implementation |interval_round_outside| is used, the user has to guarantee that for each |interval| operation the IEEE754 rounding mode ''towards $+\infty$'' is active. This can be achieved by calling the function |fpu::round_up()|. To avoid side effects with library functions that require the default IEEE754 rounding mode |to_nearest|, the function |fpu::round_nearest()| can be used to reset the rounding mode. 
   }*/
  friend __exportF double to_double(const INTERVAL&);
  double to_double() const;
/*{\Xop returns the midpoint of the interval |\Xvar| as an approximation for the exact real number represented by |\Xvar|. }*/
  double get_double_error() const;
/*{\Xop returns the diameter of the interval |\Xvar| which is the maximal error of the approximation |\Xvar.to_double()| of the exact real number represented by |\Xvar|. }*/
  inline bool is_a_point() const;
/*{\Xop returns true if and only if the interval |\Xvar| consists of a single 
point. }*/
  inline bool is_finite() const;
/*{\Xop returns true if and only if the interval |\Xvar| is a finite interval. }*/
  bool contains(double x) const;
/*{\Xop returns true if and only if the interval |\Xvar| contains the number |x|}*/
  inline double upper_bound() const;
/*{\Xop returns the upper bound of the interval |\Xvar|. }*/
  inline double lower_bound() const;
/*{\Xop returns the lower bound of the interval |\Xvar|. }*/
  void set_range(VOLATILE_I double x, VOLATILE_I double y);
/*{\Xop sets the current interval to $[x,y]$. }*/
  void set_midpoint(VOLATILE_I double num, VOLATILE_I double error);
/*{\Xop sets the current interval to a superset of $[num-error,num+error]$,
    i.e., to an interval with midpoint |num| and radius |error|. }*/
  inline bool sign_is_known() const;
/*{\Xop returns true if and only if all numbers in the interval |\Xvar| have the same sign}*/
  inline int sign() const;
/*{\Xop returns the sign of all numbers in the interval |\Xvar| if this sign is unique; aborts with an error message if |\Xvar.sign_is_known()| gives false }*/
  friend __exportF int sign(const INTERVAL& x) { return x.sign(); }
  
  friend ADD_INLINE __exportF INTERVAL operator+(const INTERVAL&,const INTERVAL&);
  friend ADD_INLINE __exportF INTERVAL operator-(const INTERVAL&,const INTERVAL&);
  friend MUL_INLINE __exportF INTERVAL operator*(const INTERVAL&,const INTERVAL&);
  friend MUL_INLINE __exportF INTERVAL operator/(const INTERVAL&,const INTERVAL&);

  friend __exportF INTERVAL operator-(const INTERVAL&);
 
  inline void operator+=(const INTERVAL& y); 
  inline void operator-=(const INTERVAL& y); 
  void operator*=(const INTERVAL& y) { *this=*this*y; }
  void operator/=(const INTERVAL& y) { *this=*this/y; }

  friend __exportF INTERVAL sqrt(const INTERVAL& x);
  friend __exportF INTERVAL root(const INTERVAL& x, int d);
  friend __exportF INTERVAL  abs(const INTERVAL& x);

  friend __exportF ostream& operator<<(ostream& O, const INTERVAL& x);
  friend __exportF istream& operator>>(istream& I, INTERVAL&) { return I; }

  friend inline bool operator==(const INTERVAL& x, const INTERVAL& y)
  { return x.I.NUM == y.I.NUM && x.I.ERROR == y.I.ERROR; }

  friend inline bool operator!=(const INTERVAL& x, const INTERVAL& y)
  { return x.I.NUM != y.I.NUM || x.I.ERROR != y.I.ERROR; }

  inline void dummy();
  /*{\Ximplementation
    The types |interval_round_inside| and |interval_round_outside| represent intervals directly by (the negative of) its lower bound and its upper bound as |double|s. 
Here all arithmetic operations require that the IEEE754 rounding mode ''towards $+\infty$'' is active. For type |interval_round_inside| this is done {\em inside} each operation, and for type |interval_round_outside| the user has to do this manually ''from outside the operations'' by an explicit call of |fpu::round_up()|. 

The types |interval_bound_absolute| and |interval_bound_relative| represent intervals by their |double| midpoint |NUM| and diameter |ERROR|. The interpretation is that |NUM| is the numerical approximation of a real number and |ERROR| is a bound for the absolute, respectively relative error of |NUM|.

More details can be found in the full documentation of type |interval| and its variants.
}*/
};


inline void INTERVAL::dummy() { }

LEDA_END_NAMESPACE


LEDA_BEGIN_NAMESPACE

#ifdef interval_inline_addition
inline 
   INTERVAL operator+(const INTERVAL& x,const INTERVAL& y)
    { 
      VOLATILE_I double num = x.I.NUM + y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
{  
  if (num-x.I.NUM == y.I.NUM && num-y.I.NUM == x.I.NUM)
    return num;
}

#endif	
      
#ifdef interval_relative_bound
  if (fp::is_not_denormal(num)) 
   { 
     VOLATILE_I double term_x = x.I.NUM;
     fp::clear_sign_bit(term_x);
     VOLATILE_I double term_y = y.I.NUM;
     fp::clear_sign_bit(term_y);
     VOLATILE_I double term_z = num;
     fp::clear_sign_bit(term_z);
     error = (term_x*x.I.ERROR+term_y*y.I.ERROR+twoMinDbl)/term_z + eps;
   }
  else
   {  
     num =  fp::abs(x.I.NUM)*x.I.ERROR + fp::abs(y.I.NUM)*y.I.ERROR 
       + 4*MinDbl;
     error = 2;
   }
  error *= roundup_factor;
  if (fp::is_infinite(error)) // || (error-error)!=0)
    error = fp::pInf;
#else
  VOLATILE_I double term_z = num; 
  fp::clear_sign_bit(term_z);
  error = x.I.ERROR + y.I.ERROR + term_z*eps + threeMinDbl;
  error *= roundup_factor;
#endif

      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
    }

inline 
   INTERVAL operator-(const INTERVAL& x,const INTERVAL& y)
    { 
      VOLATILE_I double num = x.I.NUM - y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
{  
  if (num+y.I.NUM == x.I.NUM && x.I.NUM-num == y.I.NUM)
    return num;
}


#endif
      
#ifdef interval_relative_bound
  if (fp::is_not_denormal(num)) 
   { 
     VOLATILE_I double term_x = x.I.NUM;
     fp::clear_sign_bit(term_x);
     VOLATILE_I double term_y = y.I.NUM;
     fp::clear_sign_bit(term_y);
     VOLATILE_I double term_z = num;
     fp::clear_sign_bit(term_z);
     error = (term_x*x.I.ERROR+term_y*y.I.ERROR+twoMinDbl)/term_z + eps;
   }
  else
   {  
     num =  fp::abs(x.I.NUM)*x.I.ERROR + fp::abs(y.I.NUM)*y.I.ERROR 
       + 4*MinDbl;
     error = 2;
   }
  error *= roundup_factor;
  if (fp::is_infinite(error)) // || (error-error)!=0)
    error = fp::pInf;
#else
  VOLATILE_I double term_z = num; 
  fp::clear_sign_bit(term_z);
  error = x.I.ERROR + y.I.ERROR + term_z*eps + threeMinDbl;
  error *= roundup_factor;
#endif

      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
    }

#endif
#ifdef interval_inline_multiplication
inline 
   INTERVAL operator*(const INTERVAL& x,const INTERVAL& y)
    {
      VOLATILE_I double num = x.I.NUM * y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
{  
  if (fp::is_single_precision(x.I.NUM) && fp::is_single_precision(y.I.NUM))
    {  
      num = x.I.NUM * y.I.NUM;
      if (fp::is_finite(num) && fp::is_not_denormal(num))
	return num;
    }
}


#endif
      
#ifdef interval_relative_bound
  if (fp::is_not_denormal(num)) 
  { 
    error = x.I.ERROR + y.I.ERROR + x.I.ERROR*y.I.ERROR + eps;
  }
  else
  { 
    if ((x.I.NUM != 0) && (y.I.NUM != 0))
      { 
	num = MinDbl*(1 + x.I.ERROR)*(1 + y.I.ERROR);
	error = 2;
      }
    else 
      return INTERVAL(0,0);
  }
  error *= roundup_factor;
#else
  VOLATILE_I double term_x = x.I.NUM;
  fp::clear_sign_bit(term_x);
  VOLATILE_I double term_y = y.I.NUM;
  fp::clear_sign_bit(term_y);
  VOLATILE_I double term_z = num;
  fp::clear_sign_bit(term_z);
  error = x.I.ERROR*term_y+y.I.ERROR*(term_x+x.I.ERROR)+term_z*eps+threeMinDbl;
  error *= roundup_factor;
#endif
  if (fp::is_infinite(error)) //  || (error-error)!=0)
    error = fp::pInf;


   
      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
    }

inline 
  INTERVAL operator/(const INTERVAL& x,const INTERVAL& y)
    { 
      VOLATILE_I double num;
      if (y.I.NUM == 0)
	num = 1;
      else
	num = x.I.NUM/y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
  if (fp::is_single_precision(y.I.NUM))
{  
  if (fp::is_single_precision(num) && num*y.I.NUM == x.I.NUM)
    if (fp::is_finite(num) && fp::is_not_denormal(num))
      return num;
}

#endif
      
#ifdef interval_relative_bound
  if (y.I.ERROR >= 1 || y.I.NUM == 0)
    error = fp::pInf; 
  else 
  {
    if (fp::is_not_denormal(num))
      error = (x.I.ERROR + y.I.ERROR) / (1 - y.I.ERROR) + eps;
    else
      { 
	if (x.I.NUM != 0)
	  { 
	    num = MinDbl*(1 + x.I.ERROR)/(1 - y.I.ERROR);
	    error = 2;
	  }
	else 
	  return 0;
      }
  }
#else
  VOLATILE_I double term_x = x.I.NUM;
  fp::clear_sign_bit(term_x);
  VOLATILE_I double term_y = y.I.NUM;
  fp::clear_sign_bit(term_y);
  VOLATILE_I double term_z = num;
  fp::clear_sign_bit(term_z);
  VOLATILE_I double y_low = term_y-y.I.ERROR;
  if (y_low > 0)
  {
    error = ((y.I.ERROR/term_y)*term_x+x.I.ERROR+MinDbl)/y_low + term_z*eps;
    error += twoMinDbl;
  }
  else
    error = fp::pInf;
#endif
error *= roundup_factor; 

      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
   }
 

#endif


inline INTERVAL::INTERVAL()
{
  I.NUM = I.ERROR = 0;
}

inline INTERVAL::INTERVAL(VOLATILE_I double x)
{
  I.NUM = x; 
  if (fp::is_finite(x))
    I.ERROR = 0;
  else
    {
      I.NUM = 1;
      I.ERROR = fp::pInf;
    }
}

inline INTERVAL::INTERVAL(int x)
{
  I.NUM = x; 
  I.ERROR = 0;
}

inline INTERVAL::INTERVAL(long x)
{
  I.NUM = x; 
  I.ERROR = 0;
}

inline INTERVAL::INTERVAL(unsigned int x)
{
  I.NUM = x; 
  I.ERROR = 0;
}

inline INTERVAL::INTERVAL(unsigned long x)
{
  I.NUM = x; 
  I.ERROR = 0;
}


inline bool INTERVAL::is_a_point() const {
  return (I.ERROR==0);
}

inline bool INTERVAL::is_finite() const {
  return (I.ERROR<fp::pInf);
}

inline double INTERVAL::upper_bound() const
{
  return to_double()+get_double_error();
}

inline double INTERVAL::lower_bound() const
{
  return to_double()-get_double_error();
}


inline void INTERVAL::operator+=(const INTERVAL& y) { *this=*this+y; }
inline void INTERVAL::operator-=(const INTERVAL& y) { *this=*this-y; }


inline bool INTERVAL::sign_is_known() const
{
#ifdef interval_relative_bound
  return (I.ERROR<1);
#else
  return (fp::abs(I.NUM)>I.ERROR) || (I.ERROR == 0);
#endif
}
inline int INTERVAL::sign() const
{  
  if (sign_is_known())
    return fp::sign(I.NUM);
  else
    LEDA_EXCEPTION(1,"error: sign cannot be determined");
  return 0;
  // never used
}



#endif


#ifndef INTERVAL_BOUND_RELATIVE_H
#define INTERVAL_BOUND_RELATIVE_H

#undef interval_inline_multiplication 
#undef interval_inline_addition

#undef INTERVAL
#define INTERVAL interval_bound_relative

#define interval_relative_bound



#ifndef BOUND_H
#define BOUND_H

struct _bound
{
  VOLATILE_I double NUM;
  VOLATILE_I double ERROR;

#ifndef interval_avoid_defaults
  _bound(const _bound& x) { NUM = x.NUM; ERROR = x.ERROR; }
  _bound& operator=(const _bound& x) 
  { NUM = x.NUM; ERROR = x.ERROR; return *this; }
#endif

  _bound() {}
  _bound(double num, double error) { NUM = num; ERROR = error; }
};

#endif


#undef IMP
#define IMP _bound


#undef ADD_INLINE
#ifdef interval_inline_addition
#define ADD_INLINE inline
#else
#define ADD_INLINE 
#endif

#undef MUL_INLINE
#ifdef interval_inline_multiplication
#define MUL_INLINE inline
#else
#define MUL_INLINE 
#endif

#undef interval_avoid_defaults
#ifndef with_exactness_check
#define with_exactness_check
#endif

/*{\Xanpage {interval} {} {Interval Arithmetic in LEDA} {x}}*/

class __exportC INTERVAL
{
/*{\Xdefinition
An instance of the data type |\Xtype| represents a real interval 
$I=[a,b]$. 
The basic interval operations $+,-,*,/,\sqrt{\phantom{x}}$ are available.
Type |\Xtype| can be used to approximate exact real arithmetic operations 
by inexact interval operations, as follows.
Each input number $x_i$ is converted into the interval $\{x_i\}$ 
and all real operations are replaced by interval operations.
If $x$ is the result of the exact real calculation and $I$ the 
interval computed by type |\Xtype|, it is guaranteed that $I$ contains $x$.
$I$ can be seen as a more or less accurate approximation of $x$.
In many cases the computed interval $I$ is small enough to provide 
a useful approximation of |x| and the {\em exact} sign of |x|.
There are four different implementations of |interval|s:
\begin{itemize}
\item Class |interval_round_inside|, which is the default implementation 
that can be accessed by the name |interval|
\item Classes |interval_bound_absolute| and |interval_bound_relative|
\item Class |interval_round_outside| which is usually the fastest but 
requires that the IEEE754 rounding mode |ieee_positive| is activated, 
e.g. by using the LEDA class |fpu|.
\end{itemize}
The interface of all |interval| variants are identical.
However, note that the types |interval_round_inside| and 
|interval_round_outside| are only available on some explicitly 
supported UNIX platforms, currently including SPARC, MIPS, i386 
(PC's compatible to 80386 or higher), and ALPHA. 
For all platforms, the name |\Xtype| stands for 
the default implementation |interval_bound_absolute|.
}*/


protected:

  IMP I;

  INTERVAL(double x, double y) : I(x,y) {}
  
public:
  
/*{\Xoptions ack=no }*/
/*{\Xoptions warnings=no }*/

  inline INTERVAL();
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with the interval $\{0\}$ }*/

  inline INTERVAL(VOLATILE_I double a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with $\{a\}$ }*/

  inline INTERVAL(int a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with $\{a\}$ }*/

  inline INTERVAL(long a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with $\{a\}$ }*/

  inline INTERVAL(unsigned int a);
  inline INTERVAL(unsigned long a);

  INTERVAL(const integer& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with the smallest possible interval containing |a| }*/

  INTERVAL(const bigfloat& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with the smallest possible interval containing |a| }*/

  INTERVAL(const real& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with the smallest possible interval containing |a| }*/

  INTERVAL(const rational& a);
/*{\Xcreate creates an instance |\Xvar| of type |\Xname| and initializes it
            with the smallest possible interval containing |a| }*/


  INTERVAL(const INTERVAL& x) { I = x.I; }
  INTERVAL& operator=(const INTERVAL& x) { I = x.I; return *this; }

/*{\Xoperations 2 4.7 }*/

/*{\Xtext The arithmetic operations |+,-,*,/,sqrt,+=,-=,*=,/=| and the stream 
   operators are all available.
   {\bf Important: } 
   If the advanced implementation |interval_round_outside| is used, the user has to guarantee that for each |interval| operation the IEEE754 rounding mode ''towards $+\infty$'' is active. This can be achieved by calling the function |fpu::round_up()|. To avoid side effects with library functions that require the default IEEE754 rounding mode |to_nearest|, the function |fpu::round_nearest()| can be used to reset the rounding mode. 
   }*/

  friend __exportF double to_double(const INTERVAL&);

  double to_double() const;
/*{\Xop returns the midpoint of the interval |\Xvar| as an approximation for the exact real number represented by |\Xvar|. }*/
  double get_double_error() const;
/*{\Xop returns the diameter of the interval |\Xvar| which is the maximal error of the approximation |\Xvar.to_double()| of the exact real number represented by |\Xvar|. }*/
  inline bool is_a_point() const;
/*{\Xop returns true if and only if the interval |\Xvar| consists of a single 
point. }*/
  inline bool is_finite() const;
/*{\Xop returns true if and only if the interval |\Xvar| is a finite interval. }*/
  bool contains(double x) const;
/*{\Xop returns true if and only if the interval |\Xvar| contains the number |x|}*/
  inline double upper_bound() const;
/*{\Xop returns the upper bound of the interval |\Xvar|. }*/
  inline double lower_bound() const;
/*{\Xop returns the lower bound of the interval |\Xvar|. }*/
  void set_range(VOLATILE_I double x, VOLATILE_I double y);
/*{\Xop sets the current interval to $[x,y]$. }*/
  void set_midpoint(VOLATILE_I double num, VOLATILE_I double error);
/*{\Xop sets the current interval to a superset of $[num-error,num+error]$,
    i.e., to an interval with midpoint |num| and radius |error|. }*/
  inline bool sign_is_known() const;
/*{\Xop returns true if and only if all numbers in the interval |\Xvar| have the same sign}*/
  inline int sign() const;
/*{\Xop returns the sign of all numbers in the interval |\Xvar| if this sign is unique; aborts with an error message if |\Xvar.sign_is_known()| gives false }*/
  friend __exportF int sign(const INTERVAL& x) { return x.sign(); }
  
  friend ADD_INLINE __exportF INTERVAL operator+(const INTERVAL&,const INTERVAL&);
  friend ADD_INLINE __exportF INTERVAL operator-(const INTERVAL&,const INTERVAL&);
  friend MUL_INLINE __exportF INTERVAL operator*(const INTERVAL&,const INTERVAL&);
  friend MUL_INLINE __exportF INTERVAL operator/(const INTERVAL&,const INTERVAL&);

  friend __exportF INTERVAL operator-(const INTERVAL&);
 
  inline void operator+=(const INTERVAL& y); 
  inline void operator-=(const INTERVAL& y); 
  void operator*=(const INTERVAL& y) { *this=*this*y; }
  void operator/=(const INTERVAL& y) { *this=*this/y; }

  friend __exportF INTERVAL sqrt(const INTERVAL& x);
  friend __exportF INTERVAL root(const INTERVAL& x, int d);
  friend __exportF INTERVAL  abs(const INTERVAL& x);

  friend __exportF ostream& operator<<(ostream& O, const INTERVAL& x);
  friend __exportF istream& operator>>(istream& I, INTERVAL&) { return I; }

  friend inline bool operator==(const INTERVAL& x, const INTERVAL& y)
  { return x.I.NUM == y.I.NUM && x.I.ERROR == y.I.ERROR; }

  friend inline bool operator!=(const INTERVAL& x, const INTERVAL& y)
  { return x.I.NUM != y.I.NUM || x.I.ERROR != y.I.ERROR; }

  inline void dummy();
  /*{\Ximplementation
    The types |interval_round_inside| and |interval_round_outside| represent intervals directly by (the negative of) its lower bound and its upper bound as |double|s. 
Here all arithmetic operations require that the IEEE754 rounding mode ''towards $+\infty$'' is active. For type |interval_round_inside| this is done {\em inside} each operation, and for type |interval_round_outside| the user has to do this manually ''from outside the operations'' by an explicit call of |fpu::round_up()|. 

The types |interval_bound_absolute| and |interval_bound_relative| represent intervals by their |double| midpoint |NUM| and diameter |ERROR|. The interpretation is that |NUM| is the numerical approximation of a real number and |ERROR| is a bound for the absolute, respectively relative error of |NUM|.

More details can be found in the full documentation of type |interval| and its variants.
}*/
};


inline void INTERVAL::dummy() { }


#ifdef interval_inline_addition
inline 
   INTERVAL operator+(const INTERVAL& x,const INTERVAL& y)
    { 
      VOLATILE_I double num = x.I.NUM + y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
{  
  if (num-x.I.NUM == y.I.NUM && num-y.I.NUM == x.I.NUM)
    return num;
}

#endif	
      
#ifdef interval_relative_bound
  if (fp::is_not_denormal(num)) 
   { 
     VOLATILE_I double term_x = x.I.NUM;
     fp::clear_sign_bit(term_x);
     VOLATILE_I double term_y = y.I.NUM;
     fp::clear_sign_bit(term_y);
     VOLATILE_I double term_z = num;
     fp::clear_sign_bit(term_z);
     error = (term_x*x.I.ERROR+term_y*y.I.ERROR+twoMinDbl)/term_z + eps;
   }
  else
   {  
     num =  fp::abs(x.I.NUM)*x.I.ERROR + fp::abs(y.I.NUM)*y.I.ERROR 
       + 4*MinDbl;
     error = 2;
   }
  error *= roundup_factor;
  if (fp::is_infinite(error)) // || (error-error)!=0)
    error = fp::pInf;
#else
  VOLATILE_I double term_z = num; 
  fp::clear_sign_bit(term_z);
  error = x.I.ERROR + y.I.ERROR + term_z*eps + threeMinDbl;
  error *= roundup_factor;
#endif

      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
    }

inline 
   INTERVAL operator-(const INTERVAL& x,const INTERVAL& y)
    { 
      VOLATILE_I double num = x.I.NUM - y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
{  
  if (num+y.I.NUM == x.I.NUM && x.I.NUM-num == y.I.NUM)
    return num;
}


#endif
      
#ifdef interval_relative_bound
  if (fp::is_not_denormal(num)) 
   { 
     VOLATILE_I double term_x = x.I.NUM;
     fp::clear_sign_bit(term_x);
     VOLATILE_I double term_y = y.I.NUM;
     fp::clear_sign_bit(term_y);
     VOLATILE_I double term_z = num;
     fp::clear_sign_bit(term_z);
     error = (term_x*x.I.ERROR+term_y*y.I.ERROR+twoMinDbl)/term_z + eps;
   }
  else
   {  
     num =  fp::abs(x.I.NUM)*x.I.ERROR + fp::abs(y.I.NUM)*y.I.ERROR 
       + 4*MinDbl;
     error = 2;
   }
  error *= roundup_factor;
  if (fp::is_infinite(error)) // || (error-error)!=0)
    error = fp::pInf;
#else
  VOLATILE_I double term_z = num; 
  fp::clear_sign_bit(term_z);
  error = x.I.ERROR + y.I.ERROR + term_z*eps + threeMinDbl;
  error *= roundup_factor;
#endif

      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
    }

#endif
#ifdef interval_inline_multiplication
inline 
   INTERVAL operator*(const INTERVAL& x,const INTERVAL& y)
    {
      VOLATILE_I double num = x.I.NUM * y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
{  
  if (fp::is_single_precision(x.I.NUM) && fp::is_single_precision(y.I.NUM))
    {  
      num = x.I.NUM * y.I.NUM;
      if (fp::is_finite(num) && fp::is_not_denormal(num))
	return num;
    }
}


#endif
      
#ifdef interval_relative_bound
  if (fp::is_not_denormal(num)) 
  { 
    error = x.I.ERROR + y.I.ERROR + x.I.ERROR*y.I.ERROR + eps;
  }
  else
  { 
    if ((x.I.NUM != 0) && (y.I.NUM != 0))
      { 
	num = MinDbl*(1 + x.I.ERROR)*(1 + y.I.ERROR);
	error = 2;
      }
    else 
      return INTERVAL(0,0);
  }
  error *= roundup_factor;
#else
  VOLATILE_I double term_x = x.I.NUM;
  fp::clear_sign_bit(term_x);
  VOLATILE_I double term_y = y.I.NUM;
  fp::clear_sign_bit(term_y);
  VOLATILE_I double term_z = num;
  fp::clear_sign_bit(term_z);
  error = x.I.ERROR*term_y+y.I.ERROR*(term_x+x.I.ERROR)+term_z*eps+threeMinDbl;
  error *= roundup_factor;
#endif
  if (fp::is_infinite(error)) //  || (error-error)!=0)
    error = fp::pInf;


   
      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
    }

inline 
  INTERVAL operator/(const INTERVAL& x,const INTERVAL& y)
    { 
      VOLATILE_I double num;
      if (y.I.NUM == 0)
	num = 1;
      else
	num = x.I.NUM/y.I.NUM;
      VOLATILE_I double error;
#ifdef with_exactness_check
      
if (x.I.ERROR + y.I.ERROR==0)
  if (fp::is_single_precision(y.I.NUM))
{  
  if (fp::is_single_precision(num) && num*y.I.NUM == x.I.NUM)
    if (fp::is_finite(num) && fp::is_not_denormal(num))
      return num;
}

#endif
      
#ifdef interval_relative_bound
  if (y.I.ERROR >= 1 || y.I.NUM == 0)
    error = fp::pInf; 
  else 
  {
    if (fp::is_not_denormal(num))
      error = (x.I.ERROR + y.I.ERROR) / (1 - y.I.ERROR) + eps;
    else
      { 
	if (x.I.NUM != 0)
	  { 
	    num = MinDbl*(1 + x.I.ERROR)/(1 - y.I.ERROR);
	    error = 2;
	  }
	else 
	  return 0;
      }
  }
#else
  VOLATILE_I double term_x = x.I.NUM;
  fp::clear_sign_bit(term_x);
  VOLATILE_I double term_y = y.I.NUM;
  fp::clear_sign_bit(term_y);
  VOLATILE_I double term_z = num;
  fp::clear_sign_bit(term_z);
  VOLATILE_I double y_low = term_y-y.I.ERROR;
  if (y_low > 0)
  {
    error = ((y.I.ERROR/term_y)*term_x+x.I.ERROR+MinDbl)/y_low + term_z*eps;
    error += twoMinDbl;
  }
  else
    error = fp::pInf;
#endif
error *= roundup_factor; 

      
if (fp::is_infinite(num))
{
  error = fp::pInf;
  num = 1;
}

      return INTERVAL(num,error);
   }
 

#endif


inline INTERVAL::INTERVAL()
{
  I.NUM = I.ERROR = 0;
}

inline INTERVAL::INTERVAL(VOLATILE_I double x)
{
  I.NUM = x; 
  if (fp::is_finite(x))
    I.ERROR = 0;
  else
    {
      I.NUM = 1;
      I.ERROR = fp::pInf;
    }
}

inline INTERVAL::INTERVAL(int x)
{
  I.NUM = x; 
  I.ERROR = 0;
}

inline INTERVAL::INTERVAL(long x)
{
  I.NUM = x; 
  I.ERROR = 0;
}

inline INTERVAL::INTERVAL(unsigned int x)
{
  I.NUM = x; 
  I.ERROR = 0;
}

inline INTERVAL::INTERVAL(unsigned long x)
{
  I.NUM = x; 
  I.ERROR = 0;
}


inline bool INTERVAL::is_a_point() const {
  return (I.ERROR==0);
}

inline bool INTERVAL::is_finite() const {
  return (I.ERROR<fp::pInf);
}

inline double INTERVAL::upper_bound() const
{
  return to_double()+get_double_error();
}

inline double INTERVAL::lower_bound() const
{
  return to_double()-get_double_error();
}


inline void INTERVAL::operator+=(const INTERVAL& y) { *this=*this+y; }
inline void INTERVAL::operator-=(const INTERVAL& y) { *this=*this-y; }


inline bool INTERVAL::sign_is_known() const
{
#ifdef interval_relative_bound
  return (I.ERROR<1);
#else
  return (fp::abs(I.NUM)>I.ERROR) || (I.ERROR == 0);
#endif
}
inline int INTERVAL::sign() const
{  
  if (sign_is_known())
    return fp::sign(I.NUM);
  else
    LEDA_EXCEPTION(1,"error: sign cannot be determined");
  return 0;
  // never used
}




#endif


typedef interval_bound_absolute interval;

#if LEDA_ROOT_INCL_ID == 600260
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

