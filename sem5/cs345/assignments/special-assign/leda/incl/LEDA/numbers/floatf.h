/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  floatf.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FFLOAT_H
#define LEDA_FFLOAT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600255
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>
#include <LEDA/numbers/integer.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {floatf} {} {A Floating Point Filter} }*/ 


const int NO_IDEA = 2;

const double eps0 = ldexp(1,-53);   // \eps0 = 2^-53


class floatf {

/*{\Mdefinition
The type $floatf$ provides a clean and efficient way to approximately
compute with large integers. Consider an expression $E$ with integer
operands and operators $+,-$, and $*$, and suppose that we want
to determine the sign of $E$. In general, the integer arithmetic
provided by our machines does not suffice to evaluate $E$ since intermediate
results might overflow. Resorting to arbitrary precision integer
arithmetic is a costly process. An alternative is to evaluate the 
expression using floating point arithmetic, i.e., to convert the
operands to doubles and to use floating-point addition, subtraction,
and multiplication. 

Of course, only an approximation $E'$ of the true value $E$ is
computed. However, $E'$ might still be able to tell us
something about the sign of $E$. If $E'$ is far away from zero
(the forward error analysis carried out in the next section gives a
precise meaning to "far away") then the signs of $E'$ and $E$
agree and if $E'$ is zero then we may be able to conclude under
certain circumstances that $E$ is zero. Again, forward error analysis
can be used to say what `certain circumstances' are. 

The type $floatf$ encapsulates this kind of approximate integer
arithmetic. Any integer (= object of type $integer$) can be converted
to a $floatf$; $floatf$s can be added, subtracted, multiplied, and
their sign can be computed: for any $floatf$ $x$ the function
$Sign(x)$ returns either the sign of $x$ ($-1$ if $x < 0$, $0$ if $x =
0$, and $+1$ if $x > 0$) or the special value $NO\_IDEA$.  If $x$
approximates $X$, i.e., $X$ is the integer value obtained by an exact
computation, then $Sign(x) != NO\_IDEA$ implies that $Sign(x)$ is
actually the sign of $X$ if $Sign(x) = NO\_IDEA$ then no claim is made
about the sign of $X$.  }*/


  double num;
  double mes;
  float  ind;

floatf(double d, double m, float i) { num = d; mes = m; ind = i;}

public:


/*{\Mcreation x }*/

floatf() { num = 0; mes = 1; ind = 0.5; }
/*{\Mcreate   introduces a variable |\Mvar| of type |\Mname| and initializes
              it with zero. }*/

floatf(integer i) 
{ num = i.to_double();
  mes = ldexp(1, log(abs(i)+1) + 1);
  ind = 0.5;
 }
/*{\Mcreate   introduces a variable |\Mvar| of type |\Mname| and initializes
              it with integer $i$. }*/


/*{\Moperations 2 4 }*/


operator double() const { return num; }


friend floatf operator+(const floatf& a, const floatf& b)
{ return floatf(a.num + b.num, 2 * ((a.mes > b.mes) ? a.mes : b.mes),
                              (a.ind + b.ind + 1)/2); }
/*{\Mbinopfunc   Addition. }*/

friend floatf operator-(const floatf& a, const floatf& b)
{ return floatf(a.num - b.num, 2 * ((a.mes > b.mes) ? a.mes : b.mes),
                              (a.ind + b.ind + 1)/2); }
/*{\Mbinopfunc   Subtraction. }*/

friend floatf operator*(const floatf& a, const floatf& b)
{ return floatf(a.num * b.num, a.mes * b.mes, a.ind + b.ind + 0.5 ); }
/*{\Mbinopfunc   Multiplication. }*/


friend int Sign(const floatf& f)
{ double eps = f.ind * f.mes * eps0;
  if (f.num >  eps)  return  1;
  else
  if (f.num < -eps)  return -1;
  else
  if (eps < 1)       return  0;
  else
  return NO_IDEA;
 }
/*{\Mfunc  as described above. }*/


/*{\Mimplementation
A $floatf$ is represented by a double (its value) and an error bound.
An operation on $floatf$s performs the corresponding operation on
the values and also computes the error bound for the result. For
this reason the cost of a $floatf$ operation is about four times
the cost of the corresponding operation on doubles. The rules 
used to compute the error bounds are described in 
(\cite{Me-Naeher:sweep}).}*/


};

/*{\Mexample
see \cite{Me-Naeher:sweep} for an application in a sweep line algorithm.
}*/

inline int Sign(const integer& x)  { return sign(x); }

inline int Sign(double x)
{ if (x==0) return 0;
  else return (x>0) ? 1 : -1;
 }


#if LEDA_ROOT_INCL_ID == 600255
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
