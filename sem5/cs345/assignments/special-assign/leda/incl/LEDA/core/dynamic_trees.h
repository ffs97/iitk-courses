/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dynamic_trees.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DYNAMIC_TREES_H
#define LEDA_DYNAMIC_TREES_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600028
#include <LEDA/internal/PREAMBLE.h>
#endif

// -------------------------------------------------------------------- //
// Declaration of class dynamic_trees                                   //
//                                                                      //
// This is the interface of an implementation of the dynamic trees data //
// structure by Sleator and Tarjan.                                     //
//                                                                      //
// by David Alberts (1996)                                              //
// -------------------------------------------------------------------- //

// RCS ID //
/* $Id: dynamic_trees.h,v 1.2 2005/11/10 14:55:38 algosol Exp $ */

#include<LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

class __exportC vertex_struct;           // forward reference, see below
typedef vertex_struct* vertex;
typedef vertex_struct* path;   // no global information at a path

/*{\Manpage {dynamic_trees} {} {Dynamic Trees} }*/

class __exportC dynamic_trees            // a set of trees with edge weights
{

  /*{\Mdefinition
  An instance $D$ of the data type |\Mname| is a set of dynamically changing
  rooted trees. Each edge is directed towards the root and has a weight.
  Optionally, user defined information can be stored at the vertices
  and at the edges.

  \textsl{Remark} Dynamic trees are very similar to the data type
  |tree_collection|. The main difference is that the former use
  edge weights instead of the node weights used by the latter.}*/

  public:

  /*{\Mcreation D }*/

    dynamic_trees() { }
    /*{\Mcreate creates an empty instance |\Mvar| of type |\Mname.|}*/

    ~dynamic_trees() { clear(); }

    /*{\Moperations 1.3 5 }*/
    
    vertex make(void* x=nil);
    /*{\Mop creates a tree containing a single node $v$ which is returned.
      The additional user defined information $x$ is stored at $v$.}*/
    
    void clear();
    
    void* vertex_inf(vertex v);
    /*{\Mop returns the additional user defined information at $v$.}*/
    
    void* edge_inf(vertex v);
    /*{\Mop returns the additional user defined information at
      ($v$,parent($v$)) or $nil$, if $v$ has no parent.}*/
    
    vertex parent(vertex v);
    /*{\Mop returns the parent of $v$ in the tree or $nil$.}*/
    
    vertex root(vertex v);
    /*{\Mop returns the root of the tree containing $v$.}*/
    
    double cost(vertex v);
    /*{\Mop returns the cost of ($v$,parent($v$)).\\
      \precond $v$ is not a tree root.}*/
    
    vertex mincost(vertex v);
    /*{\Mop returns vertex $w$ closest to root($v$) s.t. ($w$,parent($w$))
      has minimal cost on the path $v$ $\rightarrow$ root($v$).\\
      \precond $v$ is not a tree root.}*/
    
    void update(vertex v, double x);
    /*{\Mop adds $x$ to each edge on the path $v$ $\rightarrow$ root($v$).}*/
    
    void link(vertex v, vertex w, double x, void* e_inf=nil);
    /*{\Mop links the tree root $v$ (prec.) to the vertex $w$ in a different
      tree (prec.). The edge $e=(v,w)$ gets weight $x$, and the
      additional user defined information $e_inf$ is stored at $e$.}*/
    
    double cut(vertex v);
    /*{\Mop deletes the edge ($v$,parent($v$)) and returns its weight.\\
      \precond $v$ is not a tree root.}*/
    
    void evert(vertex v);
    /*{\Mop makes $v$ the new root of its tree.}*/
    
    vertex lca(vertex v, vertex w);
    /*{\Mop returns the lowest common ancestor of $v$ and $w$ or $nil$.\\
      \precond $v$ and $w$ are not $nil$.}*/
    

  protected:
    path splice(path p);
      // Extend the solid path p at its tail by one edge. Return
      // the extended path. Prec.: tail(p) is not a tree root.
    
    path expose(vertex v);
      // Convert the path from v to root(v) to a single solid path
      // and return it.
    

    list<vertex> vertices;     // current vertices (for destruction)

  friend class __exportC vertex_struct;
};

/*{\Mimplementation
Dynamic Trees are implemented using binary trees with the randomized
balancing scheme by Aragon and Seidel. Each operation takes $O(\log^2 n)$
amortized expected time except for make which takes constant time. $n$
is the current number of nodes.}*/

class __exportC vertex_struct            // one vertex on a dynamic path
{
  public:

    vertex_struct(bool leaf=false, void* x=nil);
      // Ceate a new vertex_struct representing an edge or a leaf
      // (depending on leaf). x is a pointer to optional user data.

    virtual ~vertex_struct() {}
    
    friend path path_of(vertex v);
      // Return the path containing v.
    
    friend vertex head(path p);
      // Return the first vertex of p.
    
    friend vertex tail(path p);
      // Return the last vertex of p.
    
    friend vertex bef(vertex v);
      // Return the vertex before v on path_of(v) or nil.
    
    friend vertex aft(vertex v);
      // Return the vertex after v on path_of(v) or nil.
    
    friend double pcost(vertex v);
      // Return the cost of the edge (v,aft(v)).
      // Prec.: v != tail(path_of(v))
    
    friend vertex pmincost(path p);
      // Return the vertex v closest to tail(p) such that the cost
      // of (v,aft(v)) is minimal on p.
      // Prec.: p contains more than one vertex.
    
    friend void pupdate(path p, double x);
      // Add x to the cost of every edge on p.
    
    friend void reverse(path p);
      // Reverse the direction of p.
      // Prec.: p represents an entire path (p->par == nil).
    
    friend path concatenate(path p, path q, double x, void* e_inf=nil);
      // Combine p and q by adding the edge (tail(p),head(q)) of cost x
      // with additional information e_inf. Return the combined path.
    
    friend void split(vertex v, path& p, path& q, double& x, double&y);
      // Split path_of(v) by deleting the edges incident to v.
      // Assign the path head(path_of(v)) -> bef(v) to p.
      // Assign the path aft(v) -> tail(path_of(v)) tp q.
      // Assign the cost of the deleted edge (bef(v),v) to x.
      // Assign the cost of the deleted edge (v,aft(v)) to y.
      // p or q might be nil, then x or y are undefined.
    
    void* vertex_inf();
      // Return the additional user defined information at this node.
    

  protected:

      friend void rotate(vertex rot_child, vertex rot_parent);
        // Rotate such that rot_child becomes the parent of rot_parent.
        // Prec.: rot_child is a child of rot_parent, both are internal.
    
      virtual void isolate();
        // Make this node an isolated node before deletion.
        // Prec.: this is a tree root and it is internal.
    

    long prio;              // priority for balancing
    
    vertex par;             // parent node
    vertex child[2];        // children
    vertex handt[2];        // head and tail of the subpath of this node
    
    int  reversed;          // reversal state of subpath of this node
    bool external;          // external <=> this node is a leaf in the tree
    
    double netcost;         // implicit representation of the cost of
    double netmin;          // the edge of this node and the minimal edge
                            // cost on the subpath of this node (see docs)
                            // valid only for internal nodes
    
    vertex dparent;         // (this node, dparent) is a dashed edge,
                            // if dparent != nil
    double dcost;           // cost of the above edge
    
    void* x;                // additional user defined vertex information
    void* y;                // additional user defined edge information
    

  friend class __exportC dynamic_trees;
};


#if LEDA_ROOT_INCL_ID == 600028
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
