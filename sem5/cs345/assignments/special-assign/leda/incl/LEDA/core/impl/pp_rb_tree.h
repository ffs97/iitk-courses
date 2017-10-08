/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  pp_rb_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PP_RB_TREE_H
#define LEDA_PP_RB_TREE_H


//------------------------------------------------------------------------------
//
// pp_rb_tree:  red black trees (leaf oriented & doubly linked)
//
//           derived from class "pp_btree"
//
// Sven Thiel (1998); ephemeral version: Stefan Naeher (1993)
//
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/impl/pp_bin_tree.h>

LEDA_BEGIN_NAMESPACE
 
typedef pp_btree_node* pp_rb_tree_item;
 
// ----------------------------------------------------------------
// class pp_rb_tree
// ----------------------------------------------------------------

class __exportC pp_rb_tree : public pp_btree
{ 
  enum Color  { Red = 0, Black = 1 };

  int root_balance() { return Black; }
  int node_balance() { return Red; }
  int leaf_balance() { return Black; }
 
  void insert_rebal(pp_rb_tree_item);
  void del_rebal(pp_rb_tree_item, pp_rb_tree_item);

private:
  pp_rb_tree(const pp_rb_tree& T);
  pp_rb_tree& operator=(const pp_rb_tree& T);

public:

  pp_rb_tree() {}
 ~pp_rb_tree() {}
};

LEDA_END_NAMESPACE

#endif

