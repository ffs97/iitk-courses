/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  k_heap.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_KHEAP_H
#define LEDA_KHEAP_H

//------------------------------------------------------------------------------
// k-nary Heaps
//------------------------------------------------------------------------------


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

class __exportC k_heap_elem
{
  friend class __exportC k_heap;

  GenPtr key;
  GenPtr inf;

  int index;

  k_heap_elem(GenPtr k, GenPtr i, int pos) { key = k; inf = i; index = pos; }

  LEDA_MEMORY(k_heap_elem)

};


typedef k_heap_elem* k_heap_item;


class __exportC k_heap  {

virtual int cmp(GenPtr, GenPtr) const { return 0; }
virtual void copy_key(GenPtr&)  const {}
virtual void copy_inf(GenPtr&)  const {}
virtual void clear_key(GenPtr&) const {}
virtual void clear_inf(GenPtr&) const {}
virtual void print_key(GenPtr)  const {}
virtual void print_inf(GenPtr)  const {}

virtual int  key_type_id() const { return UNKNOWN_TYPE_ID; }


int          K;
int          count;
int          max_size;
k_heap_item* HEAP;

void rise(int,k_heap_item);
void sink(int,k_heap_item);

void check(k_heap_item);

public:

typedef k_heap_item item;

k_heap_item insert(GenPtr, GenPtr);
k_heap_item find_min()  const      { return HEAP[1];  }
k_heap_item first_item() const     { return HEAP[1]; }
k_heap_item next_item(k_heap_item it) const 
{ return it ? HEAP[it->index+1] : 0;}

void decrease_key(k_heap_item, GenPtr);
void change_inf(k_heap_item, GenPtr);
void del_item(k_heap_item);
void del_min() { del_item(find_min()); }

const GenPtr& key(k_heap_item it) const { return it->key; }
const GenPtr& inf(k_heap_item it) const { return it->inf; }

int  size()    const  { return count;    }
bool empty()   const  { return count==0; }

void clear();
void print();

k_heap& operator=(const k_heap&);

k_heap(int n = 0,int k = 2);  // default: binary heap
k_heap(const k_heap&);

virtual ~k_heap();

};

LEDA_END_NAMESPACE

#endif
