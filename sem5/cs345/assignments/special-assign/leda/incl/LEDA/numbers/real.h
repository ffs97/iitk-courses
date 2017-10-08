/*******************************************************************************
+
+  LEDA 6.3
+
+
+  real.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


// ============================================================================
// Author(s): Algorithmic Solutions <leda@algorithmic-solutions.com>
//            Christoph Burnikel, Rudolf Fleischer, Stefan Funke,
//            Kurt Mehlhorn, Stefan Schirra,
//            Susanne Schmitt  <sschmitt@mpi-sb.mpg.de>
//
// ============================================================================

#ifndef _LEDA_REAL_H
#define _LEDA_REAL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600259
#include <LEDA/internal/PREAMBLE.h>
#endif

#define LEDA_REAL_VERSION 200
#define BFMSS_ONLY

#include <LEDA/core/list.h>
#include <LEDA/numbers/bigfloat.h>
#include <LEDA/numbers/rational.h>


// the following forward declarations are needed for g++ (is this still true ?)
#if defined(__GNUC__)
LEDA_BEGIN_NAMESPACE
class __exportC real;
__exportF int  sign(const real& x);
__exportF bool sign_is_known(const real& x);
LEDA_END_NAMESPACE
#endif

#include <LEDA/numbers/polynomial.h>


LEDA_BEGIN_NAMESPACE


class __exportC real;
typedef polynomial<real> Polynomial;
typedef polynomial<rational> rat_Polynomial;
typedef polynomial<integer> int_Polynomial;


enum algorithm_type { USPENSKY=0, USPENSKY_WITH_GCD=1, STURM=2 };

#define ERROR_PREC 32
#define DOUBLE_PREC 53

/*{\Manpage {real} {} {The data type real} 
}*/

/*{\Mdefinition
An instance $x$ of the data type \Mtype is a real algebraic number. 
There are many ways to construct a \Mtype: 
either by conversion from |double|, |bigfloat|, |integer| or |rational|,
by applying one of the arithmetic operators $+, -, *, /$ or $\sqrt[d]{} $ to 
\Mtype numbers or by using the $\diamond$-operator to define a real root of a
polynomial over \Mtype numbers.
One may test the sign of a \Mtype number or compare two \Mtype numbers by
any of the comparison relations $=, \neq, < , \leq, >$ and $\geq$.
The outcome of such a test is mathematically {\em exact}. 
We give consider an example expression to clarify this:
$$ x := (\sqrt{17} - \sqrt{12}) * (\sqrt{17} + \sqrt{12}) - 5 $$
Clearly, the value of $x$ is zero. But if you evaluate $x$ using double 
arithmetic you obtain a tiny non-zero value due to rounding errors.
If the data type \Mtype is used to compute $x$ then |sign(x)| yields zero.
\SKIP{
There is also a non--standard version of the sign function: the call
|x.sign(integer q)| 
computes the sign of |x| under the precondition
that $\Lvert x\Lvert \leq 2^{-q}$ implies $x = 0$.
This version of the sign function allows the user to assist the data type in the
computation of the sign of $x$, see the example below.
}

There are several functions to compute approximations of \Mtype{s}.
The calls |x.to_bigfloat()| and |x.get_bigfloat_error()| return |bigfloat|s
|xnum| and |xerr| such that $\Lvert |xnum| - x \Lvert \leq |xerr|$. 
The user may set a bound on |xerr|.
More precisely, after the call |x.improve_approximation_to(integer q)| the
data type guarantees $|xerr| \leq 2^{-q}$. 
One can also ask for |double| approximations of a real number $x$.
The calls |x.to_double()| and |x.get_double_error()| return |double|s |xnum| and
|xerr| such that $\Lvert |xnum| - x \Lvert \leq |xerr|$. 
Note that $|xerr| = \infty$ is possible. 
}*/

class __exportC real_rep; 
class __exportC real 
{ 
  friend class real_rep;

/*{\Mtypes 7.5}*/ 
  typedef polynomial<real> Polynomial;
  /*{\Mtypedef the polynomial type.}*/

protected:

  double value;
  real_rep* PTR;
  bool is_double;

  void adjust() const;

  
  static void convert_int_poly_to_real_poly(const int_Polynomial& iP, Polynomial& P);
  

  void clear_visited_marks();

public:
  static algorithm_type isolating_algorithm;

  enum io_mode { IO_to_double, IO_dag, IO_interval };
private:
  static io_mode output_mode;

/*{\Mcreation x
}*/

/*{\Mtext 
\Mtype{s} may be constructed from data types |double|, |bigfloat|,
 |long|, |int| and |integer|.
 The default constructor |real()| initializes the \Mtype\ to zero.
}*/

public:
  real();
  real(double);
  real(int);
  real(const integer&);
  real(const bigfloat&);
  real(const rational&);
  real(const real&);
  real(real_rep&);

  ~real();

  real& operator=(const real&);

/*{\Moperations 1.7 3.5
}*/
/*{\Mtext \settowidth{\typewidth}{|bigfloat|}
          \addtolength{\typewidth}{\colsep}
          \computewidths
}*/


  friend __exportF double to_double(const real&);
  friend __exportF bigfloat to_bigfloat(const real&);


  double to_double() const;
/*{\Mop returns the current double approximation of \Mvar. 
}*/
  
  double to_double(double& error) const;
/*{\Mop as above, but also computes a bound on the absolute error. 
}*/
  
  double to_close_double() const
  { improve_approximation_to(10); return to_double(); }

  bigfloat to_bigfloat() const; 
/*{\Mop returns the current bigfloat approximation of \Mvar. 
}*/

  double get_double_error() const;
/*{\Mop returns the {\em absolute} error of the current double approximation of 
   \Mvar, i.e., $\Lvert |x-x.to_double()|\Lvert\leq|x.get_double_error()|$.
   % previously returned {\em relative} error. 
}*/
  bigfloat get_bigfloat_error() const;
/*{\Mop returns the absolute error of the current bigfloat approximation of
   |x|, i.e., $\Lvert|x-x.to_bigfloat()|\Lvert\leq|x.get_bigfloat_error()|$.
}*/
  bigfloat get_lower_bound() const;
/*{\Mop returns the lower bound of the current interval approximation 
   of |x|.
}*/
  bigfloat get_upper_bound() const;
/*{\Mop returns the upper bound of the current interval approximation 
   of |x|.
}*/

  rational high() const;
/*{\Mop returns a rational upper bound of the current interval approximation
    of |x|.
}*/

  rational low() const;
/*{\Mop returns a rational lower bound of the current interval approximation
    of |x|.
}*/

  double get_double_lower_bound() const;
/*{\Mop returns a |double| lower bound of |x|.
}*/
  double get_double_upper_bound() const;
/*{\Mop returns a |double| upper bound of |x|.
}*/

  bool possible_zero() const;
/*{\Mop returns true if 0 is in the current interval approximation of |x|
}*/

  integer separation_bound();
/*{\Mop returns the separation bound of |x|.
}*/

  integer sep_bfmss();
/*{\Mop returns the k-ary BFMSS separation bound of |x|.
}*/

#ifndef BFMSS_ONLY
  integer sep_degree_measure();
/*{\Mop returns the degree measure separation bound of |x|.
}*/

  integer sep_li_yap();
/*{\Mop returns the Li / Yap separation bound of |x|.
}*/
#endif

  void print_separation_bounds();
/*{\Mop prints the different separation bounds of |x|.
}*/

  bool is_general() const;
/*{\Mop returns true if the expression defining |x| contains a 
    $\diamond$-operator, false otherwise.
}*/
  bool is_rational() const;
/*{\Mop returns true if the expression is rational, false otherwise.
}*/
  rational to_rational() const;
/*{\Mop returns the rational number given by the expression. \\
    \precond |is_rational()| has is true.
}*/


  rat_Polynomial get_polynomial() const;
  void set_polynomial(const Polynomial& P);


