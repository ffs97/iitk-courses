/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  residual.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RESIDUAL_H
#define LEDA_RESIDUAL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600244
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/numbers/integer.h>

#include <LEDA/internal/std/math.h>
#include <LEDA/internal/std/fstream.h>

LEDA_BEGIN_NAMESPACE


#if defined(RESIDUAL_CHECK_PRECONDITIONS) \
 || defined(RESIDUAL_CHECK_POSTCONDITIONS)
#include <LEDA/system/assert.h>
#endif

#define ROUNDING_INDEPENDENCE_REQUIRED

#define PRIMELENGTH 26
#define LAGRANGE_SIZE 150
#define COMPILE_TABLESIZE 2000
#define MAX_TABLESIZE 40000 


class __exportC residual_sequence
{
public:

  struct rep // contains the main data of an instance
  {
    unsigned int count;  // reference counting
    int size;		 // size of the array mod
    double mod[1];	 // the array of moduli
  };

  rep* PTR;
  
  static inline rep* new_rep(int n);
  static inline void delete_rep(rep*& PTR);
  static inline void copy_rep(rep* source, rep* target);
  static inline void copy_rep(rep* source, rep* target, int num);
  inline void resize_rep(int n);
  residual_sequence(rep* ptr) { PTR = ptr; }

  residual_sequence() { PTR = NULL; }
  inline residual_sequence(int n) { PTR = new_rep(n); }
  inline ~residual_sequence() { if (PTR) delete_rep(PTR); }
  residual_sequence& operator=(const residual_sequence& x); 
  residual_sequence(const residual_sequence& x);
  
  inline int size() const 
   { if (PTR) return PTR->size; else return 0; }
  inline double operator[](int i) const;
  inline double& operator[](int i);

  friend class residual_matrix; 
};


inline residual_sequence::rep* residual_sequence::new_rep(int n)
{
  int b = sizeof(rep)+(n-1)*sizeof(double);
  rep* ptr = (rep*)std_memory.allocate_bytes(b);
  //rep* ptr = (rep*)new char[b];
  ptr->size = n;
  ptr->count = 1;
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  for (int i=0;i<ptr->size;i++)
    ptr->mod[i]=-1;
#endif
  return ptr;
}

inline void residual_sequence::delete_rep(rep*& ptr)
{
  if (--ptr->count==0) {
    int b = sizeof(rep)+(ptr->size-1)*sizeof(double);
    std_memory.deallocate_bytes(ptr,b);
    //if (b>0) delete[] ptr;
#ifdef RESIDUAL_CHECK_POSTCONDITIONS
    ptr=NULL;
#endif
  }
}

inline void residual_sequence::copy_rep
(rep* source, rep* target)
{
  int i;
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(target->size >= source->size);
#endif
  for (i=0; i<source->size; i++)
    target->mod[i]=source->mod[i];
}

inline void residual_sequence::copy_rep
(rep* source, rep* target, int num)
{
  int i;
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(target->size >= num);
  assert(source->size >= num);
#endif
  for (i=0; i<num; i++)
    target->mod[i]=source->mod[i];
}

inline void residual_sequence::resize_rep(int n)
{
  if (PTR!=NULL)
    delete_rep(PTR);
  PTR = new_rep(n);
}

inline double& residual_sequence::operator[](int i)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert (PTR!=NULL);
  assert (0<=i&&i<PTR->size);
  assert (PTR->count > 0);
#endif
  return PTR->mod[i];
}

inline double residual_sequence::operator[](int i) const
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert (PTR!=NULL);
  assert (0<=i&&i<PTR->size);
  assert (PTR->count > 0);
#endif
  return PTR->mod[i];
}



class __exportC residual_matrix
{
  int m, n;
  residual_sequence* row;

public:

  residual_matrix() { m=n=0; row=NULL; }
  int rowsize() const { return m; }
  int colsize() const { return n; }
  residual_matrix(int m, int n);
  ~residual_matrix();
  residual_sequence& operator[](int i);

  void resize(int m, int n); // destroys and resizes matrix

protected:

  void delete_me();
  void create_me(int m, int n);
};


/*{\Manpage {residual} {} {Modular Arithmetic in LEDA} {x}}*/

