/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_list.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_LIST_H
#define LEDA_NODE_LIST_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600192
#include <LEDA/internal/PREAMBLE.h>
#endif

#if GRAPH_REP == 2

#include <LEDA/graph/graph.h>
#include <LEDA/core/impl/map_list.h>

LEDA_BEGIN_NAMESPACE

typedef map_list<node,map<node,node> >  node_list;

LEDA_END_NAMESPACE

#else

//------------------------------------------------------------------------------
// node_list
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {node_list} {} {Lists of Nodes} }*/


class __exportC node_list : public c_obj_list {

/*{\Mdefinition
An instance of the data type |\Mname| is a doubly linked list of nodes. It
is implemented more efficiently than the general list type $list\<node\>$ 
(\ref{Linear Lists}). However, it can only be used with the 
restriction that every node is contained in at most one |\Mname|. Also
many operations supported by $list\<node\>$ (for instance |size|) are not supported
by |\Mname|.}*/


// private copy constructor and assingment operator
// (node_list's cannot be copied)

node_list(const node_list&);

node_list& operator=(const node_list&);

public:

  typedef node item;

/*{\Mcreation L }*/

  node_list() {};
/*{\Mcreate    introduces a variable |\Mvar| of type |\Mname| and initializes
               it with the empty list. }*/


/*{\Moperations 2 5.0 }*/

  static void del_node(node v) { v->del_item(); }

  void append(node v) { c_obj_list::append(v); }
/*{\Mop   appends $v$ to list |\Mvar|. }*/

  void push(node v)   { c_obj_list::push(v); }
/*{\Mop   adds $v$ at the front of |\Mvar|. }*/

  void insert(node v, node w) { c_obj_list::insert(v,w); }
/*{\Mop   inserts $v$ after $w$ into |\Mvar|.\\
          \precond $w \in L$. }*/


  node pop()  { return node(c_obj_list::pop()); }
/*{\Mop   deletes the first node from |\Mvar| and returns it.\\
          \precond |\Mvar| is not empty. }*/

  node pop_back()  { return node(c_obj_list::pop_back()); }
/*{\Mop   deletes the last node from |\Mvar| and returns it.\\
          \precond |\Mvar| is not empty. }*/

  void del(node v, node w) { c_obj_list::del(v,w); }

  void del(node v) { c_obj_list::del(v); }
/*{\Mop   deletes $v$ from |\Mvar|.\\
          \precond $v \in L$. }*/

  bool member(node v) const { return v->succ_item() != nil; }
/*{\Mop    returns true if $v \in L$ and false otherwise. }*/

  bool operator()(node v) const { return member(v); }
/*{\Mfunop    returns true if $v \in L$ and false otherwise. }*/

/*{\Moptions nextwarning=no}*/
  node head() const { return node(c_obj_list::first()); }
  node first()const { return node(c_obj_list::first()); }
/*{\Mop    returns the first node in |\Mvar| (nil if |\Mvar| is empty).}*/

/*{\Moptions nextwarning=no}*/
  node tail() const { return node(c_obj_list::last());  }
  node last() const { return node(c_obj_list::last());  }
/*{\Mop    returns the last node in |\Mvar| (nil if |\Mvar| is empty).}*/


  node succ(node v) const { return node(c_obj_list::succ(v)); }
/*{\Mop    returns the successor of $v$ in |\Mvar|.\\
           \precond $v \in L$. }*/

  node pred(node v) const { return node(c_obj_list::pred(v)); }
/*{\Mop    returns the predecessor of $v$ in |\Mvar|.\\
           \precond $v \in L$. }*/

  node cyclic_succ(node v) const { return node(c_obj_list::cyclic_succ(v)); }
/*{\Mop    returns the cyclic successor of $v$ in |\Mvar|.\\
           \precond $v \in L$. }*/

  node cyclic_pred(node v) const { return node(c_obj_list::cyclic_pred(v)); }
/*{\Mop    returns the cyclic predecessor of $v$ in |\Mvar|.\\
           \precond $v \in L$. }*/

  bool empty() const { return c_obj_list::empty(); }
/*{\Mop    returns |true| if |\Mvar| is empty and |false| otherwise. }*/

  void clear() { c_obj_list::clear(); }
/*{\Mop    makes |\Mvar| the empty list. }*/


// iteration

  node first_item()       const { return first(); }
  node last_item()        const { return last(); }
  node next_item(node x)  const { return x ? succ(x) : 0; }
  node pred_item(node x)  const { return x ? pred(x) : 0; }
  node inf(node x)        const { return x; } 

};

/*{\Mtext
\bigskip
{\bf forall}($x,L$)
$\{$ ``the elements of $L$ are successively assigned to $x$'' $\}$ }*/


LEDA_END_NAMESPACE

#endif


#if LEDA_ROOT_INCL_ID == 600192
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
