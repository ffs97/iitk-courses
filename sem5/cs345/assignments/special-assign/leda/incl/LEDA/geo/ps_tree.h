/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ps_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PSTREE_H
#define LEDA_PSTREE_H

//--------------------------------------------------------------------
//  
//  Priority Search Trees
//
//  Renate Lassen (1990)
//
// Implementation as described in
// Kurt Mehlhorn: Data Structures and Algorithms 2, section III.5.1.2
//
//--------------------------------------------------------------------

// -----------------------------------------------------
// includes
// -----------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/b_stack.h>

LEDA_BEGIN_NAMESPACE


// -----------------------------------------------------
// declarations and definitions
// -------------------------------------------------------

const int PS_STACKSIZE = 70 ; // according to tree.size and alpha
 		            // here: tree.size <= 10^9
 		            //       alpha = 0.25 ( worst case )

enum ps_leaf_or_node { is_ps_leaf = 0 , is_ps_node = 1 } ;

class ps_node;
class ps_tree;

typedef int x_typ;

struct ps_pair { x_typ x_pkt;
              x_typ y_pkt;
              bool valid;
            };

typedef ps_pair* pair_item;


// -------------------------------------------------------
// class ps_node     
// -------------------------------------------------------

class __exportC ps_node {

  x_typ x_val;
  x_typ y_val;
  x_typ split_val[2];
  int gr;
  ps_node* sohn[2];

  friend class __exportC ps_tree;

public:

  x_typ x_value()           { return x_val; }
  x_typ y_value()           { return y_val; }
  x_typ split_value_x()     { return split_val[0]; }
  x_typ split_value_y()     { return split_val[1]; }

  int blatt()             { return (gr==1); }
  int groesse()           { return gr; }


  float bal()
  { if (blatt()) return 0.5;
    else return float(float(sohn[0]->groesse())/float(gr));
   }


  ps_node(x_typ x_v=0,x_typ y_v=0,x_typ split_v_0=0,x_typ split_v_1=0,ps_leaf_or_node ln=is_ps_leaf,ps_node* ls=0,ps_node* rs=0)
    { x_val = x_v;
      y_val = y_v;
      split_val[0] = split_v_0;
      split_val[1] = split_v_1;
      sohn[0] = ls;
      sohn[1] = rs;
      if (ln==is_ps_leaf)
	gr=1;
      else gr = ls->groesse()+rs->groesse();
    }

  ps_node(ps_node* p)
    { x_val = p->x_value();
      y_val = p->y_value();
      split_val[0] = p->split_value_x();
      split_val[1] = p->split_value_y();
      gr = p->groesse();
      sohn[0] = p->sohn[0];
      sohn[1] = p->sohn[1];
    }

  LEDA_MEMORY(ps_node)
      
}; 


// -------------------------------------------------------
// class ps_tree     
// -------------------------------------------------------

class __exportC ps_tree {

  ps_node* root;
  int   anzahl; 
  float alpha;
  float d;
  b_stack<ps_node*> st;

  friend class __exportC ps_node;

  void  lrot(ps_node* , ps_node* ); 
  void  rrot(ps_node* , ps_node* ); 
  void  ldrot(ps_node* , ps_node* ); 
  void  rdrot(ps_node* , ps_node* ); 

  ps_node* sink(ps_node*, x_typ , x_typ);
  void fill(ps_node*);
  void delleaf(ps_node*);
  void deltree(ps_node*);
   
  ps_node* search(x_typ, x_typ);
  ps_node* locate(x_typ, x_typ);

  void enumerate(x_typ, x_typ, x_typ, ps_node*);
  void pr_ps_tree(ps_node*, int);
  pair_item min_x_in_rect(x_typ ,x_typ ,x_typ ,ps_node*);
  pair_item max_x_in_rect(x_typ ,x_typ ,x_typ ,ps_node*);
  pair_item min_y_in_xrange(x_typ ,x_typ ,ps_node*);

public:

  virtual int cmp(x_typ x,x_typ y) { return int(x)-int(y); }
  virtual int cmp(x_typ x1,x_typ y1,x_typ x2,x_typ y2) 
                                   { if (int(x1)==int(x2))
                                        return int(y1)-int(y2); 
                                     else return int(x1)-int(x2); }

  x_typ   x_value(ps_node* it)      { return (it) ? it->x_value() : 0 ; }
  x_typ   y_value(ps_node* it)      { return (it) ? it->y_value() : 0 ; }
  x_typ   split_value_x(ps_node* it)  { return (it) ? it->split_value_x() : 0 ; }
  x_typ   split_value_y(ps_node* it)  { return (it) ? it->split_value_y() : 0 ; }
 
  ps_node* insert(x_typ ,x_typ );
  ps_node* del(x_typ ,x_typ );
     
  pair_item min_x_in_rect(x_typ x1,x_typ x2,x_typ y0) 
                         { return min_x_in_rect(x1,x2,y0,root); }
  pair_item max_x_in_rect(x_typ x1,x_typ x2,x_typ y0)
                         { return max_x_in_rect(x1,x2,y0,root); }
  pair_item min_y_in_xrange(x_typ x1,x_typ x2)
                           { return min_y_in_xrange(x1,x2,root); }

  void enumerate(x_typ x1,x_typ x2,x_typ y0) { enumerate(x1,x2,y0,root); }

  void pr_ps_tree() { pr_ps_tree(root,0); }

  ps_tree()   :  st(PS_STACKSIZE) 
  { root = 0;
    anzahl = 0;
    alpha=float(0.28);
    d=1/(2-alpha);
   }

  virtual ~ps_tree()  
  { if (root)
    { deltree(root);
      delete(root);
     } 
    root = 0; 
    anzahl = 0;
    alpha = 0;
    d = 0;
   }

};


//------------------------------------------------------------------

LEDA_END_NAMESPACE

#endif
