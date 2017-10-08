/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  b_queue.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BQUEUE_H
#define LEDA_BQUEUE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600015
#include <LEDA/internal/PREAMBLE.h>
#endif

//------------------------------------------------------------------------------
// bounded queues
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {b_queue} {E} {Bounded Queues}}*/

template<class E> 

class b_queue  
{
/*{\Mdefinition
An instance $Q$ of the parameterized data type |\Mname| is a (double ended)
queue (see section \ref{Queues}) of bounded size.}*/


E* first;     // first element of array
E* stop;      // one position behind last element of array
E* start;     // current start of queue (wraps around)
E* end;       // one position behind end of queue (wraps around)

public:										

typedef E* item;


/*{\Mcreation Q}*/


b_queue(int n) 
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| that can hold up to $n$ 
           elements.  |\Mvar| is initialized with the empty queue.}*/
{
#if !defined(LEDA_CHECKING_OFF)
  if (n<1) LEDA_EXCEPTION(88,"_b_queue: bad size");
#endif
  first = new E[n+1];
  if (first==0) LEDA_EXCEPTION(88,"_b_queue: out of memory");
  stop  = first+n+1;
  start = end = first; 
}


b_queue() : first(0),stop(0),start(0),end(0) {} 


void share(b_queue<E>& Q)
{ first = Q.first;
  stop = Q.stop;
  start = end = first;
}

void unshare() { first = 0; }


virtual ~b_queue() { if (first) delete [] first; }

/*{\Moperations 2 5}*/

const E& front() const
{
#if !defined(LEDA_CHECKING_OFF)
  if (start==end) LEDA_EXCEPTION(88, "_b_queue empty");
#endif
  return *start;
}
/*{\Mop  returns the first element of |\Mvar|.\\
         \precond $Q$ is not empty.}*/


const E& back() const
{
#if !defined(LEDA_CHECKING_OFF)
  if (start==end) LEDA_EXCEPTION(88, "b_queue::back empty");
#endif
  E* p = (end == first) ? stop : end;
  return *(p-1);
}
/*{\Mop  returns the last element of |\Mvar|.\\
         \precond $Q$ is not empty.}*/



const E& pop_front()
{
#if !defined(LEDA_CHECKING_OFF)
 if (start==end) LEDA_EXCEPTION(88, "b_queue::pop underflow");
#endif
  E* p = start++;
  if (start == stop) start = first;
  return *p;
}
/*{\Mop  deletes and returns the first element of |\Mvar|.\\
         \precond $Q$ is not empty.}*/


const E& pop_back()
{
#if !defined(LEDA_CHECKING_OFF)
 if (start==end) LEDA_EXCEPTION(88, "b_queue::pop_back underflow");
#endif
  if (end == first) end = stop;
  return *--end;
}
/*{\Mop  deletes and returns the last element of |\Mvar|.\\
         \precond $Q$ is not empty.}*/


void push_front(const E& x)
{ if (start == first) start = stop;
  *--start = x;
#if !defined(LEDA_CHECKING_OFF)
  if (start==end) LEDA_EXCEPTION(88, "b_queue::push overflow");
#endif
}
/*{\Mop   inserts $x$ at the beginning of |\Mvar|.\\
	  \precond $Q$.size()$ < n$.}*/


void push_back(const E& x)
{ *end = x;
  if (++end == stop) end = first;
#if !defined(LEDA_CHECKING_OFF)
  if (start==end) LEDA_EXCEPTION(88, "b_queue::append overflow");
#endif
}
/*{\Mop   inserts $x$ at the end of |\Mvar|.\\
	  \precond $Q$.size()$ < n$.}*/

void append(const E& x) { push_back(x);  }
/*{\Mop   same as |\Mvar.push_back()|. }*/


void clear() { start = end = first; }
/*{\Mop    makes |\Mvar| the empty queue.}*/


int size() const 
{ int s = int(end-start);
  return (s<0) ? int(stop-first+s) : s;
}
/*{\Mop    returns the size of |\Mvar|.}*/

int length() const { return size(); }
/*{\Mop    same as |\Mvar.size()|.}*/


bool empty() const { return start == end; }
/*{\Mop    returns true if |\Mvar| is empty, false otherwise.}*/




/*{\Mtext \headerline{Stack Operations}}*/

const E& top() const { return front(); }
/*{\Mop   same as |\Mvar.front()|. }*/

const E& pop() { return pop_front(); }
/*{\Mop   same as |\Mvar.pop_front()|. }*/

void push(const E& x) { push_front(x); }
/*{\Mop   same as |\Mvar.push_front()|. }*/



// iteration (s.n. 2006)

const E& inf(item p) const { return *p; }

item first_item() const { return (start == end) ? 0 : start; }

item last_item() const 
{ if (start == end) return 0;
  else return (end == first) ? stop-1 : end-1; 
}

item next_item(item p) const
{ if (p == 0) return 0;
  if (++p == stop) p = first;
  return (p == end) ? 0 : p;
}

item pred_item(item p) const
{ if (p == 0 || p == start) return 0;
  if (p == first) p = stop;
  return p-1;
}

/*{\Mtext \headerline{Iteration}

{\bf forall}($x,Q$)       
$\{$ ``the elements of $Q$ are successively assigned to $x$'' $\}$ 

}*/


};


/*{\Mimplementation
Bounded queues are implemented by circular arrays. All operations take 
time $O(1)$. The space requirement is $O(n)$.}*/

#if LEDA_ROOT_INCL_ID == 600015
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
