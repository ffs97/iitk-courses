/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  digit.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef DIGIT_H
#define DIGIT_H


#include <LEDA/internal/std/math.h>

#include <limits.h>

#ifdef DIGIT_CHECKS_ON
#include <LEDA/system/assert.h>
#endif


#if !defined(WORD_LENGTH_32) && !defined(WORD_LENGTH_64)
#error WORD_LENGTH must be either 32 or 64
#endif


LEDA_BEGIN_NAMESPACE


typedef unsigned long digit;

typedef unsigned int  digit_sz;


#ifdef ASM_SPARC

#if defined(__sparcv9)
#error no assembbler for sparcv9
#endif

#define vecShift_asm
#define digMul_asm
#define vecAdd_asm
#define vecSub_asm
#define vecMul_asm
#define vecSqr_asm
#define vecDiv_asm
#endif


#ifdef ASM_INTEL_386
#define digMul_asm
#define vecAdd_asm
#define vecSub_asm
#define vecMul_asm
#define vecSqr_asm
#define vecDiv_asm
#endif

#if defined(ASM_AMD64)
#define digMul_asm
#define vecAdd_asm
#define vecSub_asm
//#define vecMul_asm
#define vecDiv_asm
#endif


#ifdef ASM_MIPS_64 
#define digMul_asm
#endif

#ifdef ASM_ALPHA 
#define digMul_asm
#endif





extern __exportF digit digPower(digit x, digit_sz n);
extern "C" digit   digMul(digit a, digit b, digit* high);

inline digit_sz digLeadingZeros(digit x);
inline digit_sz digTrailingZeros(digit x);
inline digit_sz digNextPower(digit x);
inline digit digModReduce(digit x, digit p) { return x%p; }


 
static const digit_sz  DIGIT_SIZE    = sizeof(digit);
static const digit_sz  DIGIT_LENGTH  = CHAR_BIT*sizeof(digit);
static const digit_sz  HALF_LENGTH   = DIGIT_LENGTH/2;
static const digit MAX_DIGIT     = ~0UL;
static const digit MIN_NDIGIT    = 1UL<<(DIGIT_LENGTH-1);
static const digit HALF_BASE     = 1UL<<HALF_LENGTH;
static const digit MAX_HALFDIGIT = HALF_BASE-1;

static const digit LEFT_MASK     = MAX_DIGIT-MAX_HALFDIGIT;
static const digit RIGHT_MASK    = MAX_HALFDIGIT;


#define LOW_WORD(w)  (w & RIGHT_MASK)
#define HIGH_WORD(w) (w >> HALF_LENGTH)

static const double digit_base = ldexp(1.0,DIGIT_LENGTH);
static const double digit_pow32 = ldexp(1.0,32);
static const double digit_pow64 = ldexp(1.0,64);
static const double digit_roundup_factor   = 1+ldexp(8.0,-53);
static const double digit_rounddown_factor = 1-ldexp(8.0,-53);

static const digit_sz digit_decimal_base_length10 = digit_sz(log10(digit_base));
static const digit_sz digit_decimal_base_length2  = digit_sz(digit_decimal_base_length10/log10(2.0));

static const digit digit_decimal_base = digPower(10,digit_decimal_base_length10);

static const digit_sz  digit_shift_decimal_to_normal=DIGIT_LENGTH-1-digit_decimal_base_length2;

static const digit digit_shifted_decimal_base=digit_decimal_base<<digit_shift_decimal_to_normal;






inline digit digDivTwoByOne(digit high, digit low, digit D, digit* q);

#ifdef WORD_LENGTH_32
inline digit digDivTwoByOne32(digit high, digit low, digit D, digit* q);
#endif

inline digit digDivTwoByOneNormal(digit high, digit low, digit D, digit* q);
inline digit digDivThreeByTwoNormal
             (digit A1, digit A2, digit A3, digit B1, digit B2);



char* digToDecimal(digit x, char* cp);
char* digFromDecimal(digit& x, char* cp);


inline digit_sz digLeadingZeros(digit x)
{
  digit_sz rshift = 0;
  digit y = x >> HALF_LENGTH;

  if (y) 
  { rshift = HALF_LENGTH;
    x = y;
  }
  
  while(x) 
  { y = x;
    x >>= 2;
    rshift += 2;
  }

  if (y==1) rshift--; 
    
  return DIGIT_LENGTH-rshift;
}


inline digit_sz digTrailingZeros(digit x)
{
  digit_sz lshift = 0;
  digit y = x << HALF_LENGTH;

  if (y) 
  { lshift = HALF_LENGTH;
    x = y;
   }
  
  while(x) 
  { y = x;
    x <<= 2;
    lshift += 2;
   }

  if (y == (digit(1) << (DIGIT_LENGTH-1)) ) lshift--; 
    
  return DIGIT_LENGTH-lshift;
}


inline digit_sz digNextPower(digit s)
{ digit p = 1;
  while (p < s) p <<= 1;
  return digit_sz(p);
}



#ifdef WORD_LENGTH_32
inline digit digDivTwoByOne32(digit high, digit low, digit D, digit* q)
{ 
  // precondition: digit size = 32 bit, high < D
  // *q = [high,low] / D, returns remainder
  // we can compute it with double precision floating point division

  double H = high*digit_base;
  double L = low;
  digit Q = digit(((H+L)/D)*digit_rounddown_factor);

  double Q_high = (double) (Q & LEFT_MASK);
  double Q_low  = (double) (Q & RIGHT_MASK);
  double diff = ((H - D*Q_high) - D*Q_low) + L;
  if (diff >= D) 
    {
      Q++;
      diff-=D;
    }
  *q = Q;
  return (digit) diff;
}
#endif


void digDivThreeHalvesByTwoSpecial
(digit A12, digit a3, digit B, digit b1, digit b2, digit& q, digit& R);


inline 
void digDivThreeHalvesByTwoNormal
(digit A12, digit a3, digit B, digit b1, digit b2, digit& q, digit& R)
{
  // small letters = half-digits
  // capital letters = digits

#ifdef DIGIT_CHECKS_ON
  assert(b1>>(HALF_LENGTH-1)!=0);
  assert(A12<B);
#endif

  q = A12/b1;

  if (HIGH_WORD(q))
    digDivThreeHalvesByTwoSpecial(A12,a3,B,b1,b2,q,R);
  else {
    digit r = A12-q*b1;
    digit A = (r<<HALF_LENGTH)+a3;
    digit D = q*b2;
    R = A-D;

    if (A<D) // remainder negative => q too large at one by least
    {
      q--;
      R += B; // R = B + R; -B<=R<B holds
          
      if (R>B) // q still too large
      { q--;
        R += B; // this is the correct R
      }
    }
  }

#ifdef DIGIT_CHECKS_ON
#ifdef WORD_LENGTH_32
   double xd = double(A12)*HALF_BASE+a3;
   double bd = B;
   assert (q==digit(xd/bd));
   assert (R==digit(xd-bd*q));
#else
   digit Ph,Pl;
   Pl  = digMul(B,q,&Ph);
   Pl += R;
   if (Pl < R)
     Ph++;
   digit _A12 = (Ph<<HALF_LENGTH)+HIGH_WORD(Pl);
   digit _a3 = LOW_WORD(Pl);
   assert(_A12==A12);
   assert(_a3==a3);
#endif
#endif
}

inline digit digDivTwoByOneNormal(digit A12, digit A34, digit B12, digit* Q)
{
  digit b1 = HIGH_WORD(B12);
  digit b2 =  LOW_WORD(B12);
  digit a3 = HIGH_WORD(A34);
  digit a4 =  LOW_WORD(A34);
  digit q1, q2, R1;
  digit R;
  digDivThreeHalvesByTwoNormal(A12,a3,B12,b1,b2,q1,R1);
  digDivThreeHalvesByTwoNormal(R1 ,a4,B12,b1,b2,q2,R );
  *Q = (q1<<HALF_LENGTH)+q2;
#ifdef DIGIT_CHECKS_ON
  digit _A12, _A34;
  _A34=digMul(*Q,B12,&_A12);
  _A34+=R;
  if (_A34<R)
    _A12++;
  assert(_A12==A12);
  assert(_A34==A34);
#endif
  return R;
}

inline digit digDivThreeByTwoNormal(digit A1, digit A2, digit A3, digit B1, digit B2)
{
  digit Q,P1,P2,R1,R2;
  R1 = digDivTwoByOneNormal(A1,A2,B1,&Q); 
  P2 =  digMul(Q,B2,&P1);  
 
  R2 = A3;
  if (P1>R1||(P1==R1&&P2>R2))
    {
      Q--;
      // [R1,R2]+=[B1,B2]
      R2+=B2;
      if (R2<B2) 
        R1++;
      R1+=B1;

      if (R1>=B1) // no overflow in R1
        if (P1>R1||(P1==R1&&P2>R2))
          Q--;
    }
  return Q;
}

inline digit digDivTwoByOne(digit A12, digit A34, digit B12, digit* Q)
{
  digit_sz lshift = digLeadingZeros(B12);
  if (lshift) {
    B12<<=lshift;
    A12<<=lshift;
    A12+=(A34>>(DIGIT_LENGTH-lshift));
    A34<<=lshift;
  }
  return digDivTwoByOneNormal(A12,A34,B12,Q)>>lshift;
}

bool digIsPrime(digit p);
bool digIsPseudoPrime(digit p, digit K);
digit digGeneratePrimeWithBound(digit a);
digit digGeneratePseudoPrimeWithBound(digit a, digit K);


int digFromDouble(double x, digit& hi, digit& lo);

LEDA_END_NAMESPACE

#endif

