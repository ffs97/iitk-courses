/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_slist.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_SLIST_H
#define LEDA_NODE_SLIST_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600178
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// node_slist
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {node_slist} {} {Singly Linked Lists of Nodes} }*/

template<class graph_t, class SuccArray>
class base_node_slist {

public:
typedef graph_t graph;
typedef typename graph_t::node node;

private:
SuccArray succ_node;
node first_node;
node last_node;

/*{\Mdefinition
An instance of the data type |\Mname| is a singly linked list of nodes. It
is implemented more efficiently than the general list type $list\<node\>$ 
(\ref{Linear Lists}). However, it can only be used with the 
restriction that every node is contained in at most one |\Mname|.  }*/


// private copy constructor and assignment operator
// (node_slist's cannot be copied)

base_node_slist(const base_node_slist&);

base_node_slist& operator=(const base_node_slist&);

public:

typedef node item;

/*{\Mcreation L }*/

 base_node_slist()
 { first_node = last_node = nil; }

 bool use_node_data(const graph& G)
 { first_node = last_node = nil;  
   return succ_node.use_node_data(G,nil); }

/*{\Moptions nextwarning=no}*/

  base_node_slist(const graph& G) : succ_node(G,nil) 
  { first_node = last_node = nil; }
/*{\Mcreate    introduces a variable |\Mvar| of type |\Mname| and initializes
               it with the empty list. }*/


/*{\Moperations 2 5.0 }*/

  bool empty()  { return first_node == nil; }
/*{\Mop   returns true iff |\Mvar| is empty. }*/

  void insert(node v, node w) 
  { if (w == last_node) 
      append(v);
    else
     { succ_node[v] = succ_node[w];
       succ_node[w] = v;
      }
   }
/*{\Mop   inserts $v$ after $w$ into |\Mvar|.\\
          \precond $w \in L$. }*/


  void append(node v) 
  { if (last_node == nil) 
        first_node = last_node = succ_node[v] = v;
    else
     { succ_node[last_node] = succ_node[v] = v;
       last_node = v;
      }
   }
/*{\Mop   appends $v$ to list |\Mvar|. }*/


  void push(node v)  
  { if (first_node == nil)
      first_node = last_node = succ_node[v] = v;
    else
     { succ_node[v] = first_node;
       first_node = v;
      }
   }
/*{\Mop   adds $v$ at the front of |\Mvar|. }*/


  node pop()  
  { node v = first_node;
    first_node = succ_node[v];
    if (first_node == v) first_node = last_node = nil;
    succ_node[v] = nil;
    return v;
   }
/*{\Mop   deletes the first node from |\Mvar| and returns it.\\
          \precond |\Mvar| is not empty. }*/


  GenPtr member(node v) const { return succ_node[v]; }
/*{\Mop    returns true if $v \in L$ and false otherwise. }*/

  GenPtr operator()(node v) const { return member(v); }
/*{\Mfunop    returns true if $v \in L$ and false otherwise. }*/

/*{\Moptions nextwarning=no}*/
  node head() const { return first_node; }
  node first()const { return first_node; }
/*{\Mop    returns the first node in |\Mvar| (nil if |\Mvar| is empty).}*/

/*{\Moptions nextwarning=no}*/
  node tail() const { return last_node; }
  node last() const { return last_node; }
/*{\Mop    returns the last node in |\Mvar| (nil if |\Mvar| is empty).}*/


  node succ(node v) const 
  { node w = succ_node[v]; 
    return (v == w) ? nil : w;
  }
/*{\Mop    returns the successor of $v$ in |\Mvar|.\\ 
           \precond $v \in L$. }*/

  void clear() { while (!empty()) pop(); }
/*{\Mop    makes |\Mvar| the empty list. }*/



// iteration
  node first_item()       const { return first(); }
  node last_item()        const { return last(); }
  node next_item(node x)  const { return x ? succ(x) : 0; }
  node inf(node x)        const { return x; } 

};

/*{\Mtext
\bigskip
{\bf forall}($x,L$)
$\{$ ``the elements of $L$ are successively assigned to $x$'' $\}$ }*/


struct node_slist : public base_node_slist<graph, node_array<node> >
{
  typedef base_node_slist<graph, node_array<node> > base_list;
  typedef base_list::item item;
  node_slist() {}
  node_slist(const graph& G) : base_list(G) {} 
};




#if LEDA_ROOT_INCL_ID == 600178
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
