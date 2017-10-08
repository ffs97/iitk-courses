/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  filtered_rational.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FILTERED_RATIONAL_H
#define LEDA_FILTERED_RATIONAL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600261
#include <LEDA/internal/PREAMBLE.h>
#endif


/*{\Xanpage {filtered_rational} {} {Filtered Rationals} {x}}*/

#include <LEDA/numbers/filtered_integer.h>

LEDA_BEGIN_NAMESPACE

class __exportC filtered_rational
{

/*{\Xdefinition An instance $q$ of type |\Mname| is a filtered rational number
where the numerator and the denominator are both of type
|filtered_integer|.}*/

protected:
  filtered_integer num; // numerator
  filtered_integer den; // denominator, always nonzero and positive


public:

/*{\Xcreation q}*/

  filtered_rational() : num(0), den(1) {}
/*{\Xcreate creates an instance |\Mvar| of type |\Mname|.}*/

  filtered_rational(filtered_integer n) : num(n), den(1) {}
/*{\Xcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the filtered integer $n$.}*/


  filtered_rational(int d , int n=1);
/*{\Xcreate creates an instance |\Mvar| of type |\Mname| and initializes its
            the filtered rational number $n/d$.}*/

  filtered_rational(const filtered_integer& d, const filtered_integer& n);

  filtered_rational(const filtered_rational& r) : num(r.num), den(r.den) {}

  filtered_rational& operator=(const filtered_rational&);


 ~filtered_rational() {}


/*{\Xoperations 2 5}*/

/*{\Xtext
The arithmetic operations $+$, $-$, $*$, $/$, $+=$, $-=$, $*=$, $/=$,
$-$(unary), $++$, $--$, the comparison operations $<$, $<=$, $>$, $>=$,
$==$, $!=$ and the stream operations are all available.}*/

  filtered_rational& operator+= (const filtered_rational& r);
  filtered_rational& operator-= (const filtered_rational&);
  filtered_rational& operator*= (const filtered_rational&);
  filtered_rational& operator/= (const filtered_rational&);

  filtered_rational& operator++ () { num += den; return *this; }
  filtered_rational& operator-- () { num -= den; return *this; }

// arithmetic operators

  friend inline filtered_rational operator+ (const filtered_rational& q, 
                                             const filtered_rational& r);

  friend inline filtered_rational operator- (const filtered_rational& q, 
                                             const filtered_rational& r);

  friend inline filtered_rational operator* (const filtered_rational& q, 
                                             const filtered_rational& r);

  friend inline filtered_rational operator/ (const filtered_rational& q, 
                                             const filtered_rational& r);

  friend inline filtered_rational operator- (const filtered_rational&);


// comparison operators

  static int cmp(const filtered_rational&, const filtered_rational&);


  friend inline bool operator==(const filtered_rational& q, 
                                const filtered_rational& r);

  friend inline bool operator< (const filtered_rational& q, 
                                const filtered_rational& r);


#ifndef LEDA_STL_REL_OPS_CONFLICT
  friend inline bool operator!=(const filtered_rational& q, 
                                const filtered_rational& r);

  friend inline bool operator<=(const filtered_rational& q, 
                                const filtered_rational& r);

  friend inline bool operator> (const filtered_rational& q, 
                                const filtered_rational& r);

  friend inline bool operator>=(const filtered_rational& q, 
                                const filtered_rational& r);
#endif



  integer numerator()   const { return num.exact_value(); }
/*{\Xop returns the numerator of $q$.}*/

  integer denominator() const { return den.exact_value(); }
/*{\Xop returns the denominator of $q$.}*/


  filtered_rational inverse();  
/*{\Xop returns the inverse of |\Mvar|.}*/

  double  to_float() const
  { return (num.to_float()/den.to_float()); } 

  double  to_double() const { return to_float(); }
/*{\Xfunc returns a double floating point approximation of |\Mvar|. }*/



/*{\Xtext
\smallskip
{\bf Non-member functions}
\smallskip }*/

  friend inline int sign(const filtered_rational& q);
/*{\Xfunc returns the sign of $q$.}*/

  friend inline filtered_rational abs(const filtered_rational& q);
/*{\Xfunc returns the absolute value of $q$.}*/

  friend inline filtered_rational sqr(const filtered_rational& q);
/*{\Xfunc returns the square of $q$.}*/


  friend __exportF istream& operator>> (istream& in, filtered_rational& q);
  friend __exportF ostream& operator<< (ostream& out, const filtered_rational& q);

};


inline  filtered_rational operator+(const filtered_rational& x, 
                                    const filtered_rational& y)
{ filtered_rational z = x; return z += y; }

inline  filtered_rational operator-(const filtered_rational& x, 
                                    const filtered_rational& y)
{ filtered_rational z = x; return z -= y; }

inline  filtered_rational operator*(const filtered_rational& x, 
                                    const filtered_rational& y)
{ filtered_rational z = x; return z *= y; }

inline  filtered_rational operator/(const filtered_rational& x, 
                                    const filtered_rational& y)
{ filtered_rational z = x; return z /= y; }

inline  filtered_rational operator-(const filtered_rational& x)
{ return filtered_rational(-x.num,x.den); }


// comparison operators

inline  bool operator==(const filtered_rational& x, const filtered_rational& y)
  { return x.num * y.den == x.den * y.num; }

inline  bool operator!=(const filtered_rational& x, const filtered_rational& y)
  { return x.num * y.den != x.den * y.num; }

inline  bool operator<(const filtered_rational& x, const filtered_rational& y)
  { return filtered_rational::cmp(x,y) < 0; }

inline  bool operator<=(const filtered_rational& x, const filtered_rational& y)
  { return filtered_rational::cmp(x,y) <= 0; }

inline  bool operator>(const filtered_rational& x, const filtered_rational& y)
  { return filtered_rational::cmp(x,y) > 0; }

inline  bool operator>=(const filtered_rational& x, const filtered_rational& y)
  { return filtered_rational::cmp(x,y) >= 0; }




  inline int sign(const filtered_rational& q) 
  { return sign(q.num); }

  inline filtered_rational abs(const filtered_rational& q)
  { if (q < 0) return -q; else return q; }

  inline filtered_rational sqr(const filtered_rational& q)
  { return q*q; }




inline const char* leda_tname(const filtered_rational*) 
{ return "filtered_rational"; }

COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE(const filtered_rational& x, 
                           const filtered_rational& y) 
{ return filtered_rational::cmp(x,y); } 



/*{\Ximplementation A $filtered_rational$ is implemented by two $filtered_integer$
numbers which represent the numerator and the denominator.}*/


#if LEDA_ROOT_INCL_ID == 600261
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif

