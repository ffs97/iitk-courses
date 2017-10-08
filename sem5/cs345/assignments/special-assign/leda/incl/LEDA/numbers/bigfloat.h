/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bigfloat.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BIGFLOAT_H
#define LEDA_BIGFLOAT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600242
#include <LEDA/internal/PREAMBLE.h>
#endif


/*{\Manpage {bigfloat} {} {The data type bigfloat}

}*/

#include <LEDA/system/basic.h>
#include <LEDA/system/assert.h>

#include <LEDA/numbers/integer.h>



#ifdef BIGFLOAT_CHECKS_ON
#include <LEDA/numbers/digit_vector.h>
#endif

LEDA_BEGIN_NAMESPACE

enum rounding_modes 
{
  TO_NEAREST = 0, TO_ZERO = 1, TO_P_INF = 2, TO_N_INF = 3, TO_INF = 4, EXACT = 5
};


#define MAXEXP  (1L<<28)
#define MINEXP  (-MAXEXP)
#define MAXPREC (1L<<28)

struct __exportC bigfloat_rep 
{
  atomic_counter count;

  sz_t          size;
  sz_t          used;
  sz_t          bitlength;

  int             sign;
  long          exponent;
  integer*    exp_ptr;
  digit*        vec;
  digit        _vec[1];

  bigfloat_rep() : count(1) { exp_ptr = 0; }
  unsigned long ref_plus()  { return ++count; }
  unsigned long ref_minus() { return --count; }

};

class __exportC rational;

class __exportC bigfloat 
{ 

/*{\Mdefinition
In general a |bigfloat| is given by two integers $s$ and $e$ where $s$ is the
significant and $e$ is the exponent.
The tuple $(s,e)$ represents the real number $$s \cdot 2^e.$$
In addition, there are the special |bigfloat| values |NaN| (not a number),
|pZero|, |nZero| ($= +0, -0$), and |pInf|, |nInf| ($= +\infty, -\infty$).
These special values behave as defined by the IEEE floating 
point standard.
In particular,
$\frac{5}{+0}=\infty$, $\frac{-5}{+0}=-\infty$, $\infty+1=\infty$, 
$\frac{5}{\infty}=+0$, $+\infty+(-\infty)=NaN$ and $0 \cdot \infty = NaN$.

Arithmetic on |bigfloat|s uses two parameters:
The precision |prec| of the result (in number of binary digits) and the 
rounding mode |mode|.
Possible rounding modes are:
\begin{itemize}
\item |TO_NEAREST|: round to the closest representable value
\item |TO_ZERO|: round towards zero
\item |TO_INF|: round away from zero
\item |TO_P_INF|: round towards $+\infty$
\item |TO_N_INF|: round towards $-\infty$
\item |EXACT|: compute exactly for |+,-,*| and round to nearest otherwise
\end{itemize}
Operations $+$, $-$, $*$ work as follows. 
First, the exact result |z| is computed.
If the rounding mode is |EXACT| then |z| is the result of the operation. 
Otherwise, let |s| be the significant of the result; |s| is rounded to |prec|
binary places as dictated by |mode|. 
Operations $/$ and $\sqrt{\hspace{1ex}}$ work accordingly except that
|EXACT| is treated as |TO_NEAREST|.

The parameters |prec| and |mode| are either set directly for a single
operation or else they are set globally for every operation to follow. 
The default values are 53 for |prec| and |TO_NEAREST| for |mode|.
}*/

public:

  
  static const bigfloat pZero;
  static const bigfloat nZero;
  static const bigfloat nInf;
  static const bigfloat pInf;
  static const bigfloat NaN;


  enum output_modes  { BIN_OUT=0,DEC_OUT=1,HEX_OUT=2 };

  enum special_values 
  {  NOT_VAL=0,PZERO_VAL=1,NZERO_VAL=-1,PINF_VAL=2,NINF_VAL=-2,NAN_VAL=3};

  private: 

  
  static sz_t global_prec;           // the default binary  arithmetic precision
  static sz_t global_output_prec;    // the default decimal output precision
  static sz_t global_input_prec;     // the default binary  input precision
  static rounding_modes round_mode;  // the default rounding mode
public:
  static bool dbool;                 // a default boolean argument
private:
  static output_modes output_mode;   // the default output mode
#ifdef BIGFLOAT_CHECKS_ON
  static sz_t obj_count;  // check that all objects are destructed
#endif