  int compare(const real& y);
/*{\Mop returns the sign of \Mvar-y.
}*/

  friend int compare_all(const growing_array<real>& R, const real& x);

  friend int compare_all(const growing_array<real>& R, int& j);
/*{\Mfunc compares all elements in $R$. It returns $i$ such that $R[i]=R[j]$
and $i\neq j$. Precondition: Only two of the elements in $R$ are equal.
\EXPERIMENTAL
}*/

  friend __exportF int sign(const real& x);
  friend __exportF bool sign_is_known(const real& x);

  bool sign_is_known() const;

  int sign() const;
/*{\Mop returns the sign of (the exact value of) \Mvar. 
}*/

  int sign(const integer& q) const; 
/*{\Mop as above. Precondition: The user guarantees that 
   $\Lvert x \Lvert \leq  2^{-q}$ is only possible if $x=0$.
   This advanced version of the |sign| function should be applied only
   by the experienced user. It gives an improvement over the 
   plain |sign| function only in some cases. 
}*/
  int sign(long) const;
 
  void improve_approximation_to(const integer& q) const;
/*{\Mopl recomputes the approximation of \Mvar\, if necessary; 
  the resulting error of the |bigfloat| approximation satisfies
  $|x.get_bigfloat_error()| \leq 2^{-q}$
  % after the call |x.improve_approximation_to(q)|.
}*/
  void compute_with_precision(long k) const;
/*{\Mopl recomputes the |bigfloat| approximation of \Mvar, if necessary;
   each numerical operation is carried out with a mantissa length of $k$. 
   Note that here the size of the resulting |x.get_bigfloat_error()| 
   cannot be predicted in general. 
}*/
  void guarantee_relative_error(long k) const;
/*{\Mopl recomputes an approximation of \Mvar, if necessary; the relative error 
    of the resulting |bigfloat| approximation is less than $2^{-k}$, i.e., 
    $|x.get_bigfloat_error()| \leq \Lvert x \Lvert \cdot 2^{-k}$.
}*/

  static io_mode get_output_mode();

  static io_mode set_output_mode(io_mode m);

  static void output_as_dag(ostream& out, const real& x);
/*{\Xstatic writes the graph output of $x$.
}*/
  static void output_as_interval(ostream& out, const real& x);
/*{\Xstatic writes the graph output of $x$.
}*/
  static void output_as_expression(ostream& out, const real& x, bool use_sub_expr = true);
/*{\Xstatic writes a program that generates $x$.
}*/

  static real input_as_dag(istream& in = cin);
/*{\Xstatic reads the graph input of a real.
}*/

  friend __exportF ostream& operator<<(ostream& O, const real& x);
/*{\Mbinopfunc writes the closest interval that is known to contain $x$
    to the output stream $O$. Note that the exact representation
    of \Mvar is lost in the stream output. 
}*/

  friend __exportF istream& operator>>(istream& I, real& x); 
/*{\Mbinopfunc reads \Mvar number |x| from the output stream $I$
   in |double| format. Note that stream input is currently impossible 
   for more general types of |real|s. 
}*/

  
    friend __exportF real operator+(const real&,const real&);
    friend __exportF real operator-(const real&,const real&);
    friend __exportF real operator*(const real&,const real&);
    friend __exportF real operator/(const real&,const real&);
    friend __exportF real operator+(const real&);
    friend __exportF real operator-(const real&);

    friend __exportF real sqrt(const real& x);
  /*{\Mfunc $\sqrt{x}$
  }*/

    friend __exportF real root(const real& x, int d);
  /*{\Mfunc $\sqrt[d]{x}$, precondition: $d \geq 2$
  }*/

  #if (__GNUC__==3 && __GNUC_MINOR__ >=4) || (defined(_MSC_VER) && _MSC_VER < 1300)
  #define isolating_algorithm real::isolating_algorithm
  #elif defined(__SUNPRO_CC)
  #define isolating_algorithm USPENSKY
  #endif

  
  /*{\Mtext \textbf{Note:} The functions |real_roots| and |diamond| below are 
  all \emph{experimental} if they are applied to a polynomial which is not 
  square-free.}*/

/*
    friend __exportF int real_roots(const Polynomial& P, 
                  list<real>& roots, 
                  algorithm_type algorithm = isolating_algorithm, 
                  bool is_squarefree = true);
*/

