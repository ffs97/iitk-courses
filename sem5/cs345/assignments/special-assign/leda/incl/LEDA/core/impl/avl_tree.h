/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  avl_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_AVL_TREE1_H
#define LEDA_AVL_TREE1_H

//------------------------------------------------------------------------------
//
// avl_tree:
//
//           AVL trees (derived from bin_tree)
//
// Stefan N"aher (1993)
//
//------------------------------------------------------------------------------



#include <LEDA/system/basic.h>
#include <LEDA/core/impl/bin_tree.h>

LEDA_BEGIN_NAMESPACE
 
typedef bin_tree_node* avl_tree_item;

 
class __exportC avl_tree : public bin_tree
{ 
  // balance of node v with right (left) subtree R (L)
  // bal(v) =  height(R) - height(L) in [-1 ... +1]
  //
  // all created nodes have balance 0

  int         tree_type() const { return LEDA_AVL_TREE; }
  const char* tree_name() const { return "AVL Tree"; }

  int root_balance() { return 0; }
  int leaf_balance() { return 0; }
  int node_balance() { return 0; }

  void insert_rebal(avl_tree_item);
  void del_rebal(avl_tree_item, avl_tree_item);


public:

  avl_tree() {}
 ~avl_tree() {}

  avl_tree(const avl_tree& T) : bin_tree(T) {}

  avl_tree& operator=(const avl_tree& T) 
  { bin_tree::operator=(T); return *this; }

};

LEDA_END_NAMESPACE

#endif