  bigfloat_rep* PTR;      // data for non-special bigfloat
  special_values special; // data for special bigfloat

  
static bigfloat_rep* new_rep(sz_t size);          // create new bigfloat rep
static void          delete_rep(bigfloat_rep *p); // delete bigfloat rep
static bigfloat_rep* copy_rep(bigfloat_rep*);     // copy bigfloat rep 
static bigfloat_rep* copy_rep(integer_rep*);      // copy bigfloat rep

inline static bigfloat_rep* allocate_bytes(sz_t s);
inline static          void deallocate_bytes(bigfloat_rep* p, sz_t s);

GenPtr copy() const { PTR->ref_plus(); return PTR; }
int    refs() const { return PTR->count; }

bigfloat(bigfloat_rep* p) { 

  PTR = p; 
  if (PTR->sign==0)
    special=bigfloat::PZERO_VAL;
  else
    special=bigfloat::NOT_VAL;
  bigfloat::normalize_rep(PTR);

}

void   clear()      { 
#ifdef BIGFLOAT_CHECKS_ON
  if (PTR==NULL)
#ifdef LINK_WITH_LEDA_BIGFLOAT
    cout << "error: destructor called twice for same object\n";
#else
    num_error(1,"error: destructor called twice for same object");
#endif
#endif

#ifdef LINK_WITH_LEDA_BIGFLOAT
  // the folllowing code is safe against the linker bug:
  if (PTR && PTR->ref_minus() == 0) 
  { bigfloat::delete_rep(PTR); 
    PTR=NULL;
   }
#else
  if (PTR->ref_minus() == 0) bigfloat::delete_rep(PTR); 
#endif
}





friend int  sign_of_special_value (const bigfloat &x);
static void normalize_long_exponent(bigfloat_rep* z, long exp);


static inline void normalize_rep (bigfloat_rep* z) {

  if (z->used && (z->vec[0]==0)) {
    sz_t zeros;
    for (zeros=1;zeros<z->used;zeros++)
      if (z->vec[zeros])
        break;
    z->vec+=zeros;
    z->exponent+=long(zeros);
    z->used-=zeros;
    z->bitlength -= (zeros<<WORD_LENGTH_LOG);
  }
  long exp = z->exponent;
  if (z->exp_ptr!=NULL || 
      exp > MAXEXP || 
      exp < MINEXP ) 
    normalize_long_exponent(z,exp);

}



public:

/*{\Mcreation x }*/

/*{\Mtext 
A |bigfloat| may be constructed from data types |double|, |long|, |int| and
|integer|, without loss of accuracy.
In addition, an instance of type |bigfloat| can be created as follows.
}*/

  
  ~bigfloat() { clear(); }  

  bigfloat();
  bigfloat(double);
  bigfloat(long);
  bigfloat(int);
  bigfloat(const integer&);
  bigfloat(const bigfloat& x);
  bigfloat(special_values sp);

  bigfloat(const integer &s,const integer &e);
  /*{\Mcreate introduces a variable |\Mvar| of type \Mtype 
     and initializes it to $s \cdot 2^e$ 
    }*/

  double to_double() const;
  /*{\Mop returns the double value next to |\Mvar|
          (i.e. rounding mode is always |TO_NEAREST|).
  }*/
  
  double to_double(bool& is_double) const
  { double err; double d = to_double(err, TO_NEAREST); is_double = (err == 0); return d; }
  /*{\Mop as above, but also returns in |is_double| whether the
          conversion was exact.
  }*/

  double to_double(double& abs_err, rounding_modes m = TO_NEAREST) const;
  /*{\Mop as above, but with more flexibility:
          The parameter |m| specifies the rounding mode.
          For the returned value $d$, we have 
          $ \Lvert |\Mvar| - d \Lvert \leq |abs_err| $.
          (|abs_err| is zero iff the conversion is exact and the returned value is finite.)
  }*/

  double to_double(rounding_modes m) const
  { double dummy; return to_double(dummy, m); }
  /*{\Mop as above, but does not return an error bound.
  }*/

  rational to_rational() const;
  /*{\Mop converts |\Mvar| into a number of type |rational|.
  }*/