  friend __exportF int real_roots(const Polynomial& P,
                  list<real>& roots,
                  algorithm_type algorithm,
                  bool is_squarefree);
  /*{\Mfunc returns all real roots of the polynomial $P$.
  }*/


/*
    friend __exportF int real_roots(const Polynomial& P, 
                  growing_array<real>& roots, 
                  algorithm_type algorithm = isolating_algorithm, 
                  bool is_squarefree = true);
*/

    friend __exportF int real_roots(const Polynomial& P,
                  growing_array<real>& roots, 
                  algorithm_type algorithm,
                  bool is_squarefree);
  /*{\Mfunc same as above.
  }*/

                  
    friend inline int real_roots(const int_Polynomial& iP, 
                   list<real>& roots, 
                   algorithm_type algorithm = isolating_algorithm, 
                   bool is_squarefree = true)
    {
      Polynomial P; real::convert_int_poly_to_real_poly(iP, P); 
      return real_roots(P, roots, algorithm, is_squarefree);
    }
  /*{\Mfunc returns all real roots of the polynomial |iP|.
  }*/

/*
    friend __exportF real diamond(int j, const Polynomial& P,             
                  algorithm_type algorithm = isolating_algorithm, 
                  bool is_squarefree = true);
*/

    friend __exportF real diamond(int j, const Polynomial& P,             
                  algorithm_type algorithm,
                  bool is_squarefree);
  /*{\Mfunc returns the $j$-th smallest real root of the polynomial $P$.
  }*/



/*
    friend __exportF real diamond(rational l, rational u, const Polynomial& P, 
                  algorithm_type algorithm = isolating_algorithm, 
                  bool is_squarefree = true);
*/

    friend __exportF real diamond(rational l, rational u, const Polynomial& P, 
                  algorithm_type algorithm, 
                  bool is_squarefree);
  /*{\Mfunc returns the real root of the polynomial $P$ which is in the
     isolating interval [l,u].
  }*/


/*
    friend __exportF real diamond_short(rational l, rational u, 
                           const Polynomial& P, 
                           algorithm_type algorithm = isolating_algorithm, 
                           bool is_squarefree = true);
*/

    friend __exportF real diamond_short(rational l, rational u, 
                           const Polynomial& P, 
                           algorithm_type algorithm, 
                           bool is_squarefree);
  /*{\Mfunc returns the real root of the polynomial $P$ which is in the
     isolating interval [l,u].\\
     \precond $(u-l) < 1/4$
  }*/

    friend inline real diamond(int j, const int_Polynomial& iP, 
                  algorithm_type algorithm = isolating_algorithm, 
                  bool is_squarefree = true)
    {
      Polynomial P; real::convert_int_poly_to_real_poly(iP, P); 
      return diamond(j, P, algorithm, is_squarefree);
    }
  /*{\Mfunc returns the $j$-th smallest real root of the polynomial |iP|.
  }*/

    friend inline real diamond(rational l, rational u, const int_Polynomial& iP, 
                  algorithm_type algorithm = isolating_algorithm, 
                  bool is_squarefree = true)
    {
      Polynomial P; real::convert_int_poly_to_real_poly(iP, P); 
      return diamond(l, u, P, algorithm, is_squarefree);
    }
  /*{\Mfunc returns the real root of the polynomial |iP| which is in the
     isolating interval [l,u].
  }*/

  
  #undef isolating_algorithm

   friend __exportF int compare(const real& x, const real& y);


