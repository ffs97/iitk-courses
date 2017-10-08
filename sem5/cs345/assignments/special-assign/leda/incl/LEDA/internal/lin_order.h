/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  lin_order.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_LIN_ORDER_H
#define LEDA_LIN_ORDER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600262
#include <LEDA/internal/PREAMBLE.h>
#endif


LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// Defining Linear Orders
//------------------------------------------------------------------------------

#define DEFINE_LINEAR_ORDER(type,cmp,new_type)\
struct new_type : public type\
{ new_type(const type& s)     : type(s) {}\
  new_type(const new_type& s) : type(s) {}\
  new_type() {}\
 ~new_type() {}\
};\
inline int compare(const new_type& x, const new_type& y) { return cmp(x,y); }


// _INTEGER<cmp>: int with user defined linear order cmp

typedef int (*CMP_INT_TYPE)(const int&, const int&);

template<CMP_INT_TYPE cmp> 
class _INTEGER {
  int p;
public:
  _INTEGER(int i=0) { p = i;}
  operator int()   { return p; }
  friend int compare(const _INTEGER<cmp>& x, const _INTEGER<cmp>& y)
  { return cmp(x.p,y.p);}
};


#if LEDA_ROOT_INCL_ID == 600262
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