  sz_t get_significant_length(void) const { return PTR->bitlength; }
  /*{\Mop returns the length of the significant of |\Mvar|.
  }*/
  sz_t get_effective_significant_length(void) const;
  /*{\Mop returns the length of the significant of |\Mvar| without trailing zeros.
  }*/
  integer get_exponent(void) const;
  /*{\Mop returns the exponent of |\Mvar|.
  }*/
  integer get_significant(void) const { return integer(PTR->used,PTR->vec,PTR->sign); }
  /*{\Mop returns the significant of |\Mvar|.
  }*/
 
  inline digit operator[] (sz_t i) const;
  // quick access to PTR->vec

  
  static sz_t set_precision(sz_t p); 
  /*{\Mstaticl sets the global arithmetic precision to |p| binary digits
     and returns the old value
  }*/
  static sz_t get_precision(); 
  /*{\Mstaticl returns the currently active global arithmetic precision
  }*/
  static sz_t set_output_precision(sz_t d); 
  /*{\Mstaticl sets the precision of |bigfloat| output to |d| decimal digits
     and returns the old value
  }*/
  static sz_t set_input_precision(sz_t p); 
  /*{\Mstaticl sets the precision of |bigfloat| input to |p| binary digits
     and returns the old value
  }*/
  static rounding_modes set_rounding_mode(rounding_modes m); 
  /*{\Mstaticl sets the global rounding mode to m and returns the old 
     rounding mode
  }*/  
  static rounding_modes get_rounding_mode(); 
  /*{\Mstaticl returns the currently active global rounding mode
  }*/ 
   static output_modes set_output_mode(output_modes o_mode);
  /*{\Mstaticl sets the output mode to |o_mode| and returns the old 
      output mode
  }*/
/*
  static bool& get_dbool() { return bigfloat::dbool; }
*/


  
  /*{\Mtext
  A |bigfloat| |\Mvar| can be rounded by the call
  |round(\Mvar,prec,mode,is_exact)|. %,bias)
  %Here bias is an optional integer variable taking values in $\{-1,0,+1\}$, 
  %with default value $0$.
  %If \Mvar is nonzero, the function |round| rounds 
  %\Mvar $+$ bias $\cdot \epsilon$, 
  %where $\epsilon$ is a positive infinitesimal, with prec binary digits 
  %in rounding mode mode.
  The optional boolean variable |is_exact| is set to true if and only if the
  rounding operation did not change the value of |\Mvar|. 
  % and bias=0. 
  }*/


/*
  friend __exportF bigfloat round(const bigfloat& y,sz_t digits=0,
			rounding_modes mode=bigfloat::get_rounding_mode(),
                 	bool &is_exact=bigfloat::dbool,int bias=0);
*/

  friend __exportF bigfloat round(const bigfloat& y,sz_t digits,
                        rounding_modes mode, bool &is_exact,int bias);


/*
  static integer to_integer(bigfloat x, 
    rounding_modes rmode = TO_NEAREST, 
    bool& is_exact=bigfloat::dbool);
*/

  // changed to member function by s.n. (July 2001)

  integer to_integer(rounding_modes rmode = TO_NEAREST, 
                     bool& is_exact=bigfloat::dbool) const;
  /*{\Mfuncl returns the integer value next to |\Mvar| 
     (in the given rounding mode)
  }*/
   
/*
  friend __exportF integer to_integer(const bigfloat& x,  
    rounding_modes rmode = TO_NEAREST, bool& is_exact=bigfloat::dbool);
*/

  friend __exportF integer to_integer(const bigfloat& x,
    rounding_modes rmode, bool& is_exact);
  /*{\Mfuncl returns |x.to_integer(...)|. }*/

  static bigfloat from_rational(const rational& r, 
                                sz_t prec = bigfloat::get_precision(),
                                rounding_modes rmode = TO_NEAREST,
                                bool& is_exact = bigfloat::dbool);

  /*{\Moperations 1.7 3.5 
  }*/

  /*{\Mtext
The arithmetical operators |+|, |-|, |*|, |/|, |+=|, |-=|, |*=|, |/=|, 
$\mathit{sqrt}$, the comparison operators $<$, $\leq$, $>$, $\geq$ , $=$, 
$\neq$ and the stream operators  are available.
Addition, subtraction, multiplication, division, square root and power are 
implemented by the functions |add|, |sub|, |mul|, |div|, |sqrt| and |power|
respectively. For example, the call 
\begin{center} |add(x, y, prec, mode, is_exact)| \end{center}
computes the sum of \Mtype{s} |x| and |y| with |prec| binary digits, in
rounding mode |mode|, and returns it.
The optional last parameter |is_exact| is a boolean variable that is set to
|true| if and only if the returned bigfloat exactly equals the sum of 
|x| and |y|. The parameters |prec| and |mode| are also optional 
and have the global default values |global_prec| and |round_mode| 
respectively, that is, the three calls |add(x, y, global_prec, round_mode)|, 
|add(x, y, global_prec)|, and |add(x, y)| are all equivalent.
The syntax for functions |sub|, |mul|, |div|, and |sqrt| is analogous.

The operators $+$, $-$, $*$, and $/$ are implemented by their 
counterparts among the functions |add|, |sub|, |mul| and |div|. 
For example, the call $x+y$ is equivalent to |add(x,y)|.
}*/

