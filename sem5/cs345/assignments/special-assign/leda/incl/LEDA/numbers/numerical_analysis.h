/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  numerical_analysis.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NUMERICAL_ANALYSIS_H
#define LEDA_NUMERICAL_ANALYSIS_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600252
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>
#include <LEDA/system/misc.h>

#include <LEDA/internal/std/math.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {numerical_analysis} {} {Numerical Analysis Functions} }*/


/*{\Mtext
%\section{Functions of Numerical Analysis}

We collect some functions of numerical analysis.
\emph{The algorithms in this section are not the best known and
are not recommended for serious use}. We refer the reader to 
the book ``Numerical Recipes in {C}: {T}he Art of Scientific Computing''
by B.P. Flannery, W.H. Press, S.A. Teukolsky, and W.T. Vetterling,
Cambridge University Press for better algorithms.

The functions in this section become available by including
|numerical_analysis.h|.


\subsection{Minima and Maxima}

\settowidth{\typewidth}{$\mathit double$}
\computewidths
}*/

double minimize_function(double (*f)(double), double& xmin, 
double tol = 1.0e-10);

/*{\Mfunc finds a local minimum of the function $f$ of one argument.
The 
minimizing argument is returned in |xmin| and the minimal function
value is returned as the result of the function. |xmin| is determined
with tolerance |tol|, i.e., the true value of the minimizing argument is
contained in the interval $[|xmin|(1 - \epsilon),|xmin|(1 + \epsilon)]$, where
$\epsilon = \max(1,|xmin|)\cdot |tol|$.  Please do not choose |tol| smaller than
$10^{-15}$.

\precond: If $+\infty$ or $-\infty$ is a local minimum of $f$, then the
call of |minimize_function| may not terminate.  

The algorithm is implemented as follows: First three arguments are determined
such that $a < b < c$ (or $a > b > c$) and 
$f(a) \ge f(b) \le f(c)$, i.e., $a$ and $c$ bracket
a minimum. The interval is found by first taking two arbitrary arguments
and comparing their function values. The argument with the larger 
function value is taken as $a$. Then steps of larger and larger 
size starting at $b$ are taken until a function value larger 
than $f(b)$ is found. Once the bracketing interval is found, 
golden-ratio search is applied to it.
}*/


/*{\Moptions nextwarning=no }*/
template <class F>
double minimize_function(const F& f, double& xmin, double tol = 1.0e-10);

/*{\Mfunc a more flexible version of the above. 
          It is assumed that class $F$ offers the operator\\
          |double operator()(double x)|. This operator is taken
          as the function $f$.
}*/



template <class F>
void bracket_minimum(const F& f, double& a, double& b, double& c)
{ 
  const double GOLD = 1.618034;
  double fa, fb, fc;

  a = 0;  fa = f(a);
  b = 1;  fb = f(b);

  if ( fb > fa) { leda_swap(a,b); leda_swap(fa,fb);}
  // we have fb <= fa;

  while ( true )
  { c = b + GOLD*(b - a);  fc = f(c);
    if ( fb <= fc ) break;
    a = b; fa = fb;
    b = c; fb = fc;
  }

 }


template <class F>
double minimize_function(const F& f, double& xmin, double tol)
{ 
  const double GOLD = 1.618034;
  const double R    = GOLD - 1;   // about 0.61
  const double C    = 1 - R;      // about 0.31


  double a, b, c, f1, f2, x0, x1, x2, x3;

  bracket_minimum(f,a,b,c);

  x0 = a; x3 = c;

  if ( fabs(c - b) > fabs(b - a) )  
    { x1 = b; x2 = b + C*(c - b); }
  else
    { x2 = b; x1 = b - C*(b - a); }

  f1 = f(x1); f2 = f(x2);

  while ( fabs(x3 - x0) > tol * leda_max(1.0,fabs(x1) + fabs(x2)) )
  { 
    if ( f2 < f1 ) 
    { x0 = x1; x1 = x2; x2 = R*x1 + C*x3;
               f1 = f2; f2 = f(x2);
    }
    else
    { x3 = x2; x2 = x1; x1 = R*x2 + C*x0;
               f2 = f1; f1 = f(x1);
    }
  }

  xmin = ( f1 < f2 ? x1 : x2);
  return f(xmin);

 }



