/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  h_array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_H_ARRAY_H
#define LEDA_H_ARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600021
#include <LEDA/internal/PREAMBLE.h>
#endif

//----------------------------------------------------------------
// h_array  
//----------------------------------------------------------------

#include <LEDA/system/basic.h> 
#include <LEDA/core/impl/ch_hash.h> 

LEDA_BEGIN_NAMESPACE


/*{\Manpage {h_array} {I,E} {Hashing Arrays} }*/

template<class I, class E>

#if defined(__DMC__)
class h_array : public ch_hash {
#else
class h_array : private ch_hash {
#endif

/*{\Mdefinition
An instance $A$ of the parameterized data type |\Mname| (hashing array)
is an injective mapping from a hashed data type $I$ , called the index type of $A$, 
to the set of variables of arbitrary type $E$, called the element type of $A$. We use $A(i)$ to 
denote the variable indexed by $i$ and  we use $\mathit{dom}(A)$ to denote 
the set of ``used indices''. This set is empty at the time of creation and 
is modified by array accesses. Each hashing array has an associated default 
value |xdef|. The variable $A(i)$ has value |xdef| for all $i \not\in dom(A)$.
If $I$ is a user-defined type, you have to provide a Hash function 
(see Section~\ref{User Defined Parameter Types}).

Related data types are |d_arrays|, |maps|, and |dictionaries|.}*/

protected: 

E xdef;

bool equal_key(GenPtr x, GenPtr y) const { return LEDA_EQUAL(I,x,y); }

int  hash_fct(GenPtr x)      const { return LEDA_HASH(I,x);    }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(E,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(I,x);  }
void copy_inf(GenPtr& x)     const { LEDA_COPY(E,x);  }

int  key_type_id() const { return LEDA_TYPE_ID(I); }

public:

typedef ch_hash_item item;

/*{\Mcreation A }*/

h_array()  { leda_init_default(xdef); }
/*{\Mcreate 
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, sets |xdef| to the default value of type |E| (if |E| has no default 
value then |xdef| stays undefined) and |dom(A)| to the empty set, and 
initializes $A$ with $a$.}*/

h_array(E x) : xdef(x) {}
/*{\Mcreate 
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, sets |xdef| to |x| and |dom(A)| to the empty set, and initializes $A$
with $a$.}*/

h_array(E x,int table_sz) : ch_hash(table_sz), xdef(x) { }
/*{\Mcreate as above, but uses an initial table size of |table_sz|
            instead of the default size $1$. }*/


h_array<I,E>& operator=(const h_array<I,E>& A)
{ ch_hash::operator=(A); xdef=A.xdef; return *this; }

h_array(const h_array<I,E>& A) : ch_hash(A),xdef(A.xdef) {}


virtual ~h_array() { /* base class constructor does the job */ }   


/*{\Moperations 2 4 }*/

const E&  operator[](const I& i)  const
{ ch_hash_item it = ch_hash::lookup(leda_cast(i));
  if (it) return LEDA_CONST_ACCESS(E,ch_hash::inf(it));
  else return xdef;
}

E&  operator[](const I& i) 
{ ch_hash_item it = ch_hash::lookup(leda_cast(i));
  if (it == nil) 
  { GenPtr p = leda_cast(xdef);
    it = ch_hash::fast_insert(leda_cast(i),p);
   }
  return LEDA_ACCESS(E,ch_hash::inf(it)); 
}
/*{\Marrop    returns the variable $A(i)$.}*/

bool defined(const I& i)  const 
{ return (ch_hash::lookup(leda_cast(i)) != nil); }
/*{\Mop      returns true if $i \in \mathit{dom}(A)$ and false otherwise.}*/



void undefine(const I& i)
{ ch_hash::del(leda_cast(i)); }
/*{\Mop      removes $i$ from $\mathit{dom}(A)$ and sets |A(i)| to |xdef|. }*/


void clear() { ch_hash::clear(); }
/*{\Mop      makes |dom(A)| empty.}*/

void clear(const E& x) { ch_hash::clear(); xdef = x; }
/*{\Mop      makes |dom(A)| empty and sets |xdef| to |x|.}*/


int  size() const { return ch_hash::size(); }
/*{\Mop      returns $\Labs{dom(A)}$.}*/

bool empty() const {return ch_hash::empty(); }
/*{\Mop      returns true if $A$ is empty, false otherwise.}*/

void set_default_value(const E& x) { xdef = x; }
/*{\Mop      sets |xdef| to |x|. }*/


// iteration

item first_item()       const { return ch_hash::first_item(); }
item next_item(item it) const { return it ? ch_hash::next_item(it) : 0; }

const I& key(item it) const { return LEDA_CONST_ACCESS(I,ch_hash::key(it)); }
const E& inf(item it) const { return LEDA_CONST_ACCESS(E,ch_hash::inf(it)); }

};


/*{\Mtext     
\bigskip
{\bf Iteration} }*/

/*{\Mtext
{\bf forall\_defined}($i,A$) 
$\{$ ``the elements from $\mathit{dom}(A)$ are successively assigned to $i$'' $\}$ \\
Remark: the current element may not be deleted resp. declared undefined 
during execution of the loop.}*/

/*{\Mtext
{\bf forall}($x,A$) 
$\{$ ``for all $i \in \mathit{dom}(A)$ the entries $A[i]$ are 
successively assigned to $x$'' $\}$.}*/



/*{\Mimplementation
Hashing arrays are implemented by hashing with chaining. Access operations 
take expected time $O(1)$. In many cases, hashing arrays are more efficient 
than dictionary arrays (cf. \ref{Dictionary Arrays}).}*/

#if LEDA_ROOT_INCL_ID == 600021
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