  static bigfloat special_addsub(const bigfloat&x,const bigfloat&y,int y_sign,bool& is_exact);
    // handle special cases of addition and subtraction

  static bigfloat longexp_addsub(
     const bigfloat&x,const bigfloat&y,int sign_y,
     sz_t prec,rounding_modes mode,bool &is_exact);
    // handle addition of numbers with long exponents

  static bigfloat_rep*  exact_unsigned_add(bigfloat_rep*a,bigfloat_rep*b);
    // performs exact unsigned addition of bigfloats
  static bigfloat_rep*  exact_unsigned_sub(bigfloat_rep*a,bigfloat_rep*b);
    // performs exact unsigned subtraction of bigfloats

  // the main arithmetic functions:

/*
  friend __exportF bigfloat add(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  friend __exportF bigfloat sub(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  friend __exportF bigfloat mul(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(), 
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  friend __exportF bigfloat sqr(const bigfloat&,
                      sz_t prec=bigfloat::get_precision(), 
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  friend __exportF bigfloat div(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  friend __exportF bigfloat sqrt_bf(const bigfloat&,
                       sz_t prec=bigfloat::get_precision(),
                       rounding_modes mode=bigfloat::get_rounding_mode(), 
                       bool &is_exact=bigfloat::dbool,
                       const bigfloat& start=bigfloat::pZero);
  
  friend __exportF bigfloat power(const bigfloat& x, unsigned long n, 
                      sz_t prec = bigfloat::get_precision(), 
                      rounding_modes mode = bigfloat::get_rounding_mode(),
                      bool &is_exact = bigfloat::dbool);

*/

  friend __exportF bigfloat add(const bigfloat&,const bigfloat&,
                      sz_t prec, rounding_modes mode, bool &is_exact);

  friend __exportF bigfloat sub(const bigfloat&,const bigfloat&,
                      sz_t prec, rounding_modes mode, bool &is_exact);

  friend __exportF bigfloat mul(const bigfloat&,const bigfloat&,
                      sz_t prec, rounding_modes mode, bool &is_exact);

  friend __exportF bigfloat sqr(const bigfloat&,
                      sz_t prec, rounding_modes mode, bool &is_exact);

  friend __exportF bigfloat div(const bigfloat&,const bigfloat&,
                      sz_t prec, rounding_modes mode, bool &is_exact);

  friend __exportF bigfloat sqrt_bf(const bigfloat&,
                       sz_t prec, rounding_modes mode, bool &is_exact,
                       const bigfloat& start);

  friend __exportF bigfloat power(const bigfloat& x, unsigned long n,
                      sz_t prec, rounding_modes mode, bool &is_exact);



  friend __exportF bigfloat operator+(const bigfloat &a,const bigfloat &b);
  friend __exportF bigfloat operator-(const bigfloat &a,const bigfloat &b);  
  friend __exportF bigfloat operator*(const bigfloat &a,const bigfloat &b);
  friend __exportF bigfloat operator/(const bigfloat &a,const bigfloat &b);
  friend __exportF bigfloat operator-(const bigfloat&);

  friend __exportF bigfloat operator+=(bigfloat &a,const bigfloat &b);
  friend __exportF bigfloat operator-=(bigfloat &a,const bigfloat &b);
  friend __exportF bigfloat operator*=(bigfloat &a,const bigfloat &b);   
  friend __exportF bigfloat operator/=(bigfloat &a,const bigfloat &b); 
 
  friend __exportF bool operator == (const bigfloat&,const bigfloat&);
  friend __exportF bool operator > (const bigfloat&,const bigfloat&);
  friend __exportF bool operator != (const bigfloat &a,const bigfloat &b);
  friend __exportF bool operator >= (const bigfloat &a,const bigfloat &b);
  friend __exportF bool operator < (const bigfloat &a,const bigfloat &b);
  friend __exportF bool operator <= (const bigfloat &a,const bigfloat &b);

