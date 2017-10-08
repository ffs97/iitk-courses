/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  olist.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_OLIST_H
#define LEDA_OLIST_H

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
//
//  obj_list  (doubly linked lists of obj_links)
//
//  each "obj_link" may be present in at most one list 
//  
//------------------------------------------------------------------------------

class obj_list; 
class obj_link;

typedef int  (*CMP_ITEM)(obj_link*,obj_link*);
typedef void (*APP_ITEM)(obj_link*);
typedef int  (*ORD_ITEM)(obj_link*);

//------------------------------------------------------------------------------
// class obj_link (base class for all list items)
//------------------------------------------------------------------------------

class __exportC obj_link {

protected:

  obj_link* succ_link;
  obj_link* pred_link;

public:

void del_item() 
{ pred_link->succ_link = succ_link; 
  succ_link->pred_link = pred_link; 
 }

//public:

//  obj_link() { succ = nil; }

  obj_link* succ_item() { return succ_link; }
  obj_link* pred_item() { return pred_link; }

  friend class __exportC obj_list;
  friend class __exportC c_obj_list;
  friend class __exportC sc_obj_list;
// friend class __exportC node_list;
// friend class __exportC graph;
};


//------------------------------------------------------------------------------
// obj_list: base class for all doubly linked lists
//------------------------------------------------------------------------------

class __exportC obj_list {

   obj_link* h;           // head
   obj_link* t;           // tail
   int count;             // length of list

void quick_sort(obj_link**,obj_link**,CMP_ITEM);

void insertion_sort(obj_link**,obj_link**,obj_link**,CMP_ITEM);

public:


// access operations

   int  length() const { return count; }
   int  size()   const { return count; }
   bool empty()  const { return count==0; }

   obj_link* first()               const { return h; }
   obj_link* first_item()          const { return h; }
   obj_link* last()                const { return t; }
   obj_link* last_item()           const { return t; }
   obj_link* next_item(obj_link* p)   const { return p ? p->succ_link : 0; }
   obj_link* succ(obj_link* p)        const { return p->succ_link; }
   obj_link* pred(obj_link* p)        const { return p->pred_link; }

   obj_link* cyclic_succ(obj_link* p) const 
   { return p->succ_link? p->succ_link : h; }

   obj_link* cyclic_pred(obj_link* p) const 
   { return p->pred_link? p->pred_link : t; }

   obj_link* succ(obj_link* l, int i) const; 
   obj_link* pred(obj_link* l, int i) const;
   obj_link* get_item(int = 0)     const; 

   obj_link* max(CMP_ITEM) const;
   obj_link* min(CMP_ITEM) const;
   obj_link* search(obj_link*) const;

   int    rank(obj_link*) const;

// update operations

   obj_link* insert(obj_link* p, obj_link* l);
   obj_link* insert(obj_link* p, obj_link* l, int dir);
   obj_link* push(obj_link* p);
   obj_link* append(obj_link* p);

   void del(obj_link* loc);
   obj_link* pop();
   obj_link* pop_back();

   void   conc(obj_list&);
   void   split(obj_link*,obj_list&,obj_list&);
   void   apply(APP_ITEM);
   void   sort(CMP_ITEM);
   void   bucket_sort(int,int,ORD_ITEM);
   void   permute();
   void   clear();

   obj_list& operator=(const obj_list&); 
   obj_list  operator+(const obj_list&); 

// constructors & destructors

   obj_list();    

 //obj_list(const obj_list&);

  ~obj_list()  { clear(); }

};



inline obj_link* obj_list::push(obj_link* p)   
{ count++;
  p->pred_link = 0;
  p->succ_link = h;
  if (h) 
      h = h->pred_link = p;
  else   
      h = t =  p;
  return p;
 }


inline obj_link* obj_list::append(obj_link* p)
{ count++;
  p->pred_link = t;
  p->succ_link = 0;
  if (t) 
      t = t->succ_link = p;
  else  
      t = h = p;
  return p; 
 } 


