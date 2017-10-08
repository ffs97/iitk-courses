/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  integer.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_INTEGER_H
#define LEDA_INTEGER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600246
#include <LEDA/internal/PREAMBLE.h>
#endif


/*{\Manpage {integer} {} {Integers of Arbitrary Length} }*/

#include <LEDA/system/basic.h>

#ifdef INTEGER_PRIV_CHECKS_ON
#include <assert.h>
#endif

/*
#include <LEDA/system/basic.h>
*/

LEDA_BEGIN_NAMESPACE

typedef unsigned long digit;
typedef unsigned int  sz_t;


class  __exportC integer_rep 
{

public:

 atomic_counter count;

#ifdef INTEGER_PUB_CHECKS_ON
  unsigned long  mod;
#endif

  sz_t  size;
  sz_t  used;
  int   sign;
  digit vec[1];

public:

 integer_rep() : count(1) {}
 unsigned long ref_plus()  { return ++count; }
 unsigned long ref_minus() { return --count; }

};




class __exportC integer {
 
  friend class rational;
  friend class bigfloat;

/*{\Mdefinition
An instance of the data type |\Mname| is an integer number of 
arbitrary length. The internal representation of an integer consists
of a vector of so-called {\em digits} and a sign bit. 
A {\em digit} is an unsigned long integer (type |unsigned long|).
}*/

private:

  integer_rep* PTR;

  
 static integer_rep* new_rep(sz_t size);
 static void         delete_rep(integer_rep *p);
 static integer_rep* copy_rep(integer_rep*);

 void construct_zero();

 inline static integer_rep* allocate_bytes(sz_t s);
 inline static void deallocate_bytes(integer_rep* p, sz_t s);

 integer(integer_rep* p) { PTR = p; }

 unsigned  refs() const { return PTR->count; }

 void decimal_string(char* p) const; 
 // writes the decimal representation into char array p

friend integer_rep* divide(
    const integer& a, const integer& b, integer_rep*& r_ptr);


public:

  enum {ZERO=0, NEGATIVE=-1, POSITIVE=1 };
  
  
#ifdef INTEGER_PUB_CHECKS_ON
static bool do_checking;
static void activate_checking();
static void inactivate_checking();
#endif
#ifdef INTEGER_PRIV_CHECKS_ON
static sz_t obj_count;
#endif

  
/*{\Mcreation a }*/

integer() { construct_zero(); }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
with zero.}*/

integer(int n);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the value of $n$.}*/

integer(unsigned int i);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the value of $i$.}*/

integer(long l);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the value of $l$.}*/

integer(unsigned long i);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the value of $i$.}*/

integer(double x); 
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the integral part of $x$.}*/

integer(unsigned int sz, const digit* vec, int sign=1);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            with the value represented by the first |sz| digits $vec$ and 
            the |sign|.}*/

integer(const char* s);
/*{\Mcreate |\Mvar| creates an instance |\Mvar| of type |\Mname| from its 
            decimal representation given by the string |s|.}*/

integer(const string& s) { PTR=NULL; (*this)=integer(s.cstring()); }
/*{\Mcreate |\Mvar| creates an instance |\Mvar| of type |\Mname| from 
            its decimal representation given by the string |s|.}*/

integer(const integer& a)  { PTR = a.PTR;  PTR->ref_plus(); }

~integer() { if (PTR && PTR->ref_minus() == 0) integer::delete_rep(PTR); }


integer(int,int); // used to construct integer_zero

  
/*{\Moperations 2 4.7 }*/
/*{\Mtext
The arithmetic operations $+$, $-$, $*$, $/$, $+=$, $-=$, $*=$, $/=$, $-$(unary), 
$++$, $--$,  the modulus operation ($\%$, $\% =$), bitwise AND ($\&$, $\& =$), 
bitwise OR ($\Lvert, \Lvert =$), the complement ($\ \tilde{}\ $),
the shift operations ($<<$, 
$>>$), the comparison operations $<$, $<=$, $>$, 
$>=$, $==$, $!=$ and the stream operations all are available.
}*/
  int  sign() const { return PTR->sign; }
/*{\Mop returns the sign of |\Mvar|. }*/

  int    length()   const;
/*{\Mop returns the number of bits of the representation of |\Mvar|.}*/

  bool   is_long()   const;