  static int compare(const bigfloat& x, const bigfloat& y);
  static int abs_compare(const bigfloat& x, const bigfloat& y);

  bigfloat& operator=(const bigfloat& x);

  
  friend __exportF bool isNaN(const bigfloat &x);
  /*{\Mfunc returns |true| if and only if |\Mvar| is in special state |NaN|
  }*/
  friend __exportF bool isnInf(const bigfloat &x);
  /*{\Mfunc returns |true| if and only if |\Mvar| is in special state |nInf|
  }*/ 
  friend __exportF bool ispInf(const bigfloat &x);
  /*{\Mfunc returns |true| if and only if |\Mvar| is in special state |pInf|
  }*/
  friend __exportF bool isnZero(const bigfloat &x);
  /*{\Mfunc returns |true| if and only if |\Mvar| is in special state |nZero|
  }*/
  friend __exportF bool ispZero(const bigfloat &x);
  /*{\Mfunc returns |true| if and only if |\Mvar| is in special state |pZero|
  }*/
  friend __exportF bool isZero(const bigfloat &x); 
  /*{\Mfunc returns |true| if and only if |ispZero(\Mvar)| or |isnZero(\Mvar)|  
  }*/
  friend __exportF bool isInf(const bigfloat &x);
  /*{\Mfunc returns |true| if and only if |ispInf(\Mvar)| or |isnInf(\Mvar)|
  }*/

  friend inline bool isSpecial(const bigfloat &x) { return x.special != 0; }
  /*{\Mfunc returns |true| if and only if |\Mvar| is in a special state
  }*/

  
  friend __exportF int sign(const bigfloat &x);
  /*{\Mfunc returns the sign of |\Mvar|.
  }*/
  friend __exportF bigfloat abs(const bigfloat &x);
  /*{\Mfunc returns the absolute value of |\Mvar| 
  }*/
  friend __exportF bigfloat ipow2(const integer &p); 
  /*{\Mfunc returns $2^p$
  }*/
  friend __exportF integer ilog2(const bigfloat &x);
  /*{\Mfunc returns the binary logarithm of abs(|\Mvar|), rounded up to the
    next integer. \precond |\Mvar| $\neq 0$
  }*/
  friend __exportF integer ceil(const bigfloat &x);
  /*{\Mfunc returns |\Mvar|, rounded up to the next integer 
  }*/
  friend __exportF integer floor(const bigfloat &x);
  /*{\Mfunc returns |\Mvar|, rounded down to the next integer 
  }*/
/*
  friend __exportF bigfloat sqrt_d(const bigfloat& x, sz_t p, int d=2); 
*/

  friend __exportF bigfloat sqrt_d(const bigfloat& x, sz_t p, int d); 
  /*{\Mfunc returns $\sqrt[d]{x}$, with relative error $\leq 2^{-p}$
            but not necessarily exactly rounded to $p$ binary digits
  }*/

  
  string to_string(sz_t dec_prec=global_output_prec) const; 
  /*{\Mop returns the decimal representation of |\Mvar|, rounded 
      to a decimal precision of |dec_prec| decimal places.  }*/

  bigfloat& from_string(string s, sz_t bin_prec=global_input_prec);
  /*{\Mop returns an approximation of the decimal number given by 
      the string |s| by a |bigfloat| that is accurate up to |bin_prec| 
      binary digits}*/

