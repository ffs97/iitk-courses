/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_obj_list.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// class graph_object: base class for all graph objects
//------------------------------------------------------------------------------

class __exportC graph_object {
friend class __exportC graph_obj_list_base;
protected:
  graph_object* obj_list_succ;
  graph_object* obj_list_pred;
};


class __exportC graph_obj_list_base
{ 
  graph_object* obj_list_head;
  graph_object* obj_list_tail;
  int           obj_list_sz;

public:
  void push(graph_object*);
  void append(graph_object*);
  void insert(graph_object*,graph_object*);
  void remove(graph_object*);
  graph_object* pop();
  void conc(graph_obj_list_base&);
  void clear();

  graph_object* head() const { return obj_list_head; }
  graph_object* tail() const { return obj_list_tail; }
  graph_object* succ(graph_object* e) const { return e->obj_list_succ; }
  graph_object* pred(graph_object* e) const { return e->obj_list_pred; }
  bool empty()  const { return obj_list_head == 0; }
  int  length() const { return obj_list_sz; }

  graph_obj_list_base() { clear(); }
};

LEDA_END_NAMESPACE

