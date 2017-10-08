/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  map.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MAP_H
#define LEDA_MAP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600019
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h> 
#include <LEDA/core/impl/ch_map.h>

LEDA_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning(disable:4172)
#endif


/*{\Mtext
\parskip 4pt plus 1pt minus 1pt
}*/
/*{\Manpage {map} {I,E} {Maps}}*/

template<class I, class E>
class map : private ch_map {
 
/*{\Mdefinition
An instance $M$ of the parameterized data type |\Mname| is an injective 
mapping from the data type $I$, called the index type of $M$,
to the set of variables of data type $E$, called the element type of $M$.
$I$ must be a pointer, item, or handle type or the type int. We use
$M(i)$ to denote the variable indexed by $i$. All variables are 
initialized to |xdef|, an element of $E$ that is specified in the 
definition of |M|. A subset of |I| is designated as the domain of |M|. Elements are
added to |dom(M)| by the subscript operator; however, the domain may also
contain indices for which the access operator was never executed.  


Related data types are |d_arrays|, |h_arrays|, and |dictionaries|. }*/

 E xdef;

 void copy_inf(GenPtr& x)   const { LEDA_COPY(E,x);  }
 void clear_inf(GenPtr& x)  const { LEDA_CLEAR(E,x); }
 void init_inf(GenPtr& x)   const { x = leda_copy((E&)xdef); }

 public:

/*{\Mtypes 5}*/ 

typedef ch_map::item item;
/*{\Mtypemember the item type.}*/

typedef I key_type;

typedef I index_type;
/*{\Mtypemember the index type.}*/  

typedef E element_type;
/*{\Mtypemember the element type.}*/ 

 /*{\Mcreation M }*/

 map()  { leda_init_default(xdef); }
 /*{\Mcreate 
 creates an injective function $m$ from $I$ to the set of unused variables of
 type $E$, sets |xdef| to the default value of type |E| (if |E| has no default 
 value then |xdef| is set to an unspecified element of |E|), and 
 initializes $M$ with $m$.}*/


 map(E x) : xdef(x) { }
 /*{\Mcreate 
 creates an injective function $m$ from $I$ to the set of unused variables of
 type $E$, sets |xdef| to |x|, and initializes $M$
 with $m$.}*/

 map(E x,int table_sz) : ch_map(table_sz), xdef(x) { }
 /*{\Mcreate as above, but uses an initial table size of |table_sz|
             instead of the default size $1$. }*/


 map<I,E>& operator=(const map<I,E>& M) 
 { ch_map::operator=((ch_map&)M);
   xdef = M.xdef;
   return *this;
 }

 map(const map<I,E>& M): 
     ch_map((ch_map&)M), xdef(M.xdef) { }




 ~map() { clear_entries(); }


 /*{\Moperations 2 5.0 }*/

 const E& operator[](const I& i) const 
 { item p = lookup(ID_Number(i)); 
   if (p) return LEDA_CONST_ACCESS(E,ch_map::inf(p));
   else return xdef;
  }
  
 E& operator[](const I& i) 
 { return LEDA_ACCESS(E,access(ID_Number(i))); }
 /*{\Marrop        returns the variable $M(i)$ and adds $i$ to |dom(M)|. If |M| is a
 const-object then |M(i)| is read-only and $i$ is not added to |dom(M)|.}*/
  
  
 bool defined(const I& i) const 
          { return lookup(ID_Number(i)) != nil; }
 /*{\Mop      returns true if $i \in |dom(M)|$.}*/


 void clear() { ch_map::clear(); }
 /*{\Mop      makes |\Mvar| empty.}*/

 void clear(const E& x) { ch_map::clear(); xdef = x; }
 /*{\Mop      makes |\Mvar| empty and sets |xdef| to |x|. }*/


 void set_default_value(const E& x) { xdef = x; }
 /*{\Mop      sets |xdef| to |x|. }*/

 E get_default_value() const { return xdef; }
 /*{\Mop      returns the default value |xdef|. }*/

// iteration

 item first_item() const 
 { return ch_map::first_item(); }

 item next_item(item it) const 
 { return ch_map::next_item(it); }

 const E& inf(item it) const
 { return LEDA_CONST_ACCESS(E,ch_map::inf(it)); }


 /*{\Mtext     
 \bigskip
 {\bf Iteration:} }*/

 /*{\Mtext
 {\bf forall}($x,M$) 
 $\{$ ``the entries $M[i]$ with $i \in |dom(M)|$ are 
 successively assigned to $x$'' $\}$ 
 }*/

 /*{\Mtext
 Note that it is \emph{not} possible to iterate over the indices in |dom(M)|.
 If you need this feature use the type |h_array| instead.
 \vspace{1cm}
 }*/


 /*{\Mimplementation
 Maps are implemented by hashing with chaining and table doubling. 
 Access operations |M[i]| take expected time $O(1)$. }*/



 void statistics() const
 { ch_map::statistics(); }


};


template<class I, class E>
class debug_map : private ch_map {
 E xdef;

 void copy_inf(GenPtr& x)   const { LEDA_COPY(E,x);  }
 void clear_inf(GenPtr& x)  const { LEDA_CLEAR(E,x); }
 void init_inf(GenPtr& x)   const { x = leda_copy((E&)xdef); }

 public:
typedef ch_map::item item;
typedef I key_type;
typedef I index_type;
typedef E element_type;

debug_map()  { leda_init_default(xdef); }
debug_map(E x) : xdef(x) { }
debug_map(E x,int table_sz) : ch_map(table_sz), xdef(x) { }
debug_map<I,E>& operator=(const debug_map<I,E>& M) 
{ ch_map::operator=((ch_map&)M);
   xdef = M.xdef;
   return *this;
 }

debug_map(const map<I,E>& M): ch_map((ch_map&)M), xdef(M.xdef) { }

~debug_map() { clear_entries(); }


const E& operator[](const I& i) const 
{ 
//cout << "const operator[]: id = " << ID_Number(i) << endl; 
//cout << i << endl;
   item p = lookup(ID_Number(i)); 
   if (p) return LEDA_CONST_ACCESS(E,ch_map::inf(p));
   else return xdef;
 }
  
 E& operator[](const I& i) 
 { 
//cout << "operator[]: id = " << ID_Number(i) << endl; 
//cout << i << endl;
E& val = LEDA_ACCESS(E,access(ID_Number(i)));
//cout << "val = " << val << endl;
return val;
}
  
bool defined(const I& i) const { return lookup(ID_Number(i)) != nil; }

void clear() { ch_map::clear(); }

void clear(const E& x) { ch_map::clear(); xdef = x; }

void set_default_value(const E& x) { xdef = x; }

E get_default_value() const { return xdef; }

item first_item() const { return ch_map::first_item(); }
item next_item(item it) const { return ch_map::next_item(it); }
const E& inf(item it) const { return LEDA_CONST_ACCESS(E,ch_map::inf(it)); }

 void statistics() const { ch_map::statistics(); }
};


#if LEDA_ROOT_INCL_ID == 600019
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