/*{\Mtext

\subsection{Integration}

\settowidth{\typewidth}{$\mathit double$}
\computewidths
}*/

double integrate_function(double (*f)(double), double l, double r, 
double delta = 1.0e-2);

/*{\Mfunc Computes the integral of $f$ in the interval 
$[l,r]$ by forming the sum 
$|delta| * \sum_{0 \le i < K} f(l + i \cdot |delta|)$, where
$K = (r - l)/|delta|$.\\
\precond $l \le r$ and $|delta| > 0$.
}*/


/*{\Moptions nextwarning=no }*/
template <class F>
double integrate_function(const F& f, double l, double r, double delta=1.0e-2);

/*{\Mfunc a more flexible version of the above. 
          It is assumed that class $F$ offers the operator\\
          |double operator()(double x)|. This operator is taken 
          as the function $f$.
}*/



template <class F>
double integrate_function(const F& f, double l, double r, 
double delta)
{ 
  leda_assert(l <= r,"integrate function: r must not be smaller than l");

  leda_assert(delta > 0,"integrate function: expect delta > 0",1);
  double sum = 0;
  for (double x = l; x < r; x += delta) sum += f(x);

  return delta*sum;



 }



/*{\Mtext
\subsection{Useful Numerical Functions}

\settowidth{\typewidth}{$\mathit double$}
\computewidths

}*/


double binary_entropy(double x);

/*{\Mfunc returns the binary entropy of $x$, i.e.,
$- x \cdot \log x - (1 - x) \cdot \log (1-x)$.\\
\precond $0 \le x \le 1$.
}*/



/*{\Mtext

\subsection{Root Finding}

\settowidth{\typewidth}{$\mathit double$}
\computewidths
}*/


double zero_of_function(double (*f)(double), double l, double r, 
double tol = 1.0e-10);

/*{\Mfunc returns a zero $x$ of $f$. We have either
$\Labs{f(x)} \le 10^{-10}$ or
there is an interval $[x_0,x_1]$ containing $x$
such that $f(x_0) \cdot f(x_1) \le 0$ and 
$x_1 - x_0 \le |tol|\cdot \max(1,\Labs{x_1} + \Labs{x_1})$.\\
\precond $l \le r$ and $f(l) \cdot f(r) \le 0$.
}*/


/*{\Moptions nextwarning=no }*/
template <class F>
double zero_of_function(const F& f, double l, double r, double tol = 1.0e-10);

/*{\Mfunc a more flexible version of the above. 
          It is assumed that class $F$ offers the operator\\
          |double operator()(double x)|. This operator is taken 
          as the function $f$.
}*/



template <class F>
double zero_of_function(const F& f, double l, double r, 
double tol)
{ 
  if ( !leda_assert(l <= r,"zero_of_function: expect l <= r") )
    r = l;

  if ( f(l) * f(r) > 0 ) 
    LEDA_EXCEPTION(1,"zero_of_function: expect f(l)*f(r) <= 0");

  if ( f(l) == 0 ) return l;
  if ( f(r) == 0 ) return r;

  double x0 = l;
  double x1 = r;

  bool increasing = ( f(x0) < 0 );

  while ( x1 - x0 > tol * leda_max(1.0, fabs(x0) + fabs(x1)) )
  { double y0 = f(x0);
    double y1 = f(x1);
    double xnew = x0 - y0 * (x1 - x0) / (y1 - y0);
    double ynew = f(xnew);

    //cout << "\n\n" << xnew << " " << ynew;

    if ( fabs(ynew) < 1.0e-10 ) return xnew;
       
    if ( ynew > 0 && increasing )
      { x1 = xnew; y1 = ynew; }
    else
      { x0 = xnew; y0 = ynew; }
  } 

  return x0;
 }


#if LEDA_ROOT_INCL_ID == 600252
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
