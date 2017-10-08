/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dlist.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_DLIST_H
#define LEDA_DLIST_H

//------------------------------------------------------------------------------
//  doubly linked lists
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

class __exportC dlist; 
class __exportC dlink;

typedef dlink* list_item;

class __exportC dlink {

public:

  dlink* succ;
  dlink* pred;

  dlink(dlink* p=0, dlink* s=0) : succ(s), pred(p) {}

/*
  LEDA_MEMORY(dlink)
*/

  friend class __exportC dlist;

public:

  dlink* get_succ() { return succ; }
  dlink* get_pred() { return pred; }

  void set_succ(dlink* p) { succ = p; }
  void set_pred(dlink* p) { pred = p; }

};




//------------------------------------------------------------------------------
// dlist: base class for all doubly linked lists
//------------------------------------------------------------------------------

class __exportC  dlist {

protected:

  dlink* h;                     //head
  dlink* t;                     //tail
  int    count;                 //length of list

virtual int  ord(dlink*) const { return 0; }

virtual dlink* copy_elem(dlink*)    const { return 0; }
virtual dlink* read_elem(istream&)  const { return 0; }
virtual void   print_elem(dlink*, ostream&)  const {}

virtual void   clear_inf(dlink*) {}
virtual void   swap_inf(dlink*,dlink*) {}

virtual int  el_size() const { return 0; }
virtual int  el_type_id() const { return UNKNOWN_TYPE_ID; }

void recompute_length() const;

public:

// access operations

   int  length() const { if (count < 0) recompute_length(); return count; }
   int  size()   const { return length(); }
   bool empty()  const { return h==nil; }

   // iteration
   dlink* first_item()          const { return h; }
   dlink* last_item()           const { return t; }
   dlink* next_item(dlink* p)   const { return p ? p->succ : 0; }
   dlink* pred_item(dlink* p)   const { return p ? p->pred : 0; }

   dlink* stl_next_item(dlink* p)   const { return p ? p->succ : h; }
   dlink* stl_pred_item(dlink* p)   const { return p ? p->pred : t; }

   dlink* first()               const { return h; }
   dlink* last()                const { return t; }
   dlink* succ(dlink* p)        const { return p->succ; }
   dlink* pred(dlink* p)        const { return p->pred; }
   dlink* cyclic_succ(dlink* p) const { return p->succ? p->succ : h; }
   dlink* cyclic_pred(dlink* p) const { return p->pred? p->pred : t; }
   
   dlink* succ(dlink* l, int i) const; 
   dlink* pred(dlink* l, int i) const;
   dlink* get_item(int = 0)     const; 


// update operations

   dlink* insert(dlink* a, dlink* l, int dir=0);
   dlink* push(dlink* a);
   dlink* append(dlink* a);
   
   dlink* pop();
   dlink* pop_back();
   dlink* del(dlink* it);
   dlink* del(dlink* p, dlink* q, dlink* r); // for unique

   void  move_to_front(dlink*);
   void  move_to_back(dlink*);

   void  swap(dlist&);
   void  splice(dlink*s, dlist&, dlink*, dlink*);
   void  splice(dlink*s, dlist&, dlink*);
   void  splice(dlink*s, dlist&);
   void  conc(dlist&, int=leda::behind);
   void  split(dlink*, dlist&, dlist&, int=leda::before);
   void  extract(dlink* it1, dlink* it2, dlist& result, bool inclusive = true);
   void  bucket_sort(int,int);
   void  bucket_sort();
   void  reverse_items();
   void  reverse();
   void  reverse_items(dlink*,dlink*);
   void  reverse(dlink*,dlink*);
   void  permute(dlink**);
   void  permute();
   void  clear();


// operators

   dlist  operator+(const dlist&); 


// I/O

   void print(ostream&,string, char)       const;    
   void print(ostream& out,char space=' ') const;
   void print(string s, char space=' ')    const { print(cout,s,space);  }
   void print(char space=' ')              const { print(cout,space); }   


   void read(istream&,string, int);  

   void read(istream& in,int delim) { read(in,"",delim); }
   void read(istream& in)           { read(in,"",EOF); }
   void read(string s, int delim)   { read(cin,s,delim); }   
   void read(string s)              { read(cin,s,'\n'); }   
   void read(char delim)            { read(cin,"",delim);}  
   void read()                      { read(cin,"",'\n');}  


// constructors

   dlist();    
   dlist(const dlist&);
   dlist& operator=(const dlist&); 

   virtual ~dlist()  { clear(); }

// misc

   int space()  const { return sizeof(dlist) + length() * sizeof(dlink); }
};


LEDA_END_NAMESPACE

#endif
