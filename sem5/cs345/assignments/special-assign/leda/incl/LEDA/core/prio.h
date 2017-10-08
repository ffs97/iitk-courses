/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  prio.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PRIORITY_QUEUE_H
#define LEDA_PRIORITY_QUEUE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600038
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/core/impl/f_heap.h>

LEDA_BEGIN_NAMESPACE

typedef f_heap::item pq_item;


/*{\Manpage {priority_queue} {K,I} {Old-Style Priority Queues}}*/

template<class K, class I, class prio_impl=f_heap> 

class priority_queue: private prio_impl
{

/*{\Mdefinition An instance $Q$ of the parameterized data type
|\Mname| is a collection of items (type |pq_item|). Every item
contains a key from type $K$ and an information from the linearly
ordered type $I$. $K$ is called the key type of $Q$ and $I$ is called
the information type of $Q$. The number of items in $Q$ is called the
size of $Q$. If $Q$ has size zero it is called the empty priority
queue. We use $\Litem{k,i}$ to denote a |pq_item| with key $k$ and
information $i$. \\
Remark: Iteration over the elements of $Q$ using iteration macros 
such as |forall| is not supported.

The type |\Mname| is identical to the type |p_queue| except that the meanings
of $K$ and $I$ are interchanged. We now believe that the semantics of
|p_queue| is the more natural one and keep |\Mname| only for historical 
reasons. We recommend to use |p_queue| instead.}*/

int  key_type_id() const { return LEDA_TYPE_ID(I); }

int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(I,x,y); }
void clear_key(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(K,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(I,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(K,x); }

//void print_key(GenPtr x)     const { LEDA_PRINT(I,x,cout); }
//void print_inf(GenPtr x)     const { LEDA_PRINT(K,x,cout); }

public:

typedef typename prio_impl::item item;

typedef typename prio_impl::item pq_item;


/*{\Mcreation Q }*/

 priority_queue()  {}
/*{\Mcreate 
 creates an instance |\Mvar| of type |\Mname| and initializes it with the
 empty priority queue. }*/

 priority_queue(int n) : prio_impl(n) {}

 priority_queue(const priority_queue<K,I,prio_impl>& Q):prio_impl(Q) {}

virtual ~priority_queue()  { prio_impl::clear(); }

priority_queue<K,I,prio_impl>& operator=(const priority_queue<K,I,prio_impl>& Q)
{ return (priority_queue<K,I,prio_impl>&)prio_impl::operator=(Q); }


/*{\Moperations 2 5.5}*/

const K& key(pq_item it) const
{ return LEDA_CONST_ACCESS(K,prio_impl::inf(it));}
/*{\Mop     returns the key of item $it$.\\
            \precond $it$ is an item in |\Mvar|.}*/

const I& inf(pq_item it) const
{ return LEDA_CONST_ACCESS(I,prio_impl::key(it));}
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in |\Mvar|.}*/

pq_item insert(const K& k, const I& i) 
                              { return prio_impl::insert(leda_cast(i),leda_cast(k)); }
/*{\Mop     adds a new item $\Litem{k,i}$ to |\Mvar| and returns it.}*/


pq_item find_min() const { return prio_impl::find_min();}
/*{\Mop     returns an item with minimal information 
	    (nil if |\Mvar| is empty).}*/


K   del_min() 
{ K x = key(find_min()); prio_impl::del_min(); return x; }
/*{\Mop    removes the item $it$ = |\Mvar|.find\_min() from \Mvar
	   and returns the key of $it$.\\
           \precond |\Mvar| is not empty.}*/

void del_item(pq_item it) { prio_impl::del_item(it); }
/*{\Mop    removes the item $it$ from |\Mvar|.\\
	   \precond $it$ is an item in |\Mvar|.}*/

void change_key(pq_item it, const K& k) 
{ prio_impl::change_inf(it,leda_cast(k)); }
/*{\Mop    makes $k$ the new key of item $it$.\\
	   \precond $it$ is an item in |\Mvar|.}*/

void decrease_inf(pq_item it, const I& i)
{ prio_impl::decrease_key(it,leda_cast(i)); }
/*{\Mop     makes $i$ the new information of item $it$.\\
	    \precond $it$ is an item in |\Mvar| and $i$
	    is not larger then $inf(it)$.}*/


int  size()    const { return prio_impl::size(); }
/*{\Mop     returns the size of |\Mvar|.}*/

bool empty()   const { return (size()==0) ? true : false; }
/*{\Mop     returns true, if |\Mvar| is empty, false otherwise}*/

void clear() {prio_impl::clear();}
/*{\Mop     makes |\Mvar| the empty priority queue. }*/


pq_item first_item() const { return prio_impl::first_item(); }
pq_item next_item(pq_item it) const { return prio_impl::next_item(it); }

};


/*{\Mimplementation
Priority queues are implemented by Fibonacci heaps \cite{FT87}. Operations
insert, del\_item, del\_min take time $O(\log n)$, find\_min, decrease\_inf, 
key, inf, empty take time $O(1)$ and clear takes time $O(n)$, where $n$ is the 
size of |\Mvar|. The space requirement is $O(n)$.}*/


/*{\Mexample
Dijkstra's Algorithm (cf. section \ref{Graph and network algorithms})}*/


#if LEDA_ROOT_INCL_ID == 600038
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
