/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  b_prio.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BPRIO_H
#define LEDA_BPRIO_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600026
#include <LEDA/internal/PREAMBLE.h>
#endif

//------------------------------------------------------------------------------
// b_priority_queues: bounded priority queues implemented by b_heaps 
//------------------------------------------------------------------------------

#include <LEDA/core/impl/b_heap.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {b_priority_queue} {I} {Bounded Priority Queues}}*/

typedef b_heap_item b_pq_item;

template<class I> 

class b_priority_queue : private b_heap 
{

/*{\Mdefinition 
An instance $Q$ of the parameterized data type |\Mname| is a collection
of items (type $b\_pq\_item$). Every item contains a priority from 
a fixed interval $[a..b]$ of integers (type |int|) and an information from
an arbitrary type $I$. The number of items 
in $Q$ is called the size of $Q$. If $Q$ has size zero it is called the empty 
priority queue. We use $\Litem{p,i}$ to denote a |b_pq_item| with priority $p\in [a..b]$ and
information $i$.\\
Remark: Iteration over the elements of $Q$ using iteration macros such as |forall|
is not supported.}*/



void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }
void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,cout); }

public:

/*{\Mcreation Q}*/

b_priority_queue(int a, int b): b_heap(a,b)  {}

/*{\Mcreate creates an instance |\Mvar| of type |\Mname| with key type 
            $[a..b]$ and initializes it with the empty priority queue. }*/

b_priority_queue(const b_priority_queue& Q) : b_heap(Q) {}

b_priority_queue& operator=(const b_priority_queue& Q)
{
	if(this != &Q)
	{
		this->b_heap::operator=(Q);
	}
	return *this;
}

virtual ~b_priority_queue()  { clear(); }


/*{\Moperations 2 5}*/


b_pq_item insert(int key, const I& inf)
{ return b_heap::insert(key,leda_cast(inf)); }
/*{\Mop                 adds a new item $<key,inf>$ to $Q$ and returns it.\\
                        \precond $key \in [a..b]$}*/

void decrease_key(b_pq_item it,int newkey)
{ b_heap::decrease_key(it,newkey); }
/*{\Mop                 makes |newkey| the new priority of item $it$.\\
                        \precond $it$ is an item in $Q$, $newkey \in [a..b]$ and
                        |newkey| is not larger than |prio(it)|.}*/

void del_item(b_pq_item x) { b_heap::delete_item(x); }
/*{\Mop                  deletes item $it$ from $Q$.\\
                        \precond $it$ is an item in $Q$.}*/

int      prio(b_pq_item x)  { return b_heap::key(x); }
/*{\Mop                   returns the priority of item $i$.\\
                          \precond $it$ is an item in $Q$.}*/

const I& inf(b_pq_item x)  { return LEDA_CONST_ACCESS(I,b_heap::inf(x)); }
/*{\Mop                   returns the information of item $i$.\\
                          \precond $it$ is an item in $Q$.}*/

b_pq_item find_min()       { return b_heap::find_min(); }
/*{\Mop                   returns an item with minimal priority ($nil$ if $Q$ is empty).}*/

I del_min()                { I i = inf(find_min()); b_heap::del_min(); return i; }
/*{\Mop                    deletes the item |it = Q.find_min()| from $Q$ and returns the
                          information of $it$.\\
                          \precond $Q$ is not empty.}*/

void clear()               { b_heap::clear(); }
/*{\Mop                 makes $Q$ the empty bounded prioriy queue.}*/

int size() const           { return b_heap::size(); }
/*{\Mop                 returns the size of $Q$.}*/

bool empty()                { return (find_min()==0) ? true : false; }
/*{\Mop                    returns true if $Q$ is empty, false otherwise.}*/

int lower_bound() const      { return b_heap::lower_bound();}
/*{\Mop                    returns the lower bound of the priority interval $[a..b]$.}*/

int upper_bound() const       { return b_heap::upper_bound();}
/*{\Mop                   returns the upper bound of the priority intervall $[a..b]$.}*/

	void print()  { b_heap::print();}
};

 
/*{\Mimplementation
Bounded priority queues are implemented by arrays of linear lists.
Operations insert, find\_min, del\_item, decrease\_key, key, inf,
and empty take time $O(1)$, del\_min (=  del\_item for the minimal
element) takes time $O(d)$, where $d$ is the distance of the minimal
element to the next bigger element in the queue (= $O(b-a)$ in the
worst case). clear takes time $O(b-a+n)$ and the space requirement is
$O(b-a+n)$, where $n$ is the current size of the queue.}*/


#if LEDA_ROOT_INCL_ID == 600026
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