/*{\Mop returns whether |\Mvar| fits in the data type $long$.}*/

  long to_long() const;
/*{\Mop returns a $long$ number which is initialized with the value of |\Mvar|.
        \precond $a$.is\_long() is $true$.}*/

  double to_double() const;
/*{\Mop returns a double floating point approximation of |\Mvar|. }*/

  double to_double(bool& is_double) const;
/*{\Mop as above, but also returns in |is_double| whether the 
        conversion was exact. }*/

  double to_float() const { return to_double(); }
/*{\Mop as above. }*/


  string to_string() const; 
/*{\Mop returns the decimal representation of |\Mvar|. }*/

  integer& from_string(string s) 
   { *this = integer(s.cstring()); return *this; }
/*{\Mop sets |\Mvar| to the number that has decimal respresentation |s|.}*/


  friend __exportF ostream& operator << (ostream & out, const integer& a);
  friend __exportF istream& operator >> (istream & in, integer& a);

  
  sz_t used_words()  const { return PTR->used; }  
/*{\Mop returns the length of the digit vector that represents |\Mvar|. }*/

  digit highword() const;
/*{\Mop returns the most significant digit of |\Mvar|. }*/

  digit contents(int i) const { return PTR->vec[i]; };
/*{\Mop returns the $i$-th digit of |\Mvar| (the first digit
        is |\Mvar.contents(0)|). }*/

  void hex_print(ostream& o) const;
/*{\Mop prints the digit vector that represents |\Mvar| in hex format to 
  the output stream |o|. }*/ 

  // const // omit for test
  digit* word_vector() const { return &PTR->vec[0]; } 
  int  zeros() const; 
  inline digit operator[] (sz_t i) const;

  

  
  integer operator-() const;
  integer operator~() const;
  integer operator<<(int n) const;
  integer operator>>(int n) const;
  integer operator+= (const integer& b) { return *this = *this + b; }
  integer operator-= (const integer& b) { return *this = *this - b; }
  integer operator*= (const integer& b) { return *this = *this * b; }
  integer operator/= (const integer& b) { return *this = *this / b; }
  integer operator%= (const integer& b) { return *this = *this % b; }
  integer operator&= (const integer& b) { return *this = *this & b; }
  integer operator|= (const integer& b) { return *this = *this | b; }


  integer& operator<<=(int n);

/*
  integer& operator<<=(int n) { return *this = *this << n; }
*/

  integer& operator>>=(int n) { return *this = *this >> n; }

  integer operator++();
  integer operator--();

  integer operator++(int) 
  { integer i = *this; operator++(); return i; }

  integer operator--(int) 
  { integer i = *this; operator--(); return i; }


  friend __exportF integer operator + (const integer& a, const integer& b);
  friend __exportF integer operator - (const integer& a, const integer& b);
  friend __exportF integer operator * (const integer& a, const integer& b);
  friend __exportF integer operator / (const integer& a, const integer& b);
  friend __exportF integer operator % (const integer& a, const integer& b);
  friend __exportF integer operator & (const integer& a, const integer& b);
  friend __exportF integer operator | (const integer& a, const integer& b);

  friend __exportF bool operator == (const integer& a, const integer& b);
  friend __exportF bool operator <  (const integer& a, const integer& b);
  friend __exportF bool operator >  (const integer& a, const integer& b);
  friend inline bool operator != (const integer& a, const integer& b);
  friend inline bool operator >= (const integer& a, const integer& b);
  friend inline bool operator <= (const integer& a, const integer& b);

  bool operator==(int n) const;
  bool operator< (int n) const;
  bool operator> (int n) const;
  bool operator!=(int n) const;
  bool operator>=(int n) const;
  bool operator<=(int n) const;

  inline integer& operator=(const integer& x);

  
  bool   iszero()   const { return (PTR->sign == 0); }
/*{\Mop returns whether |\Mvar| is equal to zero.}*/
/*{\Mtext
\smallskip
{\bf Non-member functions}
\smallskip }*/

  friend  double to_double(const integer& a) { return a.to_double(); }
/*{\Mfunc returns a double floating point approximation of $a$. }*/

  friend __exportF integer sqrt(const integer& a);
/*{\Mfunc returns the largest $integer$ which is not larger than the
          square root of $a$. }*/

  friend __exportF integer abs(const integer& a);
/*{\Mfunc returns the absolute value of $a$.}*/

  friend __exportF integer factorial(const integer& n);
