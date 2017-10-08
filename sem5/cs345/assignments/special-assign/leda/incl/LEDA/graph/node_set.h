/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_SET_H
#define LEDA_NODE_SET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600160
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// node_set  
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {node_set} {} {Sets of Nodes} }*/

class node_set 
{
/*{\Mdefinition
An instance $S$ of the data type $node\_set$ is a subset of
the nodes of a graph $G$. $S$ is said to be valid for the nodes
of $G$.}*/

  graph* g;
  list<node> L;
  node_map<list_item> A;

public:

typedef list_item item;


/*{\Mcreation S }*/

node_set(const graph& G) : A(G,nil) { g = (graph*)&G; }
/*{\Mcreate creates an instance $S$ of type $node\_set$ valid for all 
            nodes currently contained in graph $G$ and initializes it 
            to the empty set.}*/

node_set(const node_set& S) : g(S.g), L(S.L), A(S.A)
{ list_item it;
  forall_items(it,L) A[L[it]] = it; 
 }

node_set& operator=(const node_set& S)
{ g = S.g;
  L = S.L;
  A.init(*g,nil);
  list_item it;
  forall_items(it,L) A[L[it]] = it; 
  return *this;
}

~node_set() {}


/*{\Moperations 1.3 4 }*/

void insert(node x)  { if (A[x] == nil) A[x] = L.append(x); }
/*{\Mop       adds node $x$ to $S$. }*/

void del(node x)     { if (A[x] != nil) { L.del(A[x]); A[x] = nil;} }
/*{\Mop       removes node $x$ from $S$. }*/

bool member(node x) const  { return (A[x] != nil); }
/*{\Mop      returns true if $x$ in $S$, false otherwise. }*/

node choose()  const { return L.head(); }
/*{\Mop      returns a node of $S$. }*/

int  size()    const { return L.length(); }
/*{\Mop      returns the size of $S$. }*/

bool empty()   const { return L.empty(); }
/*{\Mop      returns true iff $S$ is the empty set. }*/

void clear()         
{ node v;
  forall(v,L) A[v] = nil;
  L.clear(); 
 }
/*{\Mop      makes $S$ the empty set. }*/


// iteration

item first_item()       const { return L.first(); }
item last_item()        const { return L.last(); }
item next_item(item it) const { return it ? L.succ(it) : 0; }
node inf(item it)       const { return L[it]; } 

};

/*{\Mimplementation
A node set $S$ for a graph $G$ is implemented by a combination of a 
list  $L$ of nodes and a node array of list\_items 
associating with each node its position in $L$. All operations 
take constant time, except for clear which takes time $O(|S|)$. The space 
requirement is $O(n)$, where $n$ is the number of nodes of $G$.}*/


#if LEDA_ROOT_INCL_ID == 600160
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
