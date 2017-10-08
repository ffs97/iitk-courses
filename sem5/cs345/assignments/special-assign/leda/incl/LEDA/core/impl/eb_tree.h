/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  eb_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_STRATIFIED_H
#define LEDA_STRATIFIED_H

//------------------------------------------------------------------------------
//  stratified tree  (van emde boas tree)
// 
//  Stefan Naeher (1989)
//
//  Modification History:
//
//  - Use of bounded dictionaries instead of arrays
//    ( bounded dictionaries are implemented by
//      dynamic perfect hashing )
//
//      Michael Wenzel (1990)
//
//
//  - Dynamization
//
//      Michael Wenzel (1990)
//
//
//  - Function pred
//
//      Michael Wenzel (1990)
//
//
//  - Minimum and maximum not recursively stored
//
//      Michael Wenzel (1991)
//
//
//  - Class l_stratified for bot-structures
//      with <= 2 Elements
//
//      Michael Wenzel (1991)
//
//
//  - Union b_dict for dictionary
//    ( Union of bounded dictionary and array )
//
//      Michael Wenzel (1991)
//
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include & Makros
//-----------------------------------------------------------------------------

/*
#include <LEDA/core/impl/dp_hash.h>   
*/

#include <LEDA/core/dictionary.h>
#include <LEDA/core/impl/ch_hash.h>   

LEDA_BEGIN_NAMESPACE


#define pot_2(x)          ( 1 << x )
#define mal_pot_2(x,y)    ( (x) << (y) )
#define down(k)           ( (k) >> 1 )
#define up(k)             ( down(k) + ((k) & 1) )
#define high_bits(x)      ( x >> down(k) ) 
#define low_bits(x)       ( x & (pot_2(down(k))-1) ) 


//-----------------------------------------------------------------------------
// definitions
//-----------------------------------------------------------------------------

class l_stratified;
class stratified;

typedef l_stratified* l_stratified_ptr;
typedef stratified* stratified_ptr;

//-----------------------------------------------------------------------------
// class definition 
//-----------------------------------------------------------------------------

//typedef dictionary<int,GenPtr,ch_hash> b_dict;
//typedef dictionary<int,GenPtr,ch_hash>::item b_dict_item;
typedef dictionary<int,void*,ch_hash> b_dict;
typedef dictionary<int,void*,ch_hash>::item b_dict_item;
typedef b_dict* b_dict_ptr;



class __exportC l_stratified {  // for <= 2 elements

int  mi;      // minimum
int  ma;      // maximum

int size()                
{ if ( ma == -1 ) return 0 ;
  if ( ma == mi ) return 1 ;
  if ( ma <  mi ) return 2 ;
  return 3;   // illegal, pointer was a stratified_ptr
}

int insert(int x)         
{ if ( size() == 0 ) { mi = ma = x; return 1; }
  if ( x < ma ) { ma = x; return 1; }
  if ( x > mi ) { mi = x; return 1; }
  return 0;
 } 

int del(int x)            
{ if (size() == 1 && x == mi) { mi=ma=-1; return 1; }
  if ( x == mi )   { mi = ma; return 1; }
  if ( x == ma )   { ma = mi; return 1; }
  return 0;
 } 

int member(int x)         { return x == mi || x == ma; }

int max() { return (size() <= 2) ? mi : ma; }
int min() { return (size() <= 2) ? ma : mi; }

int succ(int x) 
{ if ( x < ma ) return ma;
  if ( x < mi ) return mi;
  return -1;
}

int pred(int x) 
{ if ( x > mi ) return mi;
  if ( x > ma ) return ma;
  return -1;
}

l_stratified(int x=-1)    { mi = ma = x; }

l_stratified(stratified*, int);


LEDA_MEMORY(l_stratified)

friend class __exportC stratified;

};



class __exportC stratified : public l_stratified { // recursive structure    

  int            k;       // k-structure
  int            sz;      // size 
  l_stratified*  top;     // up(k)-structure
  b_dict_ptr     bot;     // pointer to bounded Dictionary of (k/2)-structures


public:

   stratified(l_stratified*, int);
   stratified(int);
  ~stratified();
  
  int  min()  { return mi; }
  int  max()  { return ma; }
  int  size() { return sz; }
  
  int  min2();
  int  max2();
  
  int  member(int);
  int  succ(int);
  int  pred(int);
  
  int insert(int);
  int del(int);
  
  void print();
  
  LEDA_MEMORY(stratified)
  
};

typedef stratified eb_tree;

LEDA_END_NAMESPACE

#endif