/*{\Mfunc returns $n!$.}*/

  friend __exportF integer gcd(const integer& a, const integer& b);
/*{\Mfunc returns the greatest common divisor of $a$ and $b$.}*/

  friend __exportF int  log(const integer& a);
/*{\Mfunc returns the logarithm of $a$ to the basis 2 (rounded down).}*/

  friend __exportF int  log2_abs(const integer& a);
/*{\Mfunc returns the logarithm of $\Lvert a \Lvert$ to the basis 2 (rounded up).}*/

  friend inline    int  sign(const integer& a);// { return a.PTR->sign; }
/*{\Mfunc returns the sign of $a$.}*/

  friend inline integer sqr(const integer& a);// { return a.square(); }
/*{\Mfunc returns  $a^2$. }*/


  friend __exportF double double_quotient(const integer& a, const integer& b);
/*{\Mfunc returns a the best possible floating-point approximation 
          of $a/b$.}*/

  static integer random(int n);
/*{\Mstatic  returns a random integer of length $n$ bits.}*/

  friend __exportF random_source& operator>> (random_source& ran, integer& x);
/*{\Xbinopfunc extracts an integer of default precision from random source 
               $ran$ (cf. /ref{Random Sources}) and assigns it to $x$. }*/


  friend __exportF 
    integer modMul(const integer& a,const integer& b, const integer& n);

  friend __exportF 
    integer modSquare(const integer& a, const integer& n);

  friend __exportF 
    integer modExp(const integer& a, const integer& e, const integer& n);

  friend __exportF 
    bool cmpVector(const integer& a, digit* v, sz_t used);

  static int cmp(const integer& a, const integer& b);
  void absolute();
  integer div(const integer&, integer&) const;

  integer square() const;

  LEDA_MEMORY(integer)

/*{\Mimplementation 
An $integer$ is essentially implemented by a 
vector $vec$ of $unsigned\ long$ numbers. 
The sign and the size are stored in extra variables.
Some time critical functions are also implemented in assembler code.}*/

}; 


/*
inline const integer& min(const integer& a, const integer& b) { return (a<b) ? a : b; }
inline const integer& max(const integer& a, const integer& b) { return (a>b) ? a : b; }
*/


inline integer& integer::operator=(const integer& x) {
#ifdef INTEGER_PRIV_CHECKS_ON
 int used = x.PTR->used;
 int sign = x.PTR->sign;
 if (used==0) assert (sign==0);
 if (sign==0) assert (used==0);
 if (used >0) assert(x[used-1]!=0);
#endif
 x.PTR->ref_plus();
 if (PTR && PTR->ref_minus() == 0) integer::delete_rep(PTR);
 PTR = x.PTR;
 return *this;
}


inline digit integer::operator[](sz_t i) const {
#ifdef INTEGER_PRIV_CHECKS_ON
  assert (i<=PTR->used);
#endif
  return (digit) PTR->vec[i];
}

inline digit integer::highword() const
{ if (PTR->sign == 0) return 0;
  else return PTR->vec[PTR->used-1];
}


inline bool operator != (const integer& a, const integer& b) { return !(a==b); }
inline bool operator <= (const integer& a, const integer& b) { return !(a>b);  }
inline bool operator >= (const integer& a, const integer& b) { return !(a<b);  }

inline bool integer::operator!=(int a) const { return !operator==(a); }
inline bool integer::operator<=(int a) const { return !operator>(a);  }
inline bool integer::operator>=(int a) const { return !operator<(a);  }

inline int  sign(const integer& a) { return a.PTR->sign; }
inline integer sqr(const integer& a) { return a.square(); }

inline const char* leda_tname(const integer*) { return "integer"; } 

inline int leda_type_id(const integer*)   { return INTEGER_TYPE_ID; }

inline int DEFAULT_COMPARE(const integer& a, const integer& b) 
{  return integer::cmp(a,b); }


#if defined(__DMC__)
inline integer operator + (int a, const integer& b) { return integer(a) + b; }
inline integer operator - (int a, const integer& b) { return integer(a) + b; }
inline integer operator * (int a, const integer& b) { return integer(a) + b; }
inline integer operator / (int a, const integer& b) { return integer(a) + b; }
#endif


#if LEDA_ROOT_INCL_ID == 600246
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif


