/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  iv_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_IV_TREE_H
#define LEDA_IV_TREE_H

//--------------------------------------------------------------------
//  
//  Interval Trees
//
//  Michael Seel (1990/91)
//
// Implementation as described in
// Kurt Mehlhorn: Data Structures and Algorithms 3, section VIII.5.1.1
//
//--------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/b_stack.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

class interval;
typedef interval* interval_item;





// -------------------------------------------------------
// declarations and definitions
// -------------------------------------------------------

const int IV_STACKSIZE = 70 ; // according to tree.size and alpha
 		        // here: tree.size <= 10^9
 		        //       alpha = 0.25 ( worst case )

enum leaf_or_node { is_leaf = 0 , is_node = 1 } ;

class iv_node;
class iv_tree;

typedef int       x_typ;
typedef iv_node*  iv_item;




// -----------------------------------------------------------
// zur Unterscheidung bei Eingabe von Intervallen gleicher
// x-Koordinate wird als Split-Value nicht nur die linke
// Intervall-Grenze, sondern auch eine durchlaufende Nummer
// als zweite Komponente abgespeichert, ueber der auch eine
// compare Funktion deklariert wird.
// -----------------------------------------------------------


struct split_pair
{     x_typ key1;
      int   key2;
      split_pair(x_typ x=0,int n=0) { key1=x;key2=n; }
      split_pair(split_pair& s) {key1=s.key1;key2=s.key2;}
      int  cmp(x_typ x1,x_typ x2) {return int(x1)-int(x2);}
      void print() {cout << "(" << key1 << "/" << key2 << ")"; }
    };

typedef split_pair* split_item;

// -----------------------------------------------------------
// zur Abspeicherung der x- und y-orientierten Knotenlisten !!
// Die Dictionary-Deklaration wird nach erstellen der geeig-
// neten bb_trees durch diese ersetzt. Bis dahin kostet bei
// den rb_trees die Vereinigung zweier geordneter Listen 
// (Laenge beider Listen insgesamt n) die Zeit O(n log n)
// anstatt wie angestrebt O(n) !!!
// -----------------------------------------------------------

// -------------------------------------------------------
// interval, interval_item und query-Rueckgabetyp
// -------------------------------------------------------

class interval { public:
                  x_typ koo1;
		  x_typ koo2;
		  interval(x_typ x,x_typ y) {koo1=x;koo2=y;}
                  int cmp(x_typ x1,x_typ x2) {return int(x1)-int(x2);}
                  void print() {cout << "[" << koo1 << "/" << koo2 << "]"; }
                };


COMPARE_DECL_PREFIX
inline int DEFAULT_COMPARE (const interval_item& p, const interval_item& q)
{ int a = p->cmp(p->koo1,q->koo1);
  return (a) ? a : p->cmp(p->koo2,q->koo2);
 }

LEDA_END_NAMESPACE

#include <LEDA/core/dictionary.h>

LEDA_BEGIN_NAMESPACE

typedef list<interval_item> interval_list;
typedef dictionary<interval_item,int> nodelist;
typedef nodelist* nodelist_p;

// -------------------------------------------------------
// class iv_node     
// -------------------------------------------------------

class __exportC iv_node {

  nodelist_p    x_nl;
  nodelist_p    y_nl;
  split_item    split_val;   // nach split_val (= Pointer auf das
			     // Paar (linke Intervallgrenze;-lfnr)
			     // wird geordnet
  int gr;
  iv_item son[2];

  friend class __exportC iv_tree;

public:

  LEDA_MEMORY(iv_node)

  nodelist_p    x_nodelist()           { return x_nl; }
  nodelist_p    y_nodelist()           { return y_nl; }
  split_item    split_value()          { return split_val; }

  int         blatt()                { return (gr==1); }
  int         groesse()              { return gr; }

  float bal()
	{ if (blatt()) return 0.5;
	  else return float(float(son[0]->groesse())/float(gr));
        }

  iv_node(split_item s_i, leaf_or_node ln=is_leaf, iv_item ls=0, iv_item rs=0)
  { 
    x_nl = new nodelist;
    y_nl = new nodelist;
    split_val = new split_pair(*s_i);
    son[0] = ls;
    son[1] = rs;
    if (ln==is_leaf)
	gr=1;
    else 
	gr = ls->groesse()+rs->groesse();
   }


