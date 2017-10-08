/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  f_heap.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_FHEAP_H
#define LEDA_FHEAP_H

//------------------------------------------------------------------------------
//
// Fibonacci Heap
//
// Michael Muth  (1988)
//
// Modifications
//
// - virtual compare function  (Stefan Naeher, August 1989)
//
// - virtual clear functions   (Stefan Naeher, April 1990)
//
// - improvement for double keys (key_type_id)  (Stefan Naeher, 1996)
//
//------------------------------------------------------------------------------


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE


class __exportC f_heap_node  {
// repraesentiert Knoten und Heap geordnete Baeume

friend class __exportC f_heap;

   f_heap_node* next;          // used to link all used items 
   f_heap_node* pred;          

   f_heap_node* left;          // left and right siblings (circular List)
   f_heap_node* right;   
   f_heap_node* parent;        // parent node
   f_heap_node* children;      // a child

   int  rank;                   // number of children
   char mark;                   // ( 1=true, 0=false )

   GenPtr key;                  // key
   GenPtr inf;                  // information


// size: 10 words =  40 bytes


   void link(f_heap_node*);
   void cut(f_heap_node*);


public:

   f_heap_node(GenPtr k, GenPtr i, f_heap_node* n) 
   {  key = k;
      inf = i;
      next = n;
      pred = 0;
      rank = 0;
      mark = 0;
      parent = 0;
      children = 0;

      if (n) n->pred = this;

    }  // end of f_heap_node()


  LEDA_MEMORY(f_heap_node)

 };  //f_heap_node


typedef f_heap_node* f_heap_item;


class __exportC f_heap
 /* Ein F-Heap enthaelt eine zirkulaere Liste von Heap geordneten
    Baeumen. Der Einstieg erfolgt ueber den minptr, einen Zeiger
    auf den Baum mit kleinstem Schluessel in der Wurzel. Die ei-
    gentlichen Items ( Paare aus NxT ) sind in den Knoten der
    Baeume enthalten.                                               */
 { 
   int node_count;         // number of nodes
   f_heap_node* minptr;    // entry to the List of roots
   f_heap_node* node_list; // List of all nodes

   int max_rank() const;

   f_heap_node* merge_heaps(f_heap_node**);
   f_heap_node* gen_merge_heaps(f_heap_node**);
   f_heap_node* int_merge_heaps(f_heap_node**);

   f_heap_node* new_f_heap_node(GenPtr k, GenPtr i)
   { return  node_list = new f_heap_node(k,i,node_list); }

   virtual int cmp(GenPtr, GenPtr) const { return 0; }

   virtual void print_key(GenPtr)  const {}
   virtual void print_inf(GenPtr)  const {}

   virtual void clear_key(GenPtr&) const {}
   virtual void clear_inf(GenPtr&) const {}

   virtual void copy_key(GenPtr&)  const {}
   virtual void copy_inf(GenPtr&)  const {}

   virtual int  key_type_id() const { return UNKNOWN_TYPE_ID; }


public:

 typedef f_heap_node* item;

 f_heap(int=0); // parameter not used
 f_heap(const f_heap&);
 f_heap& operator=(const f_heap&);

virtual ~f_heap()  { clear(); }


f_heap_node* insert(GenPtr, GenPtr);
f_heap_node* find_min() const { return(minptr); }

void del_min();  
void decrease_key(f_heap_node*,GenPtr);
void change_inf(f_heap_node*,GenPtr);
void del_item(f_heap_node *x){ decrease_key(x,minptr->key); del_min();}
void clear();

const GenPtr& key(f_heap_node *x) const { return x->key ; }
GenPtr&       inf(f_heap_node *x) const { return x->inf; }

  
int  size()  const { return node_count; }
int  empty() const { return (find_min()==0) ? true : false; }


// Iteration:


f_heap_node* first_item() const;
f_heap_node* next_item(f_heap_node* p) const;

f_heap_node* last_item() const { return 0; }

f_heap_node* stl_pred_item(f_heap_node*) const 
{  LEDA_EXCEPTION(1,"f_heap::stl_pred_item not implemented."); return 0; }

f_heap_node* stl_next_item(f_heap_node* p) const 
{  return p ? p->next : node_list; }


};  // end of class f_heap


LEDA_END_NAMESPACE

#endif