class __exportC residual : protected residual_sequence {

/*{\Mdefinition
The data type |\Mname| provides an implementation of exact integer arithmetic using modular computation. 
In contrast to the LEDA type |integer| which offers similar functionality as |\Mname|, the user of |\Mname| has to specify for each calculation the maximal bit length |b| of the integers she wants to be exactly representable by |\Mname|s. 
This is done by a call of |\Mname::set_maximal_bit_length(b)| preceding the calculation. The set of integers in the interval $[-2^b,2^b)$ is called the {\em current range} of numbers representable by |\Mname|s.

A \Mtype number $x$ that is outside the current range is said to {\em overflow}. As an effect of its overflow, certain operations cannot be applied to $x$ and the result is undefined. 
These critical operations include e.g. all kinds of conversion, sign testing and comparisons.
It is important to realize that for an integer $x$ given by a division-free expression it only matters whether the {\em final result} $x$ does not overflow. 
This is sometimes useful and hence overflow is not always checked by default. 
%|\Mname|s can be used to evaluate an expression $E$ with operations $+, -, *$ over the integers. If the value of $E$ is in the current range, this value will be exactly computed by |\Mname|s.
%This is true no matter if some intermediate values within the computation of $E$ are outside the range. It can then be converted to the standard |integer| format or it can be used in further |\Mname| calculations.

Division is available for |\Mname|s, but with certain restrictions.
Namely, for each division |x/y| the user has to guarantee at least one of the following two conditions:
\begin{itemize}
\item |y.is_invertible()| is |true|
\item |x/y| is integral {\em and} |x| and |y| do not overflow.
\end{itemize}
If the first condition is satisfied, there is an alternative way to do the division |x/y|. 
Introducing the \Mtype variable |z = y.inverse()|, the call |x/y| is equivalent to the call |x*z|.
The latter form is advantageous if several divisions have the same divisor |y| because here the time-consuming inversion of |y|, which is implicit in the division |x/y|, has to be performed only once. 

If the result of an operation is not integral, the computation will usually proceed without warning. In such cases the computation produces a nonsensical result that is likely to overflow but otherwise is a perfect |\Mname|. However, the operations mentioned above check for overflow.
Note that the implemented overflow checks are not rigorous, detecting invalidity only with empirically high probability. Overflow checking can be switched off by calling |set_maximal_bit_length| with a second, optional parameter |residual::no_overflow_check|.

}*/


public:

  
  
/*{\Mtext
\def\mod{\textrm { mod }}
}*/ 



/*{\Xanpage {residual::mod} {} {The mod kernel of type residual} {x}
 }*/

/*{\Manpage {residual} {} {The mod kernel of type residual} {x}
 }*/

/*{\Mdefinition
Type |residual::mod| provides the basic modular arithmetic modulo primes of maximal size $2^{26}$. 
Here numbers modulo the prime $p$ are represented by integral doubles in $[0,\cdots p-1]$. 
This type cannot be instantiated, so there are only static functions and no constructors.
 The following functions have the common precondition that $p$ is a prime between $2$ and $2^{26}$.
}*/

/*{\Moperations
}*/

class mod{

public:

  static inline double rounded_to_integral(double a)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(0 <= a && a < ldexp(1.0,31));
#endif
    return int(a);
  }

#ifdef WORD_LENGTH_64
  static inline double reduce_of_integral(digit a, double p)
  {
    return double(a%digit(p));
  }
#endif

  static inline double reduce_of_positive(double a, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(0 <= a && a < ldexp(1.0,54));
    assert(a==floor(a));
#endif
#ifdef RESIDUAL_CHECK_POSTCONDITIONS
    double result = a-int(a/p)*p;
    assert(0<=result && result<p);
    return result;
#else
    return a-int(a/p)*p;
#endif
  }
/*{\Mstatic 
returns |a| modulo |p| for nonnegative integral $0 \leq a < 2^{54}$
}*/

  static inline double reduce_of_negative(double a, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(0 > a && -a < ldexp(1.0,54));
    assert(a==floor(a));
#endif
    double negative=reduce_of_positive(-a,p);
    return negate(negative,p);
  }
/*{\Mtext }*/
  static inline double reduce(double a, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(fabs(a) < ldexp(1.0,54));
    assert(a==floor(a));
#endif
    double result=a-int(a/p)*p;
    if (result < 0)
      result+=p;
    return result;
  }
/*{\Mstatic returns |a| modulo |p| for any integral $a$ with $\vert a\vert < 2^{54}$
}*/
  static inline double reduce(double a, double p, double p_inv)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(fabs(a) < ldexp(1.0,54));
    assert(a==floor(a));
#endif
    double result=a-int(a*p_inv)*p;
    if (result < 0)
      result+=p;
    if (result == p)
      result=0;
    return result;
  }

  static inline double add(double a, double b, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(0 <= a && a < ldexp(1.0,52));
    assert(0 <= b && b < ldexp(1.0,52));
#endif
    double z = a+b;
    if (z >= p)
      z-=p;
    return z;
  }
