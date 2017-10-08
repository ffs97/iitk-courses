/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  map2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MAP2_H
#define LEDA_MAP2_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600022
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// map2  
//------------------------------------------------------------------------------ 
#include <LEDA/system/basic.h> 
#include <LEDA/core/impl/ch_map2.h> 

LEDA_BEGIN_NAMESPACE


/*{\Manpage {map2} {I1,I2,E} {Two-Dimensional Maps}}*/

template<class I1, class I2, class E>

class map2 : private ch_map2 {
 
/*{\Mdefinition
An instance $M$ of the parameterized data type |\Mname| is an injective 
mapping from the pairs in $I1\times I2$, called the index type of $M$,
to the set of variables of data type $E$, called the element type of $M$.
$I$ must be a pointer, item, or handle type or the type int. We use
$M(i,j)$ to denote the variable indexed by $(i,j)$ and
we use $\mathit{dom}(M)$ to denote the set of ``used indices''. 
This set is empty at the time of creation and is modified by map2 accesses.

Related data types are |map|, |d_arrays|, |h_arrays|, and |dictionaries|. }*/



 E xdef;

 void copy_inf(GenPtr& x)   const { LEDA_COPY(E,x);  }
 void clear_inf(GenPtr& x)  const { LEDA_CLEAR(E,x); }
 void init_inf(GenPtr& x)   const { x = leda_copy((E&)xdef); }


 public:

/*{\Mtypes 5}*/ 

typedef ch_map2::item item;
/*{\Mtypemember the item type.}*/

typedef I1 index_type1;
/*{\Mtypemember the first index type.}*/  

typedef I2 index_type2;
/*{\Mtypemember the second index type .}*/  

typedef E element_type;
/*{\Mtypemember the element type.}*/ 


/*{\Mcreation M }*/

map2()  { leda_init_default(xdef); }
/*{\Mcreate 
creates an injective function $m$ from $I1\times I2$ to the set of unused 
variables of
type $E$, sets |xdef| to the default value of type |E| (if |E| has no default 
value then |xdef| stays undefined) and |dom(M)| to the empty set, and 
initializes $M$ with $m$.}*/

map2(E x) : xdef(x) {}
/*{\Mcreate 
creates an injective function $m$ from $I1\times I2$ to the set of unused 
variables of
type $E$, sets |xdef| to |x| and |dom(M)| to the empty set, and initializes $M$
with $m$.}*/

map2(E x,int index_sz, int table_sz) : ch_map2(index_sz,table_sz),xdef(x)
{}
map2(E x,int index_sz) : ch_map2(index_sz) ,xdef(x) 
{}

 map2(const map2<I1,I2,E>& M): ch_map2((ch_map2&)M),xdef(M.xdef) { }
~map2() { clear_entries(); }

 
/*{\Moperations 2 5.0 }*/


const E& operator()(const I1& i, const I2& j) const 
{ ch_map2_item p = ch_map2::lookup(ID_Number(i),ID_Number(j));
  if (p) return LEDA_CONST_ACCESS(E,ch_map2::inf(p));
  else return xdef; 
 }
 
E& operator()(const I1& i, const I2& j)
{ return LEDA_ACCESS(E,ch_map2::access(ID_Number(i),ID_Number(j))); }
/*{\Mfunop        returns the variable $M(i)$.}*/

 
bool defined(const I1& i, const I2& j) const
{ return ch_map2::lookup(ID_Number(i),ID_Number(j)) != nil; }
/*{\Mop      returns true if $i \in \mathit{dom}(M)$ and false otherwise.}*/

void clear() { ch_map2::clear(); }
/*{\Mop      clears $M$ by making $\mathit{dom}(M)$ the empty set. }*/

// iteration

item first_item() const 
 { return ch_map2::first_item(); }

item next_item(item it) const 
 { return ch_map2::next_item(it); }

E inf(item it) const
{ return LEDA_CONST_ACCESS(E,ch_map2::inf(it)); }


};


/*{\Mimplementation
Maps are implemented by hashing with chaining and table doubling. 
Access operations $M(i,j)$ take expected time $O(1)$. }*/
 


#if LEDA_ROOT_INCL_ID == 600022
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif
