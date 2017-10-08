/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  p_queue.h
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
#define LEDA_ROOT_INCL_ID 600036
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/core/impl/bin_heap.h>

LEDA_BEGIN_NAMESPACE

typedef bin_heap::item pq_item;


/*{\Manpage {p_queue} {P,I} {Priority Queues}}*/

template<class P, class I, class pq_impl = bin_heap>
class p_queue: public pq_impl {


/*{\Mdefinition
An instance $Q$ of the parameterized data type |\Mname| is a collection of items
(type |pq_item|). Every item contains a priority from a linearly ordered type 
$P$ and an information from an arbitrary type $I$. $P$ is called the priority 
type of $Q$ and $I$ is called the information type of $Q$. If $P$ is a user-defined
type, you have to define the linear order by providing the compare 
function (see Section \ref{User Defined Parameter Types}). The number of items 
in $Q$ is called the size of $Q$. If $Q$ has size zero it is called the empty 
priority queue. We use $\Litem{p,i}$ to denote a |pq_item| with priority $p$ and
information $i$.\\
Remark: Iteration over the elements of $Q$ using iteration macros such as |forall|
is not supported.}*/


const leda_cmp_base<P>* cmp_ptr;

int (*cmp_ptr1)(const P&, const P&);

int cmp(GenPtr x, GenPtr y)  const 
{ if (cmp_ptr1)
     return LEDA_CALL2(*cmp_ptr1,P,x,y);
  else
     return LEDA_CALL2(*cmp_ptr,P,x,y);
}

mutable stl_access_pair<P,I>* value_buf;

int  ktype_id;
int  key_type_id() const  { return ktype_id; }

void clear_key(GenPtr& x)    const { LEDA_CLEAR(P,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(P,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }

//void print_key(GenPtr x)     const { LEDA_PRINT(P,x,cout); }
//void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,cout); }

public:

/*{\Mtypes 5}*/ 

typedef typename pq_impl::item item;
/*{\Mtypemember the item type.}*/

typedef typename pq_impl::item pq_item;


typedef P prio_type;
/*{\Mtypemember the priority type.}*/  

typedef I inf_type;
/*{\Mtypemember the information type.}*/ 


/*{\Mcreation Q }*/

p_queue(const leda_cmp_base<P>& cmp)
{ value_buf = 0;
  cmp_ptr  = &cmp; 
  cmp_ptr1 = 0; 
  ktype_id = UNKNOWN_TYPE_ID; 
}

p_queue()
{ value_buf = 0;
  cmp_ptr1 = compare;
  ktype_id = LEDA_TYPE_ID(P); 
}
/*{\Mcreate 
creates an instance |\Mvar| of type |\Mname| based on the linear
order defined by the global compare function |compare(const P&, const P&)| 
and initializes it with the empty priority queue. }*/

p_queue(int n) : pq_impl(n) 
{ value_buf = 0; 
  cmp_ptr1 = compare;
  ktype_id = LEDA_TYPE_ID(P); 
}

p_queue(int (*cmp)(const P&, const P&))
{ value_buf = 0;
  cmp_ptr1 = cmp;
  ktype_id = UNKNOWN_TYPE_ID; 
}
/*{\Mcreate 
creates an instance |\Mvar| of type |\Mname| based on the linear order
defined by the compare function |cmp| and initializes it with the
empty priority queue. \precond |cmp| must define a linear order on |P|. }*/


p_queue(const p_queue<P,I,pq_impl>& Q) : pq_impl(Q) 
{ value_buf = 0;
  cmp_ptr  = Q.cmp_ptr; 
  cmp_ptr1 = Q.cmp_ptr1; 
  ktype_id = Q.ktype_id; }


p_queue<P,I,pq_impl>& operator=(const p_queue<P,I,pq_impl>& Q)
{ pq_impl::operator=(Q); 
  cmp_ptr  = Q.cmp_ptr; 
  cmp_ptr1 = Q.cmp_ptr1; 
  ktype_id = Q.ktype_id; 
  return *this; 
}
 

virtual ~p_queue()  
{ pq_impl::clear();
  if (value_buf) delete value_buf;
}


/*{\Moperations 2 5}*/

const P& prio(pq_item it) const 
{ return LEDA_CONST_ACCESS(P,pq_impl::key(it)); }
/*{\Mop     returns the priority of item $it$.\\
            \precond $it$ is an item in |\Mvar|.}*/

const I& inf(pq_item it) const 
{ return LEDA_CONST_ACCESS(I,pq_impl::inf(it)); }
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in |\Mvar|.}*/

const I& operator[](pq_item it) const
{ return LEDA_CONST_ACCESS(I,pq_impl::inf(it)); }

I& operator[](pq_item it) 
{ return LEDA_ACCESS(I,pq_impl::inf(it));}
/*{\Marrop  returns a reference to the information of item $it$.\\
            \precond $it$ is an item in |\Mvar|.}*/
 

pq_item insert(const P& x, const I& i) 
                         { return pq_impl::insert(leda_cast(x),leda_cast(i)); }
/*{\Mop     adds a new item $\<x,i\>$ to |\Mvar| and returns it.}*/


pq_item find_min() const { return pq_impl::find_min();}
/*{\Mop     returns an item with minimal priority 
	    (nil if |\Mvar| is empty).}*/


P   del_min() 
{ P x = prio(find_min()); pq_impl::del_min(); return x; }
/*{\Mop    removes the item $it$ = |\Mvar|.find\_min() from |\Mvar| 
	   and returns the priority of it.\\
           \precond |\Mvar| is not empty.}*/

void del_item(pq_item it) { pq_impl::del_item(it); }
/*{\Mop    removes the item $it$ from |\Mvar|.\\
	   \precond $it$ is an item in |\Mvar|.}*/

void change_inf(pq_item it, const I& i) 
{ pq_impl::change_inf(it,leda_cast(i)); }
/*{\Mop    makes $i$ the new information of item $it$.\\
	   \precond $it$ is an item in |\Mvar|.}*/

void decrease_p(pq_item it, const P& x)
{ pq_impl::decrease_key(it,leda_cast(x)); }
/*{\Mopl    makes $x$ the new priority of item $it$.\\
	    \precond $it$ is an item in |\Mvar| and $x$
	    is not larger then $prio(it)$.}*/


int  size()    const { return pq_impl::size(); }
/*{\Mop     returns the size of |\Mvar|.}*/

bool empty()   const { return (size()==0) ? true : false; }
/*{\Mop     returns true, if |\Mvar| is empty, false otherwise.}*/

void clear() {pq_impl::clear();}
/*{\Mop     makes |\Mvar| the empty priority queue. }*/


pq_item first_item() const { return pq_impl::first_item(); }
pq_item last_item()  const { return pq_impl::last_item(); }
pq_item next_item(pq_item it) const { return pq_impl::next_item(it); }


typedef p_queue<P,I,pq_impl> this_type;

#if defined(LEDA_STL_ITERATORS)

typedef stl_access_pair<P,I> pair_type;

pair_type& stl_item_access(pq_item it) const
{ if (value_buf) delete value_buf;
  value_buf = new pair_type(prio(it),operator[](it)); 
  return *value_buf;
}

pq_item stl_next_item(pq_item it) const { return pq_impl::stl_next_item(it); }
pq_item stl_pred_item(pq_item it) const { return pq_impl::stl_pred_item(it); }

DEFINE_STL_ITERATORS(this_type,pq_item,pair_type)

#endif

};


/*{\Mimplementation
Priority queues are implemented by binary heaps \cite{Wi64}. Operations
insert, del\_item, del\_min take time $O(\log n)$, find\_min, decrease\_p, 
prio, inf, empty take time $O(1)$ and clear takes time $O(n)$, where $n$ is the 
size of |\Mvar|. The space requirement is $O(n)$.}*/


/*{\Mexample
Dijkstra's Algorithm (cf. section \ref{Graph and network algorithms})}*/


#if LEDA_ROOT_INCL_ID == 600036
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