  friend __exportF ostream& operator << (ostream &os,const bigfloat &x); 
  /*{\Mbinopfuncl writes |\Mvar| to output stream |os|
  }*/
  friend __exportF istream& operator >> (istream &is,bigfloat &x); 
  /*{\Mbinopfuncl reads |\Mvar| from input stream |is| in decimal format
  }*/

};



 __exportF bigfloat round(const bigfloat& y,sz_t digits=0,
                           rounding_modes mode=bigfloat::get_rounding_mode(),
                           bool &is_exact=bigfloat::dbool,int bias=0);

  __exportF integer to_integer(const bigfloat& x,
                               rounding_modes rmode = TO_NEAREST,
                               bool& is_exact=bigfloat::dbool);

  __exportF bigfloat add(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  __exportF bigfloat sub(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  __exportF bigfloat mul(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  __exportF bigfloat sqr(const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  __exportF bigfloat div(const bigfloat&,const bigfloat&,
                      sz_t prec=bigfloat::get_precision(),
                      rounding_modes mode=bigfloat::get_rounding_mode(),
                      bool &is_exact=bigfloat::dbool);

  __exportF bigfloat sqrt_bf(const bigfloat&,
                       sz_t prec=bigfloat::get_precision(),
                       rounding_modes mode=bigfloat::get_rounding_mode(),
                       bool &is_exact=bigfloat::dbool,
                       const bigfloat& start=bigfloat::pZero);

  __exportF bigfloat power(const bigfloat& x, unsigned long n,
                      sz_t prec = bigfloat::get_precision(),
                      rounding_modes mode = bigfloat::get_rounding_mode(),
                      bool &is_exact = bigfloat::dbool);

  __exportF bigfloat sqrt_d(const bigfloat& x, sz_t p, int d=2);



  __exportF bigfloat operator+(const bigfloat &a,const bigfloat &b);
  __exportF bigfloat operator-(const bigfloat &a,const bigfloat &b);
  __exportF bigfloat operator*(const bigfloat &a,const bigfloat &b);
  __exportF bigfloat operator/(const bigfloat &a,const bigfloat &b);
  __exportF bigfloat operator-(const bigfloat&);

  __exportF bigfloat operator+=(bigfloat &a,const bigfloat &b);
  __exportF bigfloat operator-=(bigfloat &a,const bigfloat &b);
  __exportF bigfloat operator*=(bigfloat &a,const bigfloat &b);
  __exportF bigfloat operator/=(bigfloat &a,const bigfloat &b);

  __exportF bool operator == (const bigfloat&,const bigfloat&);
  __exportF bool operator > (const bigfloat&,const bigfloat&);
  __exportF bool operator != (const bigfloat &a,const bigfloat &b);
  __exportF bool operator >= (const bigfloat &a,const bigfloat &b);
  __exportF bool operator < (const bigfloat &a,const bigfloat &b);
  __exportF bool operator <= (const bigfloat &a,const bigfloat &b);


  __exportF bool isNaN(const bigfloat &x);
  __exportF bool isnInf(const bigfloat &x);
  __exportF bool ispInf(const bigfloat &x);
  __exportF bool isnZero(const bigfloat &x);
  __exportF bool ispZero(const bigfloat &x);
  __exportF bool isZero(const bigfloat &x);
  __exportF bool isInf(const bigfloat &x);

  __exportF int sign(const bigfloat &x);
  __exportF bigfloat abs(const bigfloat &x);
  __exportF bigfloat ipow2(const integer &p);
  __exportF integer ilog2(const bigfloat &x);
  __exportF integer ceil(const bigfloat &x);
  __exportF integer floor(const bigfloat &x);
  __exportF bigfloat sqrt_d(const bigfloat& x, sz_t p, int d);



/*
inline const bigfloat& min(const bigfloat& a, const bigfloat& b) { return (a<b) ? a : b; }
inline const bigfloat& max(const bigfloat& a, const bigfloat& b) { return (a>b) ? a : b; }
*/


//deprecated
inline double to_double(const bigfloat &x) { return x.to_double(); }


inline bigfloat::bigfloat(const bigfloat& x)  {
 
 PTR = x.PTR;  
 PTR->ref_plus(); 

 special = x.special;

#ifdef BIGFLOAT_CHECKS_ON
 if (x.PTR->used>0)
   assert (x.PTR->bitlength==vecLength(x.PTR->vec,x.PTR->used));
#endif

}

inline digit bigfloat::operator[](sz_t i) const {
#ifdef BIGFLOAT_PRIV_CHECKS_ON
  assert (i<=PTR->used);
#endif
  return (digit) PTR->vec[i];
}

inline int compare(const bigfloat &x, const bigfloat &y)  
{  return sign(x-y); }

inline const char* leda_tname(const bigfloat*)  {  return "bigfloat";  }


#if defined(__DMC__)
inline bigfloat sqrt(const bigfloat& x) { return sqrt_bf(x); }
#else
inline bigfloat sqrt(const bigfloat& x,
                     sz_t prec=bigfloat::get_precision(),
                     rounding_modes mode=bigfloat::get_rounding_mode(), 
                     bool &is_exact=bigfloat::dbool,
                     const bigfloat& start=bigfloat::pZero)
{ return sqrt_bf(x,prec,mode,is_exact,start); }
#endif

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600242
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