  LEDA_MEMORY(real)

};

    __exportF int sign(const real& x);
    __exportF bool sign_is_known(const real& x);

    __exportF real sqrt(const real& x);

    __exportF int real_roots(const Polynomial& P,
                  list<real>& roots,
                  algorithm_type algorithm = real::isolating_algorithm,
                  bool is_squarefree = true);

    __exportF int real_roots(const Polynomial& P, 
                  growing_array<real>& roots, 
                  algorithm_type algorithm = real::isolating_algorithm, 
                  bool is_squarefree = true);

    __exportF real diamond(int j, const Polynomial& P,
                  algorithm_type algorithm = real::isolating_algorithm,
                  bool is_squarefree = true);

    __exportF real diamond(rational l, rational u, const Polynomial& P,
                  algorithm_type algorithm = real::isolating_algorithm,
                  bool is_squarefree = true);

    __exportF real diamond_short(rational l, rational u, const Polynomial& P,
                  algorithm_type algorithm = real::isolating_algorithm,
                  bool is_squarefree = true);


    __exportF int compare(const real& x, const real& y);







   inline const char* leda_tname(const real*)  {  return "real";  }

   inline bool operator<(const real& x,const real& y) { return compare(x,y) < 0; }
   inline bool operator<=(const real& x,const real& y) { return compare(x,y) <= 0; }
   inline bool operator>(const real& x,const real& y) { return compare(x,y) > 0; }
   inline bool operator>=(const real& x,const real& y) { return compare(x,y) >= 0; }
   inline bool operator==(const real& x,const real& y) { return compare(x,y) == 0; }
   inline bool operator!=(const real& x,const real& y) { return compare(x,y) != 0; }

    __exportF real operator +=(real& x,const real& y);
    __exportF real operator -=(real& x,const real& y); 
    __exportF real operator *=(real& x,const real& y);
    __exportF real operator /=(real& x,const real& y);

  
     __exportF real abs(const real& x);
  /*{\Mfunc absolute value of |x|
  }*/
     __exportF real sqr(const real& x); 
  /*{\Mfunc square of |x| 
  }*/
     __exportF real dist(const real& x,const real& y); 
  /*{\Mfunc euclidean distance of point (x,y) to the origin
  }*/ 
     __exportF real powi(const real& x, int n); 
  /*{\Mfunc $x^n$, i.e., |n|.th power of |x| 
  }*/
     __exportF integer floor(const real& x);
  /*{\Mfunc returns the largest integer smaller than or equal to $x$.
  }*/
     __exportF integer ceil(const real& x);
  /*{\Mfunc returns the smallest integer greater than or equal to $x$.
  }*/
     __exportF rational small_rational_between(const real& x, const real& y);
  /*{\Mfunc returns a rational number between $x$ and $y$ with the smallest
          available denominator. Note that the denominator does not need to
          be strictly minimal over all possible rationals.
  }*/
     __exportF rational small_rational_near(const real& x, double eps);
  /*{\Mfunc returns |small_rational_between(x-eps, x+eps)|.
  }*/

  /*{\Mimplementation

  A \Mtype\ is represented by the expression which defines it and an |interval|
  inclusion $I$ that contains the exact value of the \Mtype.
  The arithmetic operators $+, -, *, \, \sqrt[d]{}$ take constant time.
  When the sign of a \Mtype\ number needs to be determined, the data type first
  computes a number $q$, if not already given as an argument to |sign|, such that 
  $\Lvert \mbox{\Mvar} \Lvert \leq 2^{-q}$ implies $x = 0$.
  The bound $q$ is computed as described in \cite{S-04}.
  %The data type then computes a bigfloat approximation $xnum$ for \Mvar\ with
  %error bound $xerr \leq 2^{-q}$. 
  %The sign of $xnum$ is then returned as the sign of \Mvar.
  Using |bigfloat| arithmetic, the data type then computes an interval $I$ of
  maximal length $2^{-q}$ that contains \Mvar.
  If $I$ contains zero, then \Mvar itself is equal to zero.
  Otherwise, the sign of any point in $I$ is returned as the sign of \Mvar.

  Two shortcuts are used to speed up the computation of the sign. 
  Firstly, if the initial |interval| approximation already suffices to determine 
  the sign, then no |bigfloat| approximation is computed at all.
  Secondly, the |bigfloat| approximation is first computed only with small 
  precision. The precision is then roughly doubled until either the sign 
  can be decided (i.e., if the current approximation interval does not 
  contain zero) or the full precision $2^{-q}$ is reached.
  This procedure makes the |sign| computation of a |real| number |x|
  {\em adaptive} in the sense that the running time of the |sign|
  computation depends on the complexity of |x|.
  }*/

  /*{\Mexample

  We give two typical examples for the use of the data type \Mtype that arise 
  in Computational geometry. We admit that a certain knowledge about 
  Computational geometry is required for their full understanding.
  The examples deal with the Voronoi diagram of line segments and 
  the intersection of line segments, respectively. 

  The following incircle test is used in the computation of Voronoi diagrams 
  of line segments \cite{Burnikel:thesis, BMS:ESA}.
  For $i$, $1 \leq i \leq 3$, let $l_i : a_i x + b_i y + c_i = 0$ be a line
  in two--dimensional space and let $p = (0,0)$ be the origin.
  In general, there are two circles passing through $p$ and touching $l_1$ and
  $l_2$.
  The centers of these circles have homogeneuos coordinates $(x_v,y_v,z_v)$, where
  \begin{eqnarray*}
  x_v & = & a_1 c_2 +a_2 c_1 \pm\mbox{sign}(s)\sqrt{2 c_1 c_2 (\sqrt{N} + D)} \\
  y_v & = & b_1 c_2 +b_2 c_1 \pm\mbox{sign}(r)\sqrt{2 c_1 c_2 (\sqrt{N} - D)} 
  \\[2mm]
  z_v & = & \sqrt{N} - a_1 a_2 - b_1 b_2
  \end{eqnarray*}
  and 
  $$
  \begin{array}{cccccc}
  s & = & b_1 D_2 - b_2 D_1, &
  N & = & (a_1^{2} + b_1^{2}) (a_2^{2} + b_2^{2}) \\[3mm]
  r & = & a_1 D_2 - a_2 D_1, &
  D & = & a_1 a_2 - b_1 b_2.
  \end{array}
  $$
  Let us concentrate on one of these (say, we take the plus sign in both cases).
  The test whether $l_3$ intersects, touches or misses the circle amounts to
  determining the sign of
  \begin{eqnarray*}
  E := dist^{2}(v,l_3) - dist^{2}(v,p) = 
       \frac{(a_3 x_v + b_3 y_v + c_3)^2}{a_3^2 + b_3^2} - (x_v^2 + y_v^2).
  \end{eqnarray*}
  The following program computes the sign of 
  $\tilde{E} := (a_3^2 + b_3^2) \cdot E$ using our data type \Mtype. 

  \def\rsp{\hspace*{5pt} {\bf real}}
  \def\sp{\hspace*{5pt}}
  \def\lsp{\hspace*{14pt}}

  \begin{quote}
  {\small
  {\bf int} {\sc incircle}(
  {\bf real} $a_1$, {\bf real} $b_1$, {\bf real} $c_1$,
  {\bf real} $a_2$, {\bf real} $b_2$, {\bf real} $c_2$, 
  {\bf real} $a_3$, {\bf real} $b_3$, {\bf real} $c_3$
  ) \\
  \{\\
  \rsp\ $RN = \mbox{\small sqrt}((a_1*a_1+b_1*b_1)*(a_2*a_2+b_2*b_2))$;\\
  \rsp\ $RN_1 = \mbox{\small sqrt}(a_1*a_1+b_1*b_1);$\\
  \rsp\ $RN_2 = \mbox{\small sqrt}(a_2*a_2+b_2*b_2);$\\
  \rsp\ $A = a_1 * c_2 + a_2 * c_1;$\\
  \rsp\ $B = b_1 * c_2 + b_2 * c_1;$\\
  \rsp\ $C = 2 * c_1 * c_2;$\\
  \rsp\ $D = a_1 * a_2 - b_1 * b_2;$\\
  \rsp\ $s = b_1 * RN_2 - b_2 * RN_1;$\\
  \rsp\ $r = a_1 * RN_2 - a_2 * RN_1;$\\
  \hspace*{5pt} {\bf int} $sign_x = \mbox{sign}(s);$\\
  \hspace*{5pt} {\bf int} $sign_y = \mbox{sign}(r);$\\
  \rsp\ $x_v = A + sign_x * \mbox{\small sqrt}(C*(RN+D));$\\
  \rsp\ $y_v = B - sign_y * \mbox{\small sqrt}(C*(RN-D));$\\
  \rsp\ $z_v = RN - (a_1*a_2+b_1*b_2);$\\
  \rsp\ $P = a_3 * x_v + b_3 * y_v + c_3 * z_v;$\\
  \rsp\ $D_3^2= a_3 * a_3 + b_3 * b_3;$\\
  \rsp\ $R^2= x_v * x_v + y_v * y_v;$\\
  \rsp\ $E = P*P - D_3^2 * R^2;$\\
  \hspace*{5pt} {\bf return} $\mbox{sign}(E)$;\\
  %\hspace*{5pt} {\bf return} $E.\mbox{sign}(24*k+26)$;\\
  \}
  }

  \end{quote}
  We can make the above program more efficient if all coefficients 
  $a_i, b_i$ and $c_i$, $1 \leq i \leq 3$, are $k$ bit integers, i.e., 
  integers whose absolute value is bounded by $2^k -1$. 
  In \cite{Burnikel:thesis,BMS:ESA} we showed that for $\tilde{E} \neq 0$ 
  we have $\Lvert\tilde{E}\Lvert \geq 2^{-24k-26}$.
  Hence we may add a parameter |int k| in the above program and replace
  the last line by
  $${\bf return} \; E.\mbox{sign}(24*k+26).$$
  Without this assistance, |real|s automatically compute a weaker bound of
  $\Lvert\tilde{E}\Lvert \geq 2^{-56k-161}$ for $\tilde{E} \neq 0$ 
  by \cite{BFMS:SODA97}.

  We turn to the line segment intersection problem next. 
  Assume that all endpoints have $k$--bit integer homogeneous coordinates.
  This implies that the intersection points have homogeneous coordinates 
  $(X,Y,W)$ where $X,Y$ and $W$ are (4 k + 3) - bit integers.
  The Bentley--Ottmann plane sweep algorithm for segment intersection
  \cite{Me-Naeher:sweep} needs to sort points by their $x$--coordinates, i.e.,
  to compare fractions $X_1 / W_1$ and $X_2 / W_2$ where $X_1, X_2, W_1, W_2$
  are as above.
  This boils down to determining the sign of the $8 k + 7$ bit integer 
  $X_1 * W_2 - X_2 * W_1$.
  If all variables $X_i, W_i$ are declared |real| then their sign
  test will be performed quite efficiently.
  First, an |interval| approximation is computed and then, if necessary,
  |bigfloat| approximations of increasing precision.
  In many cases, the |interval| approximation already determines the sign.
  In this way, the user of the data type |real| gets nearly the efficiency 
  of a hand-coded floating point filter \cite{Fortune-vWijk,Me-Naeher:IFIP94} 
  without any work on his side.
  This is in marked contrast to \cite{Fortune-vWijk,Me-Naeher:IFIP94} and
  will be incorporated into \cite{Me-Naeher:sweep}.

  %-----------------------------------------------------------------------------
  % real_header.w
  %-----------------------------------------------------------------------------

  }*/


     


  
  inline int has_root(const Polynomial& P, const rational& l, const rational& r)
  {
       int s = sign(P.evaluate(l)) * sign(P.evaluate(r)); 
       return (s);
  }
  
  
  bool refine(Polynomial& P, rational& a, rational& b, bool& squarefree);
  bool refine_diff(Polynomial& P, Polynomial& Q);
  bool refine_interval(Polynomial P, Polynomial Q, rational& a, rational& b);
  

#if LEDA_ROOT_INCL_ID == 600259
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif // _LEDA_REAL_H


