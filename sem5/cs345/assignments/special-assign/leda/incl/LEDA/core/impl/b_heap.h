/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  b_heap.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BHEAP_H
#define LEDA_BHEAP_H

//------------------------------------------------------------------------------
// b_heap: bounded heaps with integer keys in [a..b]
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>
#include <LEDA/core/array.h>

LEDA_BEGIN_NAMESPACE

class __exportC b_heap_node {

friend class __exportC b_heap;
friend void print_b_heap_item(b_heap_node*);

int key;
GenPtr info;
list_item loc;

b_heap_node(int k, GenPtr i ) 
{ 
  key = k; info = i; loc = 0; }

  LEDA_MEMORY(b_heap_node)

};

typedef b_heap_node* b_heap_item;

typedef list<b_heap_item>* b_heap_bucket;


class __exportC b_heap {

    int min;
    int max;
    int low;
    int high;
    int _size;
    
    array<b_heap_bucket>  T;

		
virtual void copy_inf(GenPtr&)   const {}

virtual void clear_inf(GenPtr&)  const {}

virtual void print_inf(GenPtr)   const {}

public:

b_heap(int a, int b);
virtual ~b_heap() { clear(); }
b_heap(const b_heap& h);
b_heap& operator=(const b_heap& h);

void insert(b_heap_item p);

b_heap_item insert(int key, GenPtr info)
{ copy_inf(info);
  b_heap_item it = new  b_heap_node(key,info);
  insert(it);
  return it;
}

b_heap_item find_min();
b_heap_item find_max();
void del_min();
void del_max();
void decrease_key(b_heap_item it, int k);
void increase_key(b_heap_item it, int k);

void delete_item(b_heap_item it);
void clear();

GenPtr inf(b_heap_item it) { return it->info; }
int key(b_heap_item it)  { return it->key; }
int empty()              { return (find_min()==0) ? true : false; }

int size() const {return _size;}

void print();

int lower_bound() const {return low;}
int upper_bound() const {return high;}

};

LEDA_END_NAMESPACE

#endif

