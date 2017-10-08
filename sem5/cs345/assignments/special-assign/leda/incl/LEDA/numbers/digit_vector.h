/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  digit_vector.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef DIGIT_VECTOR_H
#define DIGIT_VECTOR_H

#include <LEDA/system/basic.h>
#include <LEDA/numbers/digit.h>
#include <LEDA/internal/std/string.h>

LEDA_BEGIN_NAMESPACE 

inline void vecInit(digit* a, digit_sz a_used);
inline digit* vecCopy(digit* a, const digit* b, digit_sz count);
void vecFill(digit* a, digit_sz a_used,digit d);
inline digit_sz vecLength(digit* x, digit_sz x_used);
inline void vecIncrement(digit* a, digit_sz a_used);
inline void vecDecrement(digit* a, digit_sz a_used);



inline void vecDecrement(digit* a, digit_sz a_used)
{
  digit* a_stop=a+a_used;
  while (a<a_stop&&--*a==MAX_DIGIT) a++;
}

inline void vecIncrement(digit* a, digit_sz a_used)
{
  digit* a_stop=a+a_used;
  while (a<a_stop&&++*a==0) a++;
}


inline void vecInit(digit* a, digit_sz a_used)
{
  if (a_used < 8) 
    switch(a_used) {
      case  7:*a++ = 0;
      case  6:*a++ = 0;
      case  5:*a++ = 0;
      case  4:*a++ = 0;
      case  3:*a++ = 0;
      case  2:*a++ = 0;
      case  1:*a++ = 0;
    }
  else
    memset(a,0,a_used*DIGIT_SIZE);
}


inline digit* vecCopy(digit* a, const digit* b, digit_sz count)
{
  if (count < 8) {
    switch (count) { 
      case  7:*a++ = *b++;
      case  6:*a++ = *b++;
      case  5:*a++ = *b++;
      case  4:*a++ = *b++;
      case  3:*a++ = *b++;
      case  2:*a++ = *b++;
      case  1:*a++ = *b++; 
    }
    return a;
  }
  else {
    memcpy((void*) a, (void*)b, count*DIGIT_SIZE);
    return a+count;
  }
}


inline digit_sz vecLength(digit* x, digit_sz x_used) {
#ifdef DIGIT_CHECKS_ON
  assert (x_used>0);
  assert (x[x_used-1]!=0);
#endif
  return (x_used<<WORD_LENGTH_LOG)-digLeadingZeros(x[x_used-1]);
}





int vecCompare(digit* a, digit* b, digit_sz n);
int vecCompare(digit* a, digit_sz a_used, digit* b, digit_sz b_used);
int vecCompareNormal(digit* a, digit_sz a_used, digit* b, digit_sz b_used);


digit vecShiftLeftCarry(digit* s, digit* a, digit_sz a_used, digit_sz n);
digit vecShiftRightCarry(digit* s, digit* a, digit_sz a_used, digit_sz n);

digit_sz vecShiftLeft(digit* s, digit* a, digit_sz a_used, digit_sz n);
digit_sz vecShiftRight(digit* s, digit* a, digit_sz a_used, digit_sz n);



digit_sz vecAdd(digit* a, digit_sz a_used, digit *b, digit_sz b_used, digit* s);

digit_sz vecAddTo(digit *a, digit_sz a_used, digit *b, digit_sz b_used);



digit_sz vecSignedAdd(digit* a, digit_sz a_used, int a_sign, digit* b, digit_sz b_used, int b_sign,
  digit* sum, int& sum_sign);


void vecSub(digit *a, digit_sz a_used, digit *b, digit_sz b_used, digit* diff);
void vecSubFrom(digit *a, digit_sz a_used, digit *b, digit_sz b_used);

digit vecSubFromReturnBorrow(digit *a, digit *b, digit_sz b_used);


 
digit_sz vecMul
  (digit* prod, digit* a, digit_sz a_used, digit* b, digit_sz b_used);
