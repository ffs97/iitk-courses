/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bb1_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




#ifndef LEDA_BB1_TREE_H
#define LEDA_BB1_TREE_H

//--------------------------------------------------------------------
//  
//  BB[alpha] Trees
//
//  Michael Wenzel   (1989)
//
// Implementation follows
// Kurt Mehlhorn: Data Structures and Algorithms 1, section III.5.1
//
// Aenderungen:
//   - virtuelle compare-Funktion   (M. Wenzel, Nov. 1989)
//--------------------------------------------------------------------


#include <LEDA/core/b_stack.h>

LEDA_BEGIN_NAMESPACE


// -----------------------------------------------------
// declarations and definitions
// -------------------------------------------------------

const int BB1_STACKSIZE = 32 ; // according to tree.size and alpha

const double SQRT1_2 = 0.70710678;

class bb1_node;
class bb1_tree;

typedef bb1_node* bb1_item;

typedef b_stack<bb1_item> bb1_tree_stack;


typedef void (*DRAW_BB_NODE_FCT)(double,double,void*);
typedef void (*DRAW_BB_EDGE_FCT)(double,double,double,double);

class __exportC bb1_node {

  GenPtr key;
  GenPtr inf;
  int gr;
  bb1_item sohn[2];

  friend class __exportC bb1_tree;
  friend class __exportC range_tree;
  friend class __exportC Segment_Tree;
  friend class __exportC seg_node_tree;

public:

  int blatt()         { return (gr==1); }
  int groesse()       { return gr; }

  float bal()
	{ if (blatt()) return 0.5 ;
	  else return float(float(sohn[0]->groesse())/float(gr));
        }

  bb1_node(GenPtr k=0,GenPtr i=0,int leaf=0,bb1_item ls=0,bb1_item rs=0)
    { key = k;
      inf = i;
      sohn[0] = ls;
      sohn[1] = rs;
      if (leaf==0)
	gr=1;
      else gr = ls->groesse()+rs->groesse();
    }

  bb1_node(bb1_item p)
    { 
      key = p->key;
      inf = p->inf;
      gr = p->groesse();
      sohn[0] = p->sohn[0];
      sohn[1] = p->sohn[1];
    }
      
  LEDA_MEMORY(bb1_node)

}; 



class __exportC bb1_tree {

enum children { left = 0 , right = 1 };

enum leaf_or_node { Leaf = 0 , Node = 1 } ;

  bb1_item root;
  bb1_item first;
  mutable bb1_item iterator;
  int   anzahl; 
  float alpha;
  float d;
  mutable bb1_tree_stack st;

  friend class __exportC bb1_node;
  friend class __exportC range_tree;
  friend class __exportC seg_node_tree;
  friend class __exportC Segment_Tree;

  int   blatt(bb1_item it)
	{ return (it) ? it->blatt() : 0; }

  void  lrot(bb1_item , bb1_item ); 
  void  rrot(bb1_item , bb1_item ); 
  void  ldrot(bb1_item , bb1_item ); 
  void  rdrot(bb1_item , bb1_item ); 

  void  deltree(bb1_item);
  bb1_item copytree(bb1_item , bb1_item , bb1_item& );
  bb1_item search(GenPtr) const;
  bb1_item sinsert(GenPtr, GenPtr=0);
  bb1_item sdel(GenPtr );

public:

  typedef bb1_node* item;

  virtual int cmp(GenPtr, GenPtr) const { return 0; }
  virtual void clear_key(GenPtr&) const {}
  virtual void clear_inf(GenPtr&) const {}
  virtual void copy_key(GenPtr&)  const {}
  virtual void copy_inf(GenPtr&)  const {}

  GenPtr        key(bb1_item it)  const   { return it->key; }
  const GenPtr& inf(bb1_item it)  const   { return it->inf; }
  GenPtr     translate(GenPtr y);

  bb1_item insert(GenPtr ,GenPtr );
  bb1_item change_obj(GenPtr ,GenPtr );
  bb1_item change_inf(bb1_item it,GenPtr y) { if (it)  
  	                                 { it->inf = y;
                                           return it; }
                                         else return 0;
                                        }

  bb1_item del(GenPtr);

  void del_item(bb1_item it) { if (it) del(it->key); }
  void del_min() { if (first) del(first->key); } 
  void decrease_key(bb1_item p, GenPtr k) { GenPtr i = p->inf; 
                                            del(p->key);
                                            insert(k,i);
                                           }

  bb1_item locate(GenPtr)  const;
  bb1_item located(GenPtr) const;
  bb1_item lookup(GenPtr)  const;

  bb1_item cyclic_succ(bb1_item it)  const
  	  { return it ? it->sohn[1] : 0 ; }
  bb1_item cyclic_pred(bb1_item it)  const
	  { return it ? it->sohn[0] : 0 ; }
  bb1_item succ(bb1_item it) const
          { return ( it && it->sohn[1] != first ) ? it->sohn[1] : 0 ; }
  bb1_item pred(bb1_item it) const
          { return ( it && it != first ) ? it->sohn[0] : 0 ; }

  bb1_item ord(int k)  const;
  bb1_item min()       const    { return (first) ? first : 0; }
  bb1_item find_min()  const    { return (first) ? first : 0; }
  bb1_item max()       const    { return (first) ? first->sohn[0] : 0 ; }

  bb1_item first_item() const          { return (first) ? first : 0; }
  bb1_item next_item(bb1_item x) const { return x ? succ(x) : 0; }

  bb1_item move_iterator() const;

  int current_item(bb1_item& x) const
     { if (!iterator) return 0;
       else { x = iterator; return 1; }
     }


  void init_iterator() const    { iterator = 0; }
  void lbreak()        const    { iterator = 0; }


  void  clear();
  int   size()    const        { return anzahl; }
  int   empty()   const        { return (anzahl==0) ? true : false; }

  bool  member(GenPtr ) const;
  bb1_tree& operator=(const bb1_tree& w);

  void  set_alpha(float a) 
        { if (anzahl>=3)
             LEDA_EXCEPTION(4,"aenderung von alpha nicht erlaubt");
          if ((a<0.25) || (a>1-SQRT1_2))
             LEDA_EXCEPTION(3,"alpha not in range");
          alpha=a;
          d = 1/(2-alpha) ;
        }

  float get_alpha() { return alpha; }

  bb1_tree() : st(BB1_STACKSIZE)
  { 
    root = first = iterator = 0;
    anzahl = 0;
    alpha=float(0.28);
    d=1/(2-alpha);
  }

  bb1_tree(float);
  bb1_tree(const bb1_tree&);


// if the destructor is declared virtual range/segment trees crash ????

virtual
  ~bb1_tree()  { clear(); }



  void draw(DRAW_BB_NODE_FCT, DRAW_BB_EDGE_FCT, bb1_node*,
            double, double, double, double, double);

  void draw(DRAW_BB_NODE_FCT, DRAW_BB_EDGE_FCT, 
            double, double, double, double);

};


LEDA_END_NAMESPACE

#endif