/*{\Mstatic returns $(a + b) \mod p$ where $a$, $b$ are integral with $\vert a\vert, \vert b\vert < 2^{52}$
}*/
  static inline double sub(double a, double b, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(0 <= a && a < ldexp(1.0,52));
    assert(0 <= b && b < ldexp(1.0,52));
#endif
    double z = a-b;
    if (z < 0)
      z+=p;
    return z;
  }
/*{\Mstatic returns $(a - b) \mod p$ where $a$, $b$ are integral with $\vert a\vert, \vert b\vert < 2^{52}$
}*/
  static inline double mul(double a, double b, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(0 <= a*b && a*b < ldexp(1.0,53));
#endif
    return reduce_of_positive(a*b,p);
  }
/*{\Mstatic returns $(a \cdot b) \mod p$ where $a$, $b$ are integral with $\vert a\cdot b\vert < 2^{53}$
}*/
  static inline double div(double a, double b, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(int(b)%int(p)!=0);
    assert(0 <= a && a < ldexp(1.0,26));
#endif
    return reduce_of_positive(a*inverse(b,p),p);
  }
/*{\Mstatic returns $(a \cdot b^{-1}) \mod p$ where $a$, $b$ are integral with $\vert a\vert < 2^{26}$ and $b \neq 0 \mod p$
}*/
  static inline double negate(double a, double p)
  {
#ifdef RESIDUAL_CHECK_PRECONDITIONS
    assert(a==floor(a));
    assert(0 <= a && a < p);
#endif
    if (a != 0)
      return p-a;
    else 
      return 0;
  }
/*{\Mstatic returns $-a \mod p$ for nonnegative $a < p$
}*/
  static double inverse(double a, double p);
/*{\Mstatic returns the inverse of |a| modulo |p| for intergal $0 \leq a < p < 2^{32}$
}*/
};

  


/*{\Xanpage {residual::smod} {} {The smod kernel of type residual} {x}
 }*/

/*{\Manpage {residual} {} {The smod kernel of type residual} {x}
 }*/

/*{\Mdefinition
Type |residual::smod| is a variant of class |residual::mod| that uses a {\em signed} representation.
 Here numbers modulo $p$ are represented by integral doubles in $(-p/2,+p/2)$. 
 All functions have the common precondition that $p$ is a prime between $3$ and $2^{26}$.
The functions of type |residual::mod| are also provided for class |residual::smod| and have the same meaning, so we do not list them separately here.
}*/

/*{\Moperations
}*/

class smod {

public:

  static const double round_mask; // = 3*2^51

  static inline double rounded_to_integral(double a);
#ifdef WORD_LENGTH_64
  static inline double reduce_of_integral(digit a, double p);
#endif
  static inline double reduce_of_positive(double a, double p);
  static inline double reduce_of_negative(double a, double p);
  static inline double reduce(double a, double p);
  static inline double reduce(double a, double p, double p_inv);
  static inline double safe_reduce(double a, double p);
  static inline double lazy_frac(double a);

  static inline double frac(double a);
/*{\Mstatic returns $a+z$ where $z$ is the unique integer such that $a+z\in[-1/2,1/2)$
}*/
  static inline int sign(double a);
  static inline double add(double a, double b, double p);
  static inline double sub(double a, double b, double p);
  static inline double mul(double a, double b, double p);
  static inline double div(double a, double b, double p);
  static inline double inverse(double a, double p);
  static inline double negate(double a, double p);
};



  
class mod_system {

public:

  
  
static bool is_prime(int p);
static int compute_prime_bounded_by(int a);
static int compute_prime_with_bit_size(int b);
static void compute_prime_sequence_bounded_by
(int a, residual_sequence& p);
static void compute_prime_sequence_with_bit_size
(int b, residual_sequence& p);


static void write_table_to_file(const char* filename, residual_sequence& table, int b);

static void read_table_from_file (const char* filename, residual_sequence& table, int& primelength, int& tablesize);

  
static void insert_garner_constant(int i, residual_sequence& u, const residual_sequence& p);
static void compute_garner_constants(int k, residual_sequence& u, const residual_sequence& p);
static void write_garner_table_to_file
(char* filename, int b, int k, residual_sequence& p);

  
 
static void compute_lagrange_w
(residual_sequence& w, const residual_sequence& p);
static void compute_lagrange_alpha
(residual_sequence& al, const residual_sequence& p);
static void compute_W_matrix
(residual_matrix& W, const residual_sequence& p);

  
static void long2residual
(long a, residual_sequence& x, const residual_sequence& p);

static void intdouble2residual
(double a, residual_sequence& x, const residual_sequence& p);

static double binary2mod
(double prime, int length, const digit* a, int sign);

static double integer2mod(const integer& a, double prime);

static double bigbinary2mod
(double prime, int length, const residual_sequence& a48, int sign, double mod48);

static void binary2residual
(residual_sequence& x, const residual_sequence& p, 
 int a_length, const digit* a, int sign,
 residual_sequence& a48, const residual_sequence& mod48);

static void integer2residual
(const integer& a, residual_sequence& x, const residual_sequence& p, 
 residual_sequence& tmp, const residual_sequence& mod48);



static double radix2mod
(double prime, int l, const residual_sequence& y, const residual_sequence& p);
static unsigned long radix2modbase
(int l, const residual_sequence& y, const residual_sequence& p);
static void radix2residual
(residual_sequence& x, int l, const residual_sequence& y, const residual_sequence& p);


static void radix2integer
(integer& X, int l, const residual_sequence& y, const residual_sequence& p);



static void add_garner_coefficient
(int i, residual_sequence& y, const residual_sequence& x, 
 const residual_sequence& u, const residual_sequence& p);
static void residual2radix
(int k, int& l, residual_sequence& y, const residual_sequence& x, 
 const residual_sequence& u, const residual_sequence& p);
static void probabilistic_check_from_radix(int l, const residual_sequence& y);

  
static int sign_from_radix(int l, const residual_sequence& y);
static int sign_from_residual
(const residual_sequence& x, int& count, 
 bool with_check, bool& overflowed, 
 const residual_sequence& w, const residual_sequence& al,
 residual_sequence& tmp, 
 const residual_sequence& p, const residual_sequence& p_inv);


};



  static const bool do_overflow_check;
  static const bool no_overflow_check;
  
protected:

  
static int maximal_primetable_size;
static int used_prime_length; 
static int used_primetable_size; 
static int used_number_of_moduli; 
static int maximal_representable_bit_size;
static bool overflow_check_active;
static residual_sequence primes;           
static residual_sequence primes_inv;       
static residual_sequence garner_constant;    
static bool lagrange_active;
static residual_matrix w_matrix;
static residual_sequence lagrange_w; 
static residual_sequence alpha; 
static residual_sequence mod_48;
static residual_sequence temporary;


  
static int initialize_static_members
(int lagrange_k, int max_k);

static void resize_double_table(int k, residual_sequence& table);
static void fill_main_tables(int k);

static void activate_lagrange();


residual(rep* ptr) { PTR = ptr; }

inline static rep* restrict_rep(rep* old_rep)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(old_rep->size > used_number_of_moduli);
#endif
  rep* newrep=new_rep(used_number_of_moduli);
  copy_rep(old_rep,newrep,used_number_of_moduli);
  delete_rep(old_rep);
  return newrep;
}

inline static rep* upgrade_rep(rep* old_rep)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
   assert(old_rep->size < used_number_of_moduli);
#endif
   rep* newrep=new_rep(used_number_of_moduli);
   copy_rep(old_rep,newrep);
   delete_rep(old_rep);
   return newrep;
}

  
void actualize() const;
void add_moduli_above(int num) const;
inline void refresh() const;


static void rewrite(residual& z);


public:

  ~residual() { }  
  
/*{\Mcreation}*/
residual();
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
with zero.
}*/
residual(long a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
with the value of |a|.
}*/
residual(int a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
with the value of |a|.
}*/
residual(double a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
with the integral part of $x$.
}*/
residual(const integer& a);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
with the value of |a|.
}*/
residual(const residual_sequence& a);
residual(const residual& x);
residual& operator=(const residual& x);

static inline int cmp(const residual& a, const residual& b) 
{ return (a-b).sign(); }
inline int compare(const residual& a, const residual& b) 
{ return cmp(a,b); }

/*{\Moperations 2 4.7 }*/

static int set_maximal_bit_length(int b, bool with_check=do_overflow_check);
/*{\Mstatic sets the maximal bit size of the representable numbers to |b| and returns the previous maximal bit size}*/

static int get_maximal_bit_length() 
{ return maximal_representable_bit_size; }
/*{\Mstatic returns the maximal bit size of the representable numbers}*/

