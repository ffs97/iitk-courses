/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bin_heap.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BIN_HEAP_H
#define LEDA_BIN_HEAP_H

//------------------------------------------------------------------------------
// binary Heaps
//
// S. Naeher (1993)
//
//------------------------------------------------------------------------------


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE


class __exportC bin_heap_elem
{
  friend class __exportC bin_heap;

  GenPtr key;
  GenPtr inf;

  int index;

  bin_heap_elem(GenPtr k, GenPtr i, int pos) { key = k; inf = i; index = pos; }

  LEDA_MEMORY(bin_heap_elem)

};


typedef bin_heap_elem* bin_heap_item;


class __exportC bin_heap  {

virtual int  cmp(GenPtr, GenPtr) const { return 0; }
virtual void copy_key(GenPtr&)   const {}
virtual void copy_inf(GenPtr&)   const {}
virtual void clear_key(GenPtr&)  const {}
virtual void clear_inf(GenPtr&)  const {}
virtual void print_key(GenPtr)   const {}
virtual void print_inf(GenPtr)   const {}

virtual int  key_type_id() const { return UNKNOWN_TYPE_ID; }


int          count;
int          max_size;

bin_heap_item* HEAP;


void rise(int,bin_heap_item);
void gen_rise(int,bin_heap_item);
void int_rise(int,bin_heap_item);
void double_rise(int,bin_heap_item);

void sink(int,bin_heap_item);
void gen_sink(int,bin_heap_item);
void int_sink(int,bin_heap_item);
void double_sink(int,bin_heap_item);


public:

typedef bin_heap_item item;


bin_heap_item insert(GenPtr, GenPtr);
bin_heap_item find_min()  const      { return (count > 0) ? HEAP[1] : 0;  }
bin_heap_item first_item() const     { return find_min(); }
bin_heap_item next_item(bin_heap_item it) const 
{ return (it && it->index < count) ? HEAP[it->index+1] : 0;}

bin_heap_item last_item() const { return 0; }

bin_heap_item stl_pred_item(bin_heap_item) const 
{  LEDA_EXCEPTION(1,"bin_heap::stl_pred_item not implemented."); return 0; }

bin_heap_item stl_next_item(bin_heap_item it) const 
{ return (it && it->index < count) ? HEAP[it->index+1] : HEAP[1]; }


void decrease_key(bin_heap_item, GenPtr);
void change_inf(bin_heap_item, GenPtr);
void del_item(bin_heap_item);
void del_min() { del_item(find_min()); }

const GenPtr& key(bin_heap_item it) const { return it->key; }
      GenPtr& inf(bin_heap_item it) const { return it->inf; }

int  size()    const  { return count;    }
bool empty()   const  { return count==0; }

void clear();
void print();

bin_heap& operator=(const bin_heap&);

bin_heap(int n = 1024);  // default start size 
bin_heap(const bin_heap&);

bin_heap(int,int) {}

virtual ~bin_heap();

};

LEDA_END_NAMESPACE

#endif