digit_sz vecMulSchool
  (digit *a, digit_sz a_used, digit *b, digit_sz b_used, digit* prod);
digit_sz vecMulSchoolHigh
  (digit *a, digit_sz a_used, digit *b, digit_sz b_used, digit* prod, digit_sz k);
void vecMulSchoolLow
  (digit *a, digit_sz a_used, digit *b, digit_sz b_used, digit* prod, digit_sz k);
digit_sz vecMulKara
  (digit* a, digit_sz a_used, digit* b, digit_sz b_used, digit* prod, digit_sz prod_size);


digit_sz vecSquare(digit* prod,digit* a_vec, digit_sz a_used);
digit_sz vecSquareSchool(digit *a, digit_sz a_used, digit *square);
digit_sz vecSquareKara(digit* a, digit_sz a_used, digit* prod);



digit_sz vecDiv
  (digit* quot, digit* r, digit_sz *r_used,
   digit* a, digit_sz a_used,
   digit* b, digit_sz b_used);
digit_sz vecDivByOneDigit
  (digit *a, digit_sz a_used, digit B, 
   digit* quot, digit* rem);
digit_sz vecDivSchool
  (digit* a, digit_sz a_used, digit* b, digit_sz b_used,
   digit* quot, digit* r, digit_sz* r_used);
digit_sz vecDivKara
  (digit* a, digit_sz a_used, digit* b, digit_sz b_used, 
   digit* quot, digit* r, digit_sz* r_used);




digit_sz vecModMul(digit* rem,
  digit* a_vec, digit_sz a_used,
  digit* b_vec, digit_sz b_used,
  digit* n_vec, digit_sz n_used);
digit_sz vecModMulLean(digit* rem,
  digit* a_vec, digit_sz a_used,
  digit* b_vec, digit_sz b_used,
  digit* n_vec, digit_sz n_used, digit *tmp);
digit_sz vecModSquare(digit* rem,
  digit* a_vec, digit_sz a_used,
  digit* n_vec, digit_sz n_used);
digit_sz vecModSquareLean(digit* rem,
  digit* a_vec, digit_sz a_used,
  digit* n_vec, digit_sz n_used, digit* tmp);
digit_sz vecModExp(digit *prod0,digit* a_vec, digit_sz a_used,
  digit* e_vec, digit_sz e_used,
  digit* n_vec, digit_sz n_used);



enum num_op { vecMUL=1, vecADD=2, vecSUB=3, vecSQR=4, vecDIV=5}; 

digit vecModReduce(digit* a, digit_sz a_used, int a_sign);

void vecModCheckOperation(num_op operation,
    digit* a_vec, digit_sz a_used, int a_sign, digit& a_mod,
    digit* b_vec, digit_sz b_used, int b_sign, digit& b_mod,
    digit* c_vec, digit_sz c_used, int c_sign, digit& c_mod);

void vecModCheckSquare(
    digit* a_vec, digit_sz a_used, int a_sign, digit& a_mod,
    digit* c_vec, digit_sz c_used, int c_sign, digit& c_mod);

void vecModCheckDivision(
    digit* a_vec, digit_sz a_used, int a_sign, digit& a_mod,
    digit* b_vec, digit_sz b_used, int b_sign, digit& b_mod,
    digit* c_vec, digit_sz c_used, int c_sign, digit& c_mod,
    digit* r_vec, digit_sz r_used, int r_sign, digit& r_mod);



char* vecToDecimal(digit* a, digit_sz a_used, char* str);
// digit* vecFromDecimal(digit* a, digit_sz a_used, char* str);


digit_sz vecFromDouble(double x, digit* vec, int& sign, int& exp);
digit_sz vecFromDoubleAligned
(double x, digit* vec, int& sign, int& exp, digit_sz& precision);
double vecToDouble(digit* vec, digit_sz used);


LEDA_END_NAMESPACE

#endif

