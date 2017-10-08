/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rational.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_RATIONAL_H
#define LEDA_RATIONAL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600249
#include <LEDA/internal/PREAMBLE.h>
#endif


/*{\Manpage {rational} {} {Rational Numbers}}*/

#include <LEDA/numbers/integer.h>

LEDA_BEGIN_NAMESPACE

class __exportC rational
{

/*{\Mdefinition An instance $q$ of type |\Mname| is a rational number
where the numerator and the denominator are both of type
$integer$.}*/

protected:

  integer num; // numerator
  integer den; // denominator, always positive

  
  rational(integer_rep* xptr,integer_rep* yptr) {
    num.PTR=xptr; den.PTR=yptr;
  }

  static rational addsub(const rational& x, const rational& y, bool subtract);


public:

/*{\Mcreation q}*/

  rational() : num(0), den(1) {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|.}*/


  rational(integer n) : num(n), den(1) {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the integer $n$.}*/

#if defined(__DMC__)
  rational(int n) : num(n), den(1) {}
#endif

/*{\Moptions nextwarning=no }*/
  rational(int, int);

  rational(integer n, integer d);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            to the rational number $n/d$.}*/



  rational(double x);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the value of $x$.}*/

  rational(const rational& r) : num(r.num), den(r.den) {}
  rational& operator=(const rational&);

 ~rational() {}

  
/*{\Moperations 2 5}*/

/*{\Mtext
The arithmetic operations $+$, $-$, $*$, $/$, $+=$, $-=$, $*=$, $/=$,
$-$(unary), $++$, $--$, the comparison operations $<$, $<=$, $>$, $>=$,
$==$, $!=$ and the stream operations are all available.}*/

  rational& operator+= (const rational&);
  rational& operator-= (const rational&);
  rational& operator*= (const rational&);
  rational& operator/= (const rational&);
  rational& operator++ ();
  rational& operator-- ();

  friend __exportF rational operator+ (const rational& q, const rational& r);
  friend __exportF rational operator- (const rational& q, const rational& r);
  friend __exportF rational operator* (const rational& q, const rational& r);
  friend __exportF rational operator/ (const rational& q, const rational& r);
  friend __exportF rational operator- (const rational&);

  
  static int cmp(const rational&, const rational&);

  friend __exportF bool operator==(const rational& q, const rational& r);
  friend __exportF bool operator==(const rational& q, const integer& a);
  friend __exportF bool operator==(const integer& n, const rational& q);
  friend __exportF bool operator==(const rational& q, int n);
  friend __exportF bool operator==(int n, const rational& q);

  friend __exportF bool operator< (const rational& q, const rational& r);

#ifndef LEDA_STL_REL_OPS_CONFLICT
  friend __exportF bool operator!=(const rational& q, const rational& r);
  friend __exportF bool operator!=(const rational& q, int n);
  friend __exportF bool operator!=(int n, const rational& q);
  friend __exportF bool operator!=(const rational& q, const integer& a);
  friend __exportF bool operator!=(const integer& n, const rational& q);

  friend __exportF bool operator<=(const rational& q, const rational& r);
  friend __exportF bool operator> (const rational& q, const rational& r);
  friend __exportF bool operator>=(const rational& q, const rational& r);
#endif // LEDA_STL_REL_OPS_CONFLICT


  
  void negate(); 
/*{\Mop negates |\Mvar|.}*/
  void invert(); 
/*{\Mop inverts |\Mvar|.}*/
  rational inverse();  
/*{\Mop returns the inverse of |\Mvar|.}*/
  integer numerator() const;
/*{\Mop returns the numerator of $q$.}*/
  integer denominator() const;
/*{\Mop returns the denominator of $q$.}*/
  rational& simplify(const integer& a);
/*{\Mop simplifies $q$ by $a$.\\
        \precond $a$ divides the numerator and the denominator of $q$.}*/
  rational& normalize();
/*{\Mop normalizes |\Mvar|.}*/
/*{\Moptions nextwarning=no }*/

  double to_double_precise() const;
  double to_double_precise(double& abs_err) const;

  double to_double() const;

  double to_float() const { return to_double(); }
/*{\Mfunc returns a double floating point approximation of |\Mvar|. 
   If the |\Mvar| is approximable by a {\em normalized, finite} floating 
   point number, the error is 3ulps, i.e., three units in the last place. 
}*/

  string to_string() const;
/*{\Mop returns a string representation of |\Mvar|. }*/

  // the following functions are just for compatiblity
  double todouble() const { return to_double(); }
  string tostring() const { return to_string(); }


  
/*{\Mtext
\smallskip
{\bf Non-member functions}
\smallskip }*/

  friend __exportF int sign(const rational& q);
/*{\Mfunc returns the sign of $q$.}*/
  friend __exportF rational abs(const rational& q);
/*{\Mfunc returns the absolute value of $q$.}*/
  friend __exportF rational sqr(const rational& q);
/*{\Mfunc returns the square of $q$.}*/
  friend __exportF integer trunc(const rational& q);
/*{\Mfunc returns the $integer$ with the next smaller absolute value.}*/
  friend __exportF rational pow(const rational& q, int n); 
/*{\Mfunc returns the $n$-th power of $q$.}*/
  friend __exportF rational pow(const rational& q, integer a); 
/*{\Mfunc returns the $a$-th power of $q$.}*/
  friend __exportF integer floor(const rational& q);
/*{\Mfunc returns the next smaller $integer$.}*/
  friend __exportF integer ceil(const rational& q);
/*{\Mfunc returns the next bigger $integer$.}*/
  friend __exportF integer round(const rational& q);
/*{\Mfunc rounds $q$ to the nearest $integer$.}*/

// deprecated
  friend double Double(const rational& q) { return q.to_double(); }
/*{\Xfunc returns a double floating point approximation of $q$.}*/

  friend __exportF istream& operator>> (istream& in, rational& q);
  friend __exportF ostream& operator<< (ostream& out, const rational& q);

  friend string to_string(const rational& x) { return x.to_string(); }


};

/*
inline const rational& min(const rational& a, const rational& b) { return (a<b) ? a : b; }
inline const rational& max(const rational& a, const rational& b) { return (a>b) ? a : b; }
*/

inline const char* leda_tname(const rational*) { return "rational"; }

inline int leda_type_id(const rational*)   { return RATIONAL_TYPE_ID; }

inline int DEFAULT_COMPARE(const rational& x, const rational& y) 
{ return rational::cmp(x,y); } 


extern __exportF rational small_rational_between(const rational& p, const rational& q);
/*{\Mfunc returns a rational number between $p$ and $q$ whose denominator is as 
          small as possible.}*/
extern __exportF rational small_rational_near(const rational& p, rational eps);
/*{\Mfunc returns a rational number between $p - eps$ and $p + eps$ whose 
          denominator is as small as possible.}*/





/*{\Mimplementation A $rational$ is implemented by two $integer$
numbers which represent the numerator and the denominator. The sign
is represented by the sign of the numerator.}*/

#ifndef NO_LEDA
#if LEDA_ROOT_INCL_ID == 600249
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif
#endif



LEDA_END_NAMESPACE

#endif