static int required_primetable_size(int b);
/*{\Mstatic returns the number of primes required to represent signed numbers up to bit length |b|}*/

/*{\Mtext
The following functions have the common {\bf precondition} that the \Mtype objects |a|, |\Mvar| are integral and do not overflow.
}*/

integer to_integer() const;
/*{\Mop returns the |integer| equal to |\Mvar|.}*/
integer tointeger() const { return to_integer(); }

long length() const;
/*{\Mop returns the length of the binary representation of the integer represented by |\Mvar|.}*/

bool is_long() const;
/*{\Mop returns |true| if and only if |\Mvar| fits in the data format 
|long|.}*/

long to_long() const;
/*{\Mop returns a |long| number which is initialized with the value of
|\Mvar|. \precond |\Mvar.is_long()| is |true|.}*/

double to_double() const;
/*{\Mop returns a double floating point approximation of |\Mvar|. }*/

double to_float() const { return to_double(); }
/*{\Mop as above. }*/


bool is_zero() const;
/*{\Mop returns true if and only if |\Mvar| is equal to zero.}*/
bool iszero() const { return is_zero(); }

bool is_invertible() const;
/*{\Mop returns |true| if and only if |\Mvar| is nonzero and the current modular representation of |\Mvar| allows to invert |\Mvar| without loss of information.}*/

int sign() const;
/*{\Mop returns the sign of |\Mvar|.}*/
friend __exportF int sign(const residual& x) { return x.sign(); }

int lagrange_sign() const;
/*{\Mop returns the sign of |\Mvar| using Lagrange's formula.}*/

int garner_sign() const;
/*{\Mop returns the sign of |\Mvar| using Garner's formula.}*/

string to_string() const; 
/*{\Mop returns the decimal representation of |\Mvar|. }*/
string tostring() const { return to_string(); }

friend __exportF residual abs(const residual& a);
/*{\Mfunc returns the absolute value of |a|}*/

void absolute(const residual& a);
/*{\Mop sets |\Mvar| to the absolute value of |a|. }*/

/*{\Mtext
The remaining functions do not have implicit preconditions. 
Although not explicitly mentioned, the arithmetic operations 
$+$, $-$, $*$, $/$, |+=|, |-=|, |*=|, |/=|, $++$, $--$, 
the shift operations, the comparison operations 
$<$, |<=|, |>|, |>=|, |==|, |!=| and the stream operations are available.  
}*/
 
friend __exportF ostream& operator << (ostream& O, const residual& a); 
friend __exportF istream& operator >> (istream& I, residual& a);

friend __exportF residual operator+(const residual& a, const residual& b); 
friend __exportF residual operator-(const residual& a, const residual& b); 
friend __exportF residual operator*(const residual& a, const residual& b);
friend __exportF residual operator/(const residual& a, const residual& b);
residual operator-() const;

friend __exportF residual sqr(const residual& a);
/*{\Mfunc returns |a*a|}*/
friend __exportF residual det2x2(const residual& a, const residual& b, 
 		       const residual& c, const residual& d);
/*{\Mfunc returns |a*d-b*c|}*/

friend __exportF bool operator == (const residual& a, const residual& b);
friend __exportF bool operator<(const residual& a, const residual& b);
friend __exportF bool operator>(const residual& a, const residual& b);
friend __exportF bool operator!=(const residual& a, const residual& b);
friend __exportF bool operator>=(const residual& a, const residual& b);
friend __exportF bool operator<=(const residual& a, const residual& b);

residual& operator+= (const residual& b);
residual& operator-= (const residual& b);
residual& operator*= (const residual& b);
residual& operator/= (const residual& b);

void add(const residual& a, const residual& b);
/*{\Mop sets |\Mvar| to |a+b|.}*/
void sub(const residual& a, const residual& b);
/*{\Mop sets |\Mvar| to |a-b|.}*/
void mul(const residual& a, const residual& b);
/*{\Mop sets |\Mvar| to |a*b|.}*/
void div(const residual& a, const residual& b);
/*{\Mop sets |\Mvar| to |a/b|.}*/
void det2x2(const residual& a, const residual& b,
	    const residual& c, const residual &d);
/*{\Mop sets |\Mvar| to |a*d-b*c|.}*/
void inverse(const residual& a);
/*{\Mop sets |\Mvar| to the modular inverse of |a|. \precond |\Mvar.in_invertible| is |true|.}*/
void negate(const residual& a);
/*{\Mop sets |\Mvar| to |-a|.}*/

