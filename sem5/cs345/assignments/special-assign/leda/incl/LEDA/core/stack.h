/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  stack.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_STACK_H
#define LEDA_STACK_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600016
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>
#include <LEDA/core/slist.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// stacks                                                                
//------------------------------------------------------------------------------


/*{\Manpage {stack} {E} {Stacks}}*/ 

template<class E>

class stack : private slist<E> {

/*{\Mdefinition
An instance $S$ of the parameterized data type |\Mname| is
a sequence of elements of data type $E$, called the element
type of $S$. Insertions or deletions of elements take place only at one end of
the sequence, called the top of $S$. The size of $S$ is the length of the
sequence, a stack of size zero is called the empty stack.}*/

public:

/*{\Mcreation S }*/ 

stack() {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname.| |\Mvar| is 
            initialized with the empty stack.}*/

 stack(const stack<E>& S) : slist<E>(S) {}
~stack() { clear(); }

 stack<E>& operator=(const stack<E>& S) 
 { slist<E>::operator=(S); return *this; }


/*{\Moperations 2 4}*/

const E& top() const { return slist<E>::head(); }
/*{\Mop      returns the top element of |\Mvar|.\\
             \precond $S$ is not empty.}*/

void push(const E& x) { slist<E>::push(x); }
/*{\Mop      adds $x$ as new top element to |\Mvar|.}*/


E pop() { return slist<E>::pop(); }
/*{\Mop      deletes and returns the top element of |\Mvar|.\\
             \precond $S$ is not empty.}*/


int  size() const { return slist<E>::length(); }
/*{\Mop      returns the size of |\Mvar|.}*/

bool  empty() const { return slist<E>::empty(); }
/*{\Mop      returns true if |\Mvar| is empty, false otherwise.}*/

void clear() { slist<E>::clear(); }
/*{\Mop      makes |\Mvar| the empty stack.}*/

};

/*{\Mimplementation
Stacks are implemented by singly linked linear lists. All operations take 
time $O(1)$, except clear which takes time $O(n)$, where $n$ is the size of 
the stack.}*/


#if LEDA_ROOT_INCL_ID == 600016
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif


