/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  iteration.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_ITERATION_H
#define LEDA_ITERATION_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600264
#include <LEDA/internal/PREAMBLE.h>
#endif

//LEDA_BEGIN_NAMESPACE

#define LEDA_FORALL_PREAMBLE  if (0); else

//------------------------------------------------------------------------------
//   forall_items(it,...)
//   forall_rev_items(it,...)
//------------------------------------------------------------------------------

template<class T_it>
inline void LOOP_ASSIGN(T_it& x, void* p) { x = (T_it)p; }

/*
#define forall_items(x,S)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var=(S).first_item();\
LOOP_ASSIGN(x,loop_var), loop_var = S.next_item(x), x;)
*/



template<class T, class T_it>
inline void* LOOP_ASSIGN_NEXT(const T& S, T_it& x, void*& p) 
{ x = (T_it)p; p = S.next_item(x); return x; }

template<class T, class T_it>
inline void* LOOP_ASSIGN_PRED(const T& S, T_it& x, void*& p) 
{ x = (T_it)p; p = S.pred_item(x); return x; }

#define forall_items(x,S)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var=(S).first_item();\
LOOP_ASSIGN_NEXT(S,x,loop_var); )

#define forall_rev_items(x,S)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var=(S).last_item();\
LOOP_ASSIGN_PRED(S,x,loop_var); )



//------------------------------------------------------------------------------
//   forall(x,...)
//   forall_rev(x,...)   (Forall)
//   forall_defined(x,...)
//------------------------------------------------------------------------------

#define LOOP_ITEM(p) (typename T::item)p

template<class T>
inline bool LedaLoopSucc(const T& S, void*& p) 
{ if (p) { p = S.next_item(LOOP_ITEM(p)); return true; }
  else return false;
 }

template<class T>
inline bool LedaLoopPred(const T& S, void*& p)
{ if (p) { p = S.pred_item(LOOP_ITEM(p)); return true; }
  else return false;
 }

template<class T, class var_type>
inline void LedaLoopInf(const T& S, var_type& x, void* p) 
{ if (p) x = S.inf(LOOP_ITEM(p)); }

template<class T, class var_type>
inline void LedaLoopKey(const T& S, var_type& x, void* p) 
{ if (p) x = S.key(LOOP_ITEM(p)); }


#define forall(x,S)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var = (S).first_item();\
LedaLoopInf(S,x,loop_var), LedaLoopSucc(S,loop_var); )

#define forall_rev(x,S)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var = (S).last();\
LedaLoopInf(S,x,loop_var), LedaLoopPred(S,loop_var); )

#define forall_defined(x,S)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var = (S).first_item();\
LedaLoopKey(S,x,loop_var), LedaLoopSucc(S,loop_var); )




//------------------------------------------------------------------------------
// Macros for generating STL iterators
//------------------------------------------------------------------------------

#if defined(__SGI_STL_INTERNAL_ITERATOR_BASE_H) || \
    defined(__SGI_STL_INTERNAL_ITERATOR_H) || \
   (defined(_MSC_VER) && defined (_ITERATOR_))
#define leda_bidirectional_iterator_tag std::bidirectional_iterator_tag
#else
struct leda_bidirectional_iterator_tag {};
#endif

typedef int leda_ptrdiff_t;

template<class T1, class T2>
class stl_access_pair { 
public:
 const T1& first; 
 const T2& second;
 stl_access_pair(const T1& x, const T2& y) : first(x), second(y) {}
}; 


#define DEFINE_STL_ITERATORS(DTYPE,ITYPE,VTYPE)\
\
class iterator {\
public:\
  DTYPE* L;\
  ITYPE it;\
 iterator(DTYPE* x, ITYPE y) : L(x), it(y)  {}\
 iterator() : L(0),it(0)   {}\
 iterator& operator++()    { it = L->stl_next_item(it); return *this; }\
 iterator& operator--()    { it = L->stl_pred_item(it); return *this; }\
 iterator  operator++(int) { iterator tmp = *this; ++(*this); return tmp; }\
 iterator  operator--(int) { iterator tmp = *this; --(*this); return tmp; }\
 VTYPE& operator*() const  { return L->stl_item_access(it); }\
 VTYPE* operator->() const { return &(L->stl_item_access(it)); }\
 bool operator==(const iterator& i) { return it == i.it; }\
 bool operator!=(const iterator& i) { return it != i.it; }\
/*\
 friend ostream& operator<<(ostream& o, const iterator& i)\
 { return o << i.it; }\
*/\
 operator ITYPE() const { return it; }\
\
 typedef VTYPE  value_type;\
 typedef VTYPE* pointer;\
 typedef VTYPE& reference;\
 typedef leda_bidirectional_iterator_tag iterator_category;\
 typedef leda_ptrdiff_t difference_type;\
\
/*friend VTYPE* value_type(const iterator&) { return (VTYPE*)0; }*/\
};\
\
iterator  begin() { return iterator(this,first_item());}\
iterator  end()   { return iterator(this,stl_next_item(last_item()));  }\
\
class const_iterator {\
  const DTYPE* L;\
  ITYPE it;\
 public:\
  const_iterator(const DTYPE* x, ITYPE y) : L(x), it(y)  {}\
 const_iterator() : L(0),it(0)   {}\
 const_iterator& operator++()    { it = L->stl_next_item(it); return *this; }\
 const_iterator& operator--()    { it = L->stl_pred_item(it); return *this; }\
 const_iterator  operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }\
 const_iterator  operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }\
 const VTYPE& operator*() const  { return L->stl_item_access(it); }\
 const VTYPE* operator->() const { return &(L->stl_item_access(it)); }\
 bool operator==(const const_iterator& i) { return it == i.it; }\
 bool operator!=(const const_iterator& i) { return it != i.it; }\
/*\
 friend ostream& operator<<(ostream& o, const const_iterator& i)\
 { return o << i.it; }\
*/\
 operator ITYPE() const { return it; }\
\
 typedef VTYPE  value_type;\
 typedef const VTYPE* pointer;\
 typedef const VTYPE& reference;\
 typedef leda_bidirectional_iterator_tag iterator_category;\
 typedef leda_ptrdiff_t difference_type;\
\
/*friend VTYPE* value_type(const const_iterator&) { return (VTYPE*)0; }*/\
};\
\
const_iterator begin() const { return const_iterator(this,first_item()); }\
const_iterator end()   const { return const_iterator(this,stl_next_item(last_item()));   }


#if LEDA_ROOT_INCL_ID == 600264
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


//LEDA_END_NAMESPACE

#endif
