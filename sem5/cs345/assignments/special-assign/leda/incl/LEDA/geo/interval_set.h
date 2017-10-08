/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  interval_set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_INTERVAL_SET_H
#define LEDA_INTERVAL_SET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600080
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/d2_dictionary.h>

LEDA_BEGIN_NAMESPACE

typedef dic2_item is_item;

/*{\Manpage {interval_set} {I} {Sets of Intervals}}*/

template <class I>
class interval_set : public d2_dictionary<double,double,I> {

typedef d2_dictionary<double,double,I> base;

/*{\Mdefinition
An instance $S$ of the parameterized data type |\Mname| is a
collection of items ($is\_item$). Every item in $S$ contains a closed
interval of the $double$ numbers as key and an information from data type $I$,
called the information type of $S$. The number of items in $S$ is called
the size of $S$. An interval set of size zero is said to be empty. We use
$\<x,y,i\>$ to denote the item with interval $[x,y]$ and information $i$;
$x$ ($y$) is called the left (right) boundary of the item. For each
interval $[x,y]$ there is at most one item $\<x,y,i\> \in S$.}*/

public:

/*{\Mcreation S }*/

interval_set()  {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes |\Mvar| 
            to the empty set.}*/

interval_set(const interval_set<I>& S)
{ is_item it;
  forall_dic2_items(it,S) 
      insert(S.left(it),S.right(it),S.inf(it));
 }

~interval_set()  {}


/*{\Moperations 2.35 5 }*/

double    left(is_item it)  const { return base::key1(it); }
/*{\Mop      returns the left boundary of item $it$.\\
	     \precond $it$ is an item in |\Mvar|.}*/

double    right(is_item it) const { return base::key2(it); }
/*{\Mop      returns the right boundary of item $it$.\\
	     \precond $it$ is an item in |\Mvar|.}*/

const I& inf(is_item it) const  { return base::inf(it); }
/*{\Mop      returns the information of item $it$.\\
	     \precond $it$ is an item in |\Mvar|.}*/

is_item insert(double x, double y, const I& i)
                      //{ return base::insert(x,y,leda_copy(i)); }
                      { return base::insert(x,y,i); }
/*{\Mopl     associates the information $i$ with interval 
	     $[x,y]$. If there is an item $\<x,y,j\>$ in \Mvar
	     then $j$ is replaced by $i$, else a new item
	     $\<x,y,i\>$ is added to $S$. In both cases
	     the item is returned.}*/

is_item lookup(double x, double y) const {return base::lookup(x,y);}
/*{\Mop      returns the item with interval $[x,y]$ 
	     (nil if no such item exists in \Mvar).}*/


list<is_item>  const intersection(double a, double b)
{ list<dic2_item> l = base::range_search(-MAXDOUBLE,b,a,MAXDOUBLE);
  return *((list<is_item>*)&l);
}
/*{\Mopl     returns all items $\<x,y,i\>\ \in\ S$ with 
	     $[x,y] \cap [a,b] \ne \emptyset$.}*/

void del(double x, double y) {base::del(x,y);}
/*{\Mop      deletes the item with interval $[x,y]$ from |\Mvar|.}*/

void del_item(is_item it) {base::del_item(it);}
/*{\Mop      removes item $it$ from |\Mvar|.\\
	     \precond $it$ is an item in |\Mvar|.}*/

void    change_inf(is_item it, const I& i)
                      { base::change_inf(it,i); }
/*{\Mop      makes $i$ the information of item $it$.\\
	     \precond $it$ is an item in |\Mvar|.}*/

void clear() {base::clear();}
/*{\Mop      makes |\Mvar| the empty interval\_set.}*/

bool empty() const {return base::empty();}
/*{\Mop      returns true iff |\Mvar| is empty.}*/

int size() const {return base::size();}
/*{\Mop      returns the size of |\Mvar|.}*/

};

#define forall_is_items(i,D) forall_dic2_items(i,D)

/*{\Mimplementation
Interval sets are implemented by two-dimensional range trees \cite{Wi85,Lu78}.
Operations insert, lookup, del\_item and del take time $O(\log^2 n)$,
intersection takes time $O(k + \log^2 n)$, where $k$ is the size
of the returned list. Operations left, right, inf, empty, and size
take time $O(1)$, and clear $O(n\log n)$. Here $n$ is always the 
current size of the interval set. The space requirement is $O(n\log n)$.}*/


#if LEDA_ROOT_INCL_ID == 600080
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

