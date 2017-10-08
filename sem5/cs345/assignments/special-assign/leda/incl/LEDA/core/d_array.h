/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d_array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D_ARRAY_H
#define LEDA_D_ARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600020
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// d_array 
//------------------------------------------------------------------------------


/*{\Manpage {d_array} {I,E} {Dictionary Arrays}}*/


#include <LEDA/core/impl/ab_tree.h> 

LEDA_BEGIN_NAMESPACE


template<class I, class E, class da_impl = ab_tree>

class d_array : public da_impl 
{

/*{\Mdefinition
An instance $A$ of the parameterized data type |\Mname| (dictionary
array) is an injective mapping from the linearly ordered data type $I$, called
the index type of $A$, to the set of variables of data type $E$, called the
element type of $A$. We use $A(i)$ to denote the variable with index $i$ and
we use $\mathit{dom}(A)$ to denote the set of ``used indices''. This set is empty at the time of creation and is modified by array accesses.
Each dictionary array has an associated default value |xdef|. The variable 
$A(i)$ has value |xdef| for all $i \not\in dom(A)$. If $I$ is a user-defined type, you
have to provide a compare function (see Section~\ref{User Defined Parameter Types}).


Related data types are |h_arrays|, |maps|, and |dictionaries|. }*/


protected: 

E xdef;

int cmp(GenPtr x, GenPtr y)  const { return LEDA_COMPARE(I,x,y); }

// for implementations based on hashing 

bool equal_key(GenPtr x, GenPtr y) const { return LEDA_EQUAL(I,x,y); }
int  hash_fct(GenPtr x)            const { return LEDA_HASH(I,x); }

void clear_key(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(E,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(I,x);  }
void copy_inf(GenPtr& x)     const { LEDA_COPY(E,x);  }

int  key_type_id() const { return LEDA_TYPE_ID(I); }

public:

/*{\Mtypes 5}*/ 

typedef typename da_impl::item item;
/*{\Mtypemember the item type.}*/

typedef I key_type;

typedef I index_type;
/*{\Mtypemember the index type.}*/  

typedef E element_type;
/*{\Mtypemember the element type.}*/ 


/*{\Mcreation A }*/

d_array()  { leda_init_default(xdef); }
/*{\Mcreate 
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, sets |xdef| to the default value of type |E| (if |E| has no default 
value then |xdef| stays undefined) and |dom(A)| to the empty set, and 
initializes $A$ with $a$.}*/

d_array(E x) { xdef = x; }
/*{\Mcreate 
creates an injective function $a$ from $I$ to the set of unused variables of
type $E$, sets |xdef| to |x| and |dom(A)| to the empty set, and initializes $A$
with $a$.}*/


d_array<I,E,da_impl>& operator=(const d_array<I,E,da_impl>& A)
{ da_impl::operator=(A); 
  xdef=A.xdef;
  return *this; 
 }

d_array(const d_array<I,E,da_impl>& A) : da_impl(A) { xdef=A.xdef; }

virtual ~d_array() { clear(); }

/*{\Moperations 2 4 }*/

const E&  operator[](const I& i)  const
{ item it = da_impl::lookup(leda_cast(i));
  if (it) return LEDA_CONST_ACCESS(E,da_impl::inf(it));
  else return  xdef; 
}

E&  operator[](const I& i) 
{ item it = da_impl::lookup(leda_cast(i));
  if (it==nil) 
  { GenPtr p = leda_cast(xdef);
    it=da_impl::insert(leda_cast(i),p);
   }
  return LEDA_ACCESS(E,da_impl::inf(it)); 
}
/*{\Marrop    returns the variable $A(i)$.}*/


bool defined(const I& i)  const 
{ return (da_impl::lookup(leda_cast(i))!=nil); }
/*{\Mop      returns true if $i \in \mathit{dom}(A)$ and false otherwise.}*/


void undefine(const I& i)
{ da_impl::del(leda_cast(i)); }
/*{\Mop      removes $i$ from $\mathit{dom}(A)$ and sets |A(i)| to |xdef|. }*/

void clear() { da_impl::clear(); }
/*{\Mop      makes |dom(A)| empty.}*/

int  size() const { return da_impl::size(); }
/*{\Mop      returns $\Labs{dom(A)}$.}*/

 void set_default_value(const E& x) { xdef = x; }
 /*{\Mop      sets |xdef| to |x|. }*/



// iteration

item first_item()       const { return da_impl::first_item(); }
item next_item(item it) const 
{ return it ? da_impl::next_item(it) : 0;}

const I& key(item it) const 
{ return LEDA_CONST_ACCESS(I,da_impl::key(it)); }

const E& inf(item it) const 
{ return LEDA_CONST_ACCESS(E,da_impl::inf(it)); }

};

/*{\Mtext     
\bigskip
{\bf Iteration} }*/


/*{\Mtext
{\bf forall\_defined}($i,A$) 
$\{$ ``the elements from $\mathit{dom}(A)$ are successively assigned to $i$'' $\}$ }*/

/*{\Mtext
{\bf forall}($x,A$) 
$\{$ ``for all $i \in \mathit{dom}(A)$ the entries $A[i]$ are successively assigned to $x$'' $\}$ }*/



/*{\Mimplementation
Dictionary arrays are implemented by $(2,4)$-trees \cite{M84}. 
Access operations $A[i]$ take time $O(\log \mathit{dom}(A))$.
The space requirement is $O(\mathit{dom}(A))$.}*/
  
/*{\Mexample

\textbf{Program 1:}

We use a dictionary array to count the number of occurrences of the elements in a 
sequence of strings.

\begin{Mverb}

#include <LEDA/core/d_array.h>

main()
{ 
  d_array<string,int> N(0);
  string s;

  while (cin >> s) N[s]++;

  forall_defined(s,N) cout << s << "  " << N[s] << endl;

}
\end{Mverb}

\textbf{Program 2:}

We use a |d_array<string,string>| to realize an english/german dictionary.

\begin{Mverb}
#include <LEDA/core/d_array.h>

main()
{ 
  d_array<string,string> dic;

  dic["hello"] = "hallo";
  dic["world"] = "Welt";
  dic["book"]  = "Buch";
  dic["key"]   = "Schluessel";
  
  string s;
  forall_defined(s,dic) cout << s << "  " << dic[s] << endl;

}
\end{Mverb}

}*/


#if LEDA_ROOT_INCL_ID == 600020
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
