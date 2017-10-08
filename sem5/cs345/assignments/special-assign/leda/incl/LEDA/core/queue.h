/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  queue.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_QUEUE_H
#define LEDA_QUEUE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600027
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// queue                                                                
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/slist.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {queue} {E} {Queues}}*/

template<class E>

class queue : private slist<E> {

/*{\Mdefinition
An instance $Q$ of the parameterized data type |\Mname| is
a sequence of elements of data type $E$, called the element
type of $Q$. Elements are inserted at one end (the rear) and deleted at the
other end (the front) of $Q$. The size of $Q$ is the length of the sequence;
a queue of size zero is called the empty queue.}*/

public:

/*{\Mtypes 5}*/ 

typedef E value_type;
/*{\Mtypemember the value type.}*/

// iteration
typedef typename slist<E>::item item; 


// additional type definitions ...
typedef value_type*         pointer;
typedef const value_type*   const_pointer;
typedef value_type&         reference;
typedef const value_type&   const_reference;
typedef int                 size_type;
typedef int                 difference_type;

/*{\Mcreation Q }*/ 

  queue() {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|. |\Mvar| is 
            initialized with the empty queue.}*/

  queue(const queue<E>& Q) : slist<E>(Q) {}
 ~queue() { clear(); }
  queue<E>& operator=(const queue<E>& Q) 
  { slist<E>::operator=(Q); return *this; }


/*{\Moperations 2 4 }*/

const E& top() const { return slist<E>::head(); }
/*{\Mop       returns the front element of |\Mvar|.\\
              \precond $Q$ is not empty.}*/


E pop() { return slist<E>::pop(); }
/*{\Mop       deletes and returns the front element of |\Mvar|.\\
	      \precond $Q$ is not empty.}*/

void append(const E& x) { slist<E>::append(x); }
/*{\Mop       appends $x$ to the rear end of |\Mvar|.}*/

void push(const E& x) { slist<E>::push(x); }
/*{\Mop   inserts $x$ at the front end of |\Mvar|.}*/


int  size() const { return slist<E>::size(); }
/*{\Mop       returns the size of |\Mvar|.}*/

int  length() const { return slist<E>::length(); }
/*{\Mop       returns the size of |\Mvar|.}*/

bool  empty() const { return slist<E>::empty(); }
/*{\Mop       returns true if |\Mvar| is empty, false otherwise.}*/

void clear() { slist<E>::clear(); }
/*{\Mop       makes |\Mvar| the empty queue.}*/


// iteration (s.n. 2006)

const E& inf(item it)   const { return slist<E>::inf(it);       }
item first_item()       const { return slist<E>::first_item();  }
item next_item(item it) const { return slist<E>::next_item(it); }

/*{\Mtext \headerline{Iteration}

{\bf forall}($x,Q$)       
$\{$ ``the elements of $Q$ are successively assigned to $x$'' $\}$ 

}*/


};

/*{\Mimplementation
Queues are implemented by singly linked linear lists. All operations take time 
$O(1)$, except clear which takes time $O(n)$, where $n$ is the size of the 
queue.}*/


#if LEDA_ROOT_INCL_ID == 600027
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
