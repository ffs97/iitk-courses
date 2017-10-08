/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  b_stack.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BSTACK_H
#define LEDA_BSTACK_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600025
#include <LEDA/internal/PREAMBLE.h>
#endif

//------------------------------------------------------------------------------
// bounded stacks 
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {b_stack} {E} {Bounded Stacks}}*/


template<class E> 

class b_stack
{
/*{\Mdefinition
An instance $S$ of the parameterized data type |\Mname| is a stack
(see section \ref{Stacks}) of bounded size.}*/

E* first;  // start of array
E* stop;   // one position behind last element
E* free;   // pointer to first free element

public:

/*{\Mcreation S }*/


b_stack(int n)
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| that can hold up to $n$ 
            elements.  |\Mvar| is initialized with the empty stack.}*/
{
#if !defined(LEDA_CHECKING_OFF)
  if (n<1) LEDA_EXCEPTION(99,"bounded stack: bad size");
#endif
  free = first = new E[n];
  stop = first + n;
  if (first==0) 
  LEDA_EXCEPTION(99,"bounded stack: out of memory");
 }


virtual ~b_stack() { delete [] first; }

/*{\Moperations 2 4}*/

E& top()
{
#if !defined(LEDA_CHECKING_OFF)
  if (free==first) LEDA_EXCEPTION(99,"bounded stack: top on empty stack");
#endif
  return *(free-1);
}

const E& top() const
{
#if !defined(LEDA_CHECKING_OFF)
  if (free==first) LEDA_EXCEPTION(99,"bounded stack: top on empty stack");
#endif
  return *(free-1);
}

/*{\Mop     returns the top element of |\Mvar|.\\
	    \precond $S$ is not empty.}*/

const E& pop()
{
#if !defined(LEDA_CHECKING_OFF)
  if (free==first) LEDA_EXCEPTION(99,"b_stack::pop underflow");
#endif
return *--free;
}
/*{\Mop   deletes and returns the top element of |\Mvar|.\\
	  \precond $S$ is not empty.}*/

void push(const E& x)
{
#if !defined(LEDA_CHECKING_OFF)
  if (free==stop) LEDA_EXCEPTION(99,"b_stack::push overflow");
#endif
  *free++ = x;
}
/*{\Mop  adds $x$ as new top element to |\Mvar|.\\
	 \precond $S$.size() $< n$.}*/

void clear() { free = first; }
/*{\Mop   makes |\Mvar| the empty stack.}*/

int   size()  const { return int(free - first); }
/*{\Mop    returns the size of |\Mvar|.}*/

bool   empty() const { return (free==first) ? true : false; }
/*{\Mop  returns true if |\Mvar| is empty, false otherwise.}*/

};

/*{\Mimplementation
Bounded stacks are implemented by \CC vectors. All operations take
time $O(1)$. The space requirement is $O(n)$.}*/


#if LEDA_ROOT_INCL_ID == 600025
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