inline obj_link* obj_list::pop()    
{ obj_link* p=h; 
  if (p) 
  { if (--count) 
      { h = h->succ_link; 
        h->pred_link = 0; 
       }
    else  
      h = t = 0;
   }
  return p;
 }


inline obj_link* obj_list::pop_back()    
{ obj_link* p=t; 
  if (p)
  { if (--count) 
      { t = t->pred_link; 
        t->succ_link = 0; 
       }
    else  
      h = t = 0;
   }
  return p;
 }


inline obj_link* obj_list::insert(obj_link* n, obj_link* p) 
{ // insert n after p
  obj_link* s=p->succ_link;
  n->pred_link = p;
  n->succ_link = s;
  p->succ_link = n;
  if (p==t) t=n; else s->pred_link = n;
  count++;
  return n;
}




//------------------------------------------------------------------------------
//
// c_obj_list (doubly linked circular object list)
//
// simple and efficient implementation (no counter, iterator, sorting, etc.)
// removed items are assigned a nil succ pointer 
// member(x) <==>  x->succ != nil
//
//------------------------------------------------------------------------------

class __exportC c_obj_list : public obj_link {

// the list head is an obj_link, namely the predecessor of the first 
// and the successor of the last element

public:

bool empty()  const { return succ_link == (obj_link*)this; }

obj_link* first()      const { return (succ_link==(obj_link*)this) ? nil : succ_link; }
obj_link* last()       const { return (pred_link==(obj_link*)this) ? nil : pred_link; }
obj_link* first_item() const { return first(); }
obj_link* last_item()  const { return last(); }

obj_link* succ(obj_link* p) const 
{ return (p->succ_link==(obj_link*)this)? nil : p->succ_link;}

obj_link* pred(obj_link* p) const 
{ return (p->pred_link==(obj_link*)this)? nil : p->pred_link;}

obj_link* cyclic_succ(obj_link* p) const 
{ return (p->succ_link==(obj_link*)this) ? succ_link : p->succ_link; }

obj_link* cyclic_pred(obj_link* p) const 
{ return (p->pred_link==(obj_link*)this) ? pred_link : p->pred_link; }


obj_link* next_item(obj_link* p) const { return p ? succ(p) : 0; }


 void insert(obj_link* n, obj_link* p) 
 { // insert n insert after p
   obj_link* s=p->succ_link;
   n->pred_link = p;
   n->succ_link = s;
   p->succ_link = n;
   s->pred_link = n;
  }

 obj_link* del(obj_link* x)
 { obj_link*  p = x->pred_link;
   obj_link*  s = x->succ_link;
   p->succ_link = s;
   s->pred_link = p;
   x->succ_link = nil;
   return x;
  }

 obj_link* del(obj_link* x, obj_link* y)
 { obj_link*  p = x->pred_link;
   obj_link*  s = y->succ_link;
   p->succ_link = s;
   s->pred_link = p;
   return x;
  }

 void move(obj_link* x, obj_link* y)
 { obj_link* px = x->pred_link;
   obj_link* sx = y->succ_link;
   obj_link* sy = y->succ_link;
   px->succ_link = sx;
   sx->pred_link = px;
   x->pred_link = y;
   x->succ_link = sy;
   y->succ_link = x;
   sy->pred_link = x;
  }


 bool member(obj_link* x) { return x->succ_link != nil; }

 void push(obj_link* p)   { insert(p,this); }
 void append(obj_link* p) { insert(p,pred_link); }

 obj_link* pop()      { return del(succ_link); }
 obj_link* pop_back() { return del(pred_link); }

 void clear() 
 { while(succ_link != this) 
   { obj_link* p = succ_link;
     succ_link = p->succ_link;
     p->succ_link = nil;
    }
   pred_link = this; 
  }

 void init() { succ_link = pred_link = this; }

// constructors & destructors

 c_obj_list()  { succ_link = pred_link = this; }
~c_obj_list()  { clear(); }

};


LEDA_END_NAMESPACE

#endif