inline residual& operator++ () { return (*this)+=1; }
inline residual& operator++ (int) { return (*this)+=1; }
inline residual& operator-- () { return (*this)-=1; }
inline residual& operator-- (int) { return (*this)-=1; }

inline residual operator<<(long n) const { return (to_integer()<<n); }
inline residual operator>>(long n) const { return (to_integer()>>n); }
inline residual& operator<<=(int i) { *this=(*this)<<i; return *this; }
inline residual& operator>>=(int i) { *this=(*this)>>i; return *this; }



/*{\Mtext
The following functions provide direct read-only access to the internal representation of \Mtype objects. They should only be used by the experienced user after reading the full documentation of type \Mtype.
}*/

static residual_sequence get_primetable() 
{ return primes; }
/*{\Mstatic returns a copy of the currently used primetable}*/
static residual_sequence get_garnertable() 
{ return garner_constant; }
/*{\Mstatic returns a copy of the currently used table of Garner's constants}*/
residual_sequence get_representation() const
{ return (residual_sequence) (*this); }
/*{\Mfunc returns a copy of the residual sequence representing \Mvar}*/




static void generate_residual_tables
(int primelength, int tablesize);


static void write_residual_tablefiles
(int primelength, int compile_size, int max_size);

  LEDA_MEMORY(residual) 
};



inline double residual::smod::rounded_to_integral(double a)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(fabs(a) < ldexp(1.0,51));
#endif
  return (a + round_mask) - round_mask;
}



inline double residual::smod::reduce(double a, double p)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(fabs(a) < ldexp(1.0,54));
  assert(a == floor(a));
#endif
  return a-rounded_to_integral(a/p)*p;
}
inline double residual::smod::reduce_of_positive(double a, double p)
{
  return reduce(a,p);
}
inline double residual::smod::reduce_of_negative(double a, double p)
{
  return reduce(a,p);
}
#ifdef WORD_LENGTH_64
inline double residual::smod::reduce_of_integral(digit a, double p)
{
  digit mod = a%digit(p);
  if ((mod<<1)>p)
    mod -= (digit) p;
  return (double) mod;
}
#endif


inline double residual::smod::reduce
(double a, double p, double p_inv)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(fabs(a) < ldexp(1.0,54));
  assert(a == floor(a));
#endif
  return a-rounded_to_integral(a*p_inv)*p;
}



inline double residual::smod::lazy_frac(double a)
{
  return a-rounded_to_integral(a);
}


inline double residual::smod::safe_reduce(double a, double p)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(a == floor(a));
  assert(fabs(a) < ldexp(1.0,54));
#endif
  double result = a-rounded_to_integral(a/p)*p;
#ifdef ROUNDING_INDEPENDENCE_REQUIRED
  if (result > p/2)
    result -= p;
  if (result < -p/2)
    result += p;
#endif
  return result;
}

inline double residual::smod::frac(double a)
{
  double result = a-rounded_to_integral(a);
#ifdef ROUNDING_INDEPENDENCE_REQUIRED
  if (result >= 0.5)
    result-=1;
  if (result < -0.5)
    result+=1;
#endif
  return result;
}


inline int residual::smod::sign(double a)
{
  return (a > 0 ? 1 : (a < 0 ? -1 : 0));
}

inline double residual::smod::inverse(double a, double p)
{
  if (a < 0)
    a+=p;
  return safe_reduce(mod::inverse(a,p),p);
}
inline double residual::smod::negate(double a, double p)
{
  return safe_reduce(p-a,p);
}
inline double residual::smod::add
(double a, double b, double p)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(fabs(a) < ldexp(1.0,52));
  assert(fabs(b) < ldexp(1.0,52));
#endif
  return reduce (a+b,p);
}
inline double residual::smod::sub
(double a, double b, double p)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(fabs(a) < ldexp(1.0,52));
  assert(fabs(b) < ldexp(1.0,52));
#endif
  return reduce (a-b,p);
}
inline double residual::smod::mul
(double a, double b, double p)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(fabs(a*b) < ldexp(1.0,53));
#endif
  return reduce (a*b,p);
}
inline double residual::smod::div
(double a, double b, double p)
{
#ifdef RESIDUAL_CHECK_PRECONDITIONS
  assert(fabs(a) < p);
  assert(fabs(b) < p);
#endif
  return reduce (a*inverse(b,p),p);
}



#if LEDA_ROOT_INCL_ID == 600244
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif

