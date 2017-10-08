/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  int_set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_INTSET_H
#define LEDA_INTSET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600048
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
/* int_set: integer sets implemented by bit vectors                           */
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

// #define SIZE_OF_ULONG  (8 * sizeof(unsigned long))


/*{\Manpage {int_set} {} {Integer Sets}}*/

class __exportC int_set {

//static const int SIZE_OF_ULONG = 8 * sizeof(unsigned long); // msc6 problem 
enum { SIZE_OF_ULONG = 8 * sizeof(unsigned long) };

/*{\Mdefinition
An instance $S$ of the data type |\Mtype| is a subset of a fixed interval
$[a..b]$ of the integers, called the range of $S$. }*/


  unsigned long*  V;
  int sz;
  int low;

public:

/*{\Mcreation S }*/

int_set(int a, int b); 
/*{\Mcreate creates an instance |\Mvar| of type |\Mtype| for elements from 
            $[a..b]$ and initializes it to the empty set.}*/

int_set(int n); 
/*{\Mcreate creates an instance |\Mvar| of type |\Mtype| for elements from 
            $[0..n-1]$ and initializes it to the empty set.}*/


int_set(const int_set&);

int_set& operator=(const int_set& T);

~int_set() { delete[] V; } 


/*{\Moperations 1.8 5 }*/

void insert(int x);
/*{\Mop      adds $x$ to |\Mvar|.\\
	         \precond $a\le x\le b$.}*/

void del(int x);
/*{\Mop      deletes $x$ from |\Mvar|.\\
             \precond $a\le x\le b$.}*/

bool member(int x) const;
/*{\Mop      returns true if $x$ in |\Mvar|, false otherwise.\\
	         \precond $a\le x\le b$.}*/

int min();
/*{\Mop      returns the minimal integer in the range of of |\Mvar|.}*/

int max();
/*{\Mop      returns the maximal integer in the range of of |\Mvar|.}*/

void clear();
/*{\Mop      makes |\Mvar| the empty set.}*/

/*{\Mtext In any binary operation below, |\Mvar| and $T$ must have the same range:}*/

int_set& join(const int_set& T);
/*{\Mop      replaces |\Mvar| by $|\Mvar| \cup T$ and returns it. }*/

int_set& intersect(const int_set& T);
/*{\Mop      replaces |\Mvar| by $|\Mvar| \cap T$ and returns it. }*/

int_set& diff(const int_set& T);
/*{\Mop      replaces |\Mvar| by $|\Mvar| \setminus T$ and returns it. }*/

int_set& symdiff(const int_set& T);
/*{\Mop      replaces |\Mvar| by $(|\Mvar| \setminus T) \cup (T \setminus |\Mvar|)$ and returns it. }*/

int_set& complement();
/*{\Mop      replaces |\Mvar| by $[a..b] \setminus |\Mvar|$ and returns it. }*/

int_set& operator|=(const int_set&);
int_set& operator&=(const int_set&);
int_set& operator-=(const int_set&);
int_set& operator%=(const int_set&);

int_set  operator|(const int_set& T);
/*{\Mbinop   returns the union of |\Mvar| and $T$.}*/

int_set  operator&(const int_set& T);
/*{\Mbinop   returns the intersection of |\Mvar| and $T$.}*/

int_set  operator-(const int_set& T);
/*{\Mbinop   returns the set difference of |\Mvar| and $T$.}*/

int_set  operator%(const int_set& T);
/*{\Mbinop   returns the symmetric difference of |\Mvar| and $T$.}*/

int_set  operator~();
/*{\Munop    returns the complement of |\Mvar|, i.e. $[a..b] \setminus |\Mvar|$.}*/

};



inline int int_set::min() { return low; }

inline int int_set::max() { return low+sz-1; }


inline bool int_set::member(int x)  const
{ int i = x-low; 
  return (V[i/SIZE_OF_ULONG] & (1UL << (i%SIZE_OF_ULONG))) != 0; 
 }

inline void int_set::insert(int x) 
{ int i  =  x-low; 
  V[i/SIZE_OF_ULONG] |= (1UL << (i%SIZE_OF_ULONG)); 
 }

inline void int_set::del(int x)    
{ int i   = x-low; 
  V[i/SIZE_OF_ULONG] &= ~(1UL << (i%SIZE_OF_ULONG)); 
 }

inline int_set& int_set::operator|=(const int_set& s) { return join(s); }

inline int_set& int_set::operator&=(const int_set& s) { return intersect(s); }

inline int_set& int_set::operator-=(const int_set& s) { return diff(s); }

inline int_set& int_set::operator%=(const int_set& s) { return symdiff(s); }


/*{\Mimplementation
Integer sets are implemented by bit vectors. Operations insert, delete,
member, min and max take constant time. All other operations
take time $O(b-a+1)$.}*/


#if LEDA_ROOT_INCL_ID == 600048
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
