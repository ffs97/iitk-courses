/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  p_heap.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PAIRING_HEAP_H
#define LEDA_PAIRING_HEAP_H

#include<LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

// Implementation following John T. Stasko and Jeffrey Scott Vitter
// ( Communications of the ACM   March 1987 Volume 30 Number 3   S. 234-240 )
//
// by Markus Neukirch   (1992)

class __exportC ph_item
{
  friend class __exportC pairing_heap;
  friend class __exportC p_heap;
  friend class __exportC p_aux_heap;

  LEDA_MEMORY(ph_item)

        GenPtr key;
        GenPtr inf;

        ph_item* parent;
        ph_item* l_child;
        ph_item* r_child;


        ph_item(GenPtr k,GenPtr i)
        { parent = l_child = r_child=nil;
          key=k;
          inf=i;
         }

       ~ph_item()       {} 
        
        
};


class __exportC p_heap
{
        
        ph_item* head;
        int item_count;
        void do_copy(ph_item*,ph_item*,bool);
        void copy_sub_tree(ph_item*, ph_item*);
        void clear_sub_tree(ph_item*);
        ph_item* new_ph_item(GenPtr,GenPtr);

        virtual int cmp(GenPtr, GenPtr) const 
        { LEDA_EXCEPTION(1,"p_heap: base class cmp called."); return 0; }

        virtual void print_key(GenPtr)  const {}
        virtual void print_inf(GenPtr)  const {}
        virtual void clear_key(GenPtr&) const {}
        virtual void clear_inf(GenPtr&) const {}
        virtual void copy_key(GenPtr&)  const {}
        virtual void copy_inf(GenPtr&)  const {}
 
        virtual int  key_type_id() const { return UNKNOWN_TYPE_ID; }
 
protected:

        //ph_item* comparison_link(ph_item*,ph_item*);

        ph_item* twopass(ph_item*);
        ph_item* multipass(ph_item*);

public:
        typedef ph_item* item;

        p_heap()        { item_count=0; }

        p_heap(int)     { LEDA_EXCEPTION(1,"no p_heap(int) constructor");}
        p_heap(int,int) { LEDA_EXCEPTION(1,"no p_heap(int,int) constructor");}

        p_heap(const p_heap&);
        p_heap& operator=(const p_heap&);

virtual ~p_heap() { clear(); }


        ph_item* insert(GenPtr,GenPtr);

        ph_item* find_min() const { return head; }

        void decrease_key(ph_item*,GenPtr);
        void delete_min_multipass();
        void delete_min_twopass();

        void del_min() { delete_min_twopass(); }

        const GenPtr& key(ph_item* x) const {return x->key;}
        const GenPtr& inf(ph_item* x) const {return x->inf;}


        void change_inf(ph_item* x,GenPtr inf)
                {clear_inf(x->inf);copy_inf(inf);x->inf=inf;}
        void del_item(ph_item* x)
                {decrease_key(x,head->key);delete_min_twopass();}

        void clear ();
        
        int  size() const  { return item_count; }
        int  empty() const { return item_count == 0; }

        // iteration (not yet implemented)

        ph_item* first_item()        const { return 0; }
        ph_item* next_item(ph_item*) const { return 0; }

        ph_item* last_item()        const { return 0; }

};      

LEDA_END_NAMESPACE

#endif
