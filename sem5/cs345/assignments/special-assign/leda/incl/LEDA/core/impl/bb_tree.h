/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bb_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_BB_TREE_H
#define LEDA_BB_TREE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 350982
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
//
// bb_tree:  
//
//           BB[alpha] trees (derived from class "bin_tree")
//
// Stefan N"aher (1993)
//
//------------------------------------------------------------------------------


#include <LEDA/system/basic.h>
#include <LEDA/core/impl/bin_tree.h>

LEDA_BEGIN_NAMESPACE

 
typedef bin_tree_node* bb_tree_item;

 
// ----------------------------------------------------------------
// class bb_tree
// ----------------------------------------------------------------

// const float alpha = 0.28;
// const float d     = 0.58;

// we multiply alpha, d, and all balances by 64 to make them integers

  const int alpha = 18; // 0.28 * 64
  const int d = 37;     // 0.58 * 64

class __exportC bb_tree : public bin_tree
{ 
  int         tree_type() const { return LEDA_BB_TREE; }
  const char* tree_name() const { return "BB[alpha] Tree"; }

  int root_balance() { return 2; }
  int node_balance() { return 2; }
  int leaf_balance() { return 1; }

  void rebal(bb_tree_item,int);
  void insert_rebal(bb_tree_item);
  void del_rebal(bb_tree_item, bb_tree_item);


  float balance(bb_tree_item p)
  { if (p->is_leaf()) 
       return 0.5 ;
    else 
       return float(p->child[left]->get_bal())/p->get_bal();
   }

public:

  bb_tree() {}
 ~bb_tree() {}

  bb_tree(const bb_tree& T) : bin_tree(T) {}

  bb_tree& operator=(const bb_tree& T) 
  { bin_tree::operator=(T); return *this; }

};

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 350982
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
