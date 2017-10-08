/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  pq_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PQ_TREE_H
#define LEDA_PQ_TREE_H

#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

class __exportC  pq_node_struct;

typedef pq_node_struct* pq_node;   


class __exportC  pq_node_struct
{
  friend class __exportC pq_tree;

  int           type, mark, status, parent_type;

  int           leaf_index;
  int           child_count;
  int           pert_child_count;
  int           pert_leaf_count;
  int           full_child_count;

  pq_node      parent;
  pq_node      link_one_side, link_other_side;
  pq_node      left_most, right_most;
  pq_node      part_child1, part_child2;


public:

        pq_node_struct(); 
  void  node_reset();
        LEDA_MEMORY(pq_node_struct);

};




class __exportC pq_tree 
{

list<pq_node>  processed;
pq_node        *leaves;
int             leaves_size;
int             blocked_chain_count;
int             blocked_nodes_count;
int             root_reached;           // => bool ***
bool            too_many_part;
  
        void    leaves_double();
        void    leaves_init(int lsize);
        void    del_pert_subtree();
        bool    bubble(list<int>& S);
        void    bubble_reset(pq_node);
        bool    reduce(list<int>& S);
        void    del_subtree(pq_node);
        void    sequence(list<int>&, pq_node, pq_node l = NULL);

        void    go_to_sibling(pq_node& k, pq_node& last_k);
        void    skip_dir(pq_node&, pq_node&);
        void    fix_part_child_direction(pq_node x, bool part_child2_too=false);
        void    append_as_right_child(pq_node parent, pq_node x);
        void    append_as_left_child(pq_node parent, pq_node child);
/*
inline  void    go_to_sibling(pq_node& k, pq_node& last_k);
inline  void    skip_dir(pq_node&, pq_node&);
inline  void    fix_part_child_direction(pq_node x, bool part_child2_too=false);
inline  void    append_as_right_child(pq_node parent, pq_node x);
inline  void    append_as_left_child(pq_node parent, pq_node child);
*/

        pq_node  hang_down_full_children(pq_node x, pq_node z);
        pq_node  remove_from_siblings(pq_node parent, pq_node child);
        void    replace_in_siblings(pq_node x, pq_node z);
        bool    set_as_part_child(pq_node x);
        void    delete_part_node_parent(pq_node x);
        bool    template_PQL1(pq_node x, bool is_pseudo_root);
        bool    template_P3(pq_node x, bool is_pseudo_root);
        bool    template_P5(pq_node x, bool is_pseudo_root);
        bool    template_P6(pq_node x);
        bool    template_Q2(pq_node x);
        bool    template_Q3(pq_node x);
  
protected:

  pq_node  root; 
  pq_node  pseudo_root;
  bool      successful;
  

public:

        pq_tree(int lsize=0);
        pq_tree(list<int> &S);
        ~pq_tree();
  
        bool reduction(list<int> &S);
        bool update(list<int> &S);
        void frontier(list<int> &F) { F.clear(); sequence(F,root); }
        void pert_sequence(list<int> &S);

        bool replace(list<int> & D, list<int>& U, list<int>& I);
};


LEDA_END_NAMESPACE

#endif





