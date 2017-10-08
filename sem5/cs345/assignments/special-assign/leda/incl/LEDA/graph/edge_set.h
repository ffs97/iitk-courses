/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  edge_set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_EDGE_SET_H
#define LEDA_EDGE_SET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600207
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE


//------------------------------------------------------------------------------
// edge_set  
//------------------------------------------------------------------------------

/*{\Manpage {edge_set} {} {Sets of Edges} }*/

class edge_set 
{
/*{\Mdefinition
An instance $S$ of the data type $edge\_set$ is a subset of
the edges of a graph $G$. $S$ is said to be valid for the
edges of $G$.}*/

  graph* g;
  list<edge> L;
  edge_map<list_item> A;

public:

/*{\Mcreation S }*/

edge_set(const graph& G) :A(G,nil) { g = (graph*)&G; }
/*{\Mcreate creates an instance $S$ of type $edge\_set$ valid for all edges 
            currently in graph $G$ and initializes it to the empty set.}*/ 


edge_set(const edge_set& S) : g(S.g), L(S.L), A(S.A)
{ list_item it;
  forall_items(it,L) A[L[it]] = it;
 }

edge_set& operator=(const edge_set& S)
{ g = S.g;
  L = S.L;
  A.init(*g,nil);
  list_item it;
  forall_items(it,L) A[L[it]] = it;
  return *this;
}


virtual ~edge_set()  { }



/*{\Moperations 1.3 4 }*/

void insert(edge x)  { if (A[x] == nil) A[x] = L.append(x); }
/*{\Mop     adds edge $x$ to $S$. }*/

void del(edge x)     { if (A[x] != nil) { L.del(A[x]); A[x] = nil;} }
/*{\Mop       removes edge $x$ from $S$. }*/

bool member(edge x) const { return (A[x] != nil); }
/*{\Mop       returns true if $x$ in $S$, false otherwise. }*/

edge choose()  const     { return L.head(); }
/*{\Mop       returns an edge of $S$. }*/

int  size()    const     { return L.length(); }
/*{\Mop       returns the size of $S$. }*/

bool empty()   const     { return L.empty(); }
/*{\Mop       returns true iff $S$ is the empty set. }*/

void clear()             { L.clear(); A.init(*g,nil); }
/*{\Mop       makes $S$ the empty set. }*/



// iteration

typedef list_item item;

item first_item()       const { return L.first(); }
item last_item()        const { return L.last(); }
item next_item(item it) const { return it ? L.succ(it) : 0; }
edge inf(item it)       const { return L[it]; }

};

/*{\Mimplementation
An edge set $S$ for a graph $G$ is implemented by a combination of a 
list  $L$ of edges and an edge array of list\_items 
associating with each edge its position in $L$. All operations 
take constant time, except for clear which takes time $O(|S|)$. The space 
requirement is $O(n)$, where $n$ is the number of edges of $G$.}*/


#if LEDA_ROOT_INCL_ID == 600207
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