 ~iv_node()
  { delete x_nl;
    delete y_nl;
    delete split_val;
  }


}; 


// -------------------------------------------------------
// class iv_tree     
// -------------------------------------------------------

class iv_tree {

  iv_item root;
  int   anzahl; 
  float alpha;
  float d;
  b_stack<iv_item> st;
  int   interval_nb;         //  jedes Intervall, das eingefuegt wird,
			     //  erhaelt eine laufende Nummer.
			     //  in dieser Implementation wird diese
			     //  mit dem Konstruktor auf 0 gesetzt
			     //  und bei jedem Insert inkrementiert
			     //  bei Dauerbenutzung einer Datenstruktur
			     //  droht damit irgendwann ein Overflow.

  friend class __exportC iv_node;

  int cmp (const split_item& p, const split_item& q)
    {
       int a = p->cmp(p->key1,q->key1);
       return (a) ? a : p->key2 -  q->key2;
    }

  
  void    lrot(iv_item , iv_item ); 
  void    rrot(iv_item , iv_item ); 
  void    ldrot(iv_item , iv_item ); 
  void    rdrot(iv_item , iv_item ); 
  void    reorganize_nodelist(iv_item , iv_item);
  iv_item search(split_item);
  iv_item ins(interval_item,interval_item,int);
  iv_item sink(iv_item, interval_item, interval_item, int);
  int     del(split_item);
  void    deltree(iv_item);

  split_item    split_value(iv_item it)  {return (it) ? it->split_value() : 0;}

  // split_in_interval ueberprueft ob die erste Komponente des split_value
  // des Knotens v im Interval i liegt

  int   split_in_x_interval(iv_item v, interval_item i)
	{ if ((i->cmp(i->koo1 , split_value(v)->key1) <= 0)
	  && (i->cmp(split_value(v)->key1 , i->koo2) <= 0))
	    return 1;
	  else
	    return 0;
        }

  int   split_in_y_interval(iv_item v, interval_item i)
	{ if ((i->cmp(i->koo2 , split_value(v)->key1) <= 0)
	  && (i->cmp(split_value(v)->key1 , i->koo1) <= 0))
	    return 1;
	  else
	    return 0;
        }

  // nodelist_swap vertauscht die Knotenlisten zweier Knoten

  void    nodelist_swap(iv_item p, iv_item q)
	  { 
	    nodelist_p help = p->x_nl;
	    p->x_nl = q->x_nl;
	    q->x_nl = help;
	    help = p->y_nl;
	    p->y_nl = q->y_nl;
	    q->y_nl = help;
          }

  void    y_search(interval_list& il, iv_item v, split_item ys, x_typ x, x_typ y);
  void    check_nodelist(interval_list& il, iv_item v, x_typ x, x_typ y);
  void    get_all_in_tree(interval_list& il, iv_item v);
  void    take_all_iv(interval_list& il, iv_item v);
  void    check_y_iv(interval_list& il, iv_item v, x_typ x);
  void    check_x_iv(interval_list& il, iv_item v, x_typ y);


  void pr_iv_tree(iv_item, int);
  void pr_iv_list(iv_item);

public:

  nodelist_p    x_nodelist(iv_item it)   {return (it) ? it->x_nodelist() : 0;}
  nodelist_p    y_nodelist(iv_item it)   {return (it) ? it->y_nodelist() : 0;}
 
 // Operationen auf Intervall-B"aumen:

  iv_item         iv_insert(x_typ, x_typ);
  void            iv_delete(x_typ, x_typ);
  interval_list   iv_query(x_typ, x_typ);

  // zur Datendarstellung folgende Ausgabe-Funktionen:

  void          print_split(iv_item);
  void          text(string s) { cout << s; }
  void          pr_list(interval_list& il);
     
  void pr_iv_tree() { pr_iv_tree(root,0); }


  iv_tree()   :  st(IV_STACKSIZE) 
  { 
    root = 0;
    anzahl = 0;
    alpha=float(0.28);
    d= float(1/(2-alpha));
    interval_nb = 0;
  }


  ~iv_tree()  
  { 
   if (root)
     deltree(root);
   root = 0;
   anzahl = 0;
   alpha = 0;
   d = 0;
   interval_nb = 0;
  }

};


LEDA_END_NAMESPACE

#endif
