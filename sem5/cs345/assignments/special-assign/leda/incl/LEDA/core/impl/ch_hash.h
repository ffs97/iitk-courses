/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ch_hash.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_CH_HASHING_H
#define LEDA_CH_HASHING_H
#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE
 
//---------------------------------------------------------------
// class ch_hash_elem  
//---------------------------------------------------------------

class __exportC ch_hash_elem 
{
  friend class __exportC ch_hash;

  ch_hash_elem* succ;
  GenPtr k;
  GenPtr i;


  public:

  ch_hash_elem(GenPtr key, GenPtr inf, ch_hash_elem* next = 0) 
  { k = key; 
    i = inf; 
    succ = next;
   }

  ch_hash_elem() {}

  LEDA_MEMORY(ch_hash_elem)

};

typedef ch_hash_elem*  ch_hash_item;


//----------------------------------------------------------------
// class ch_hash
//----------------------------------------------------------------

class __exportC ch_hash 
{
   ch_hash_elem STOP;

   ch_hash_elem* table;

   int table_size;           
   int table_size_1;           
   int low_table;           
   int high_table;           
   int count;

   virtual int key_type_id() const { return UNKNOWN_TYPE_ID; }

   virtual int hash_fct(GenPtr)  const 
   { LEDA_EXCEPTION(1,"ch_hash: base hash function called.");
     return 0; 
    }


   virtual bool equal_key(GenPtr, GenPtr) const { return false; }
   virtual void clear_key(GenPtr&) const { }
   virtual void clear_inf(GenPtr&) const { }
   virtual void copy_key(GenPtr&)  const { }
   virtual void copy_inf(GenPtr&)  const { }
   virtual void print_key(GenPtr)  const { }

   
   ch_hash_item table_entry_for(GenPtr k) const
   { return table + (hash_fct(k) & table_size_1);}


   void init(int T);
   /*{\Xop initializes a table of size $T$.}*/

   void rehash(int T);
   /*{\Xop rehashes to a table of size $T$.}*/

   void destroy();
   /*{\Xop destroys a hash table by clearing and deleting all list elements
           and by deleting the table}*/

   ch_hash_item insert_at_pos(ch_hash_item pos, GenPtr x,GenPtr y);

public:

   typedef ch_hash_item item;

    
   ch_hash(int ts = 1)    { init(ts);}

   ch_hash(const ch_hash&);
   ch_hash& operator=(const ch_hash&);

   virtual ~ch_hash()     { destroy(); }

   void clear()           { destroy(); init(1<<10);} 

   ch_hash_item lookup(GenPtr x) const;
   /*{\Xop returns the hash item with key |x| 
           (|nil| if there is no such item)}*/

   ch_hash_item insert(GenPtr x,GenPtr y);
   /*{\Xop if no item with key $x$ is present  the $\Litem{x,y}$ is added, 
           if there is such an item its information is changed to $i$. }*/ 

   ch_hash_item fast_insert(GenPtr x,GenPtr y);
   /*{\Xop adds $\Litem{x,y}$ to the hash table.\\
           \precond there is no item with key $x$. }*/


   void del(GenPtr x);
   /*{\Xop removes the item with key $x$}*/

   void del_item(ch_hash_item q);
   /*{\Xop removes the item $q$.}*/


   bool member(GenPtr x)   const  { return ( lookup(x) ? true : false ); } 

   const GenPtr& key(ch_hash_item p) const { return p->k; }
   GenPtr&       inf(ch_hash_item p) const { return p->i; }

   void change_inf(ch_hash_item, GenPtr);
   bool empty() const     { return count ? false : true ; } 
   int  size()  const     { return count; } 
   int  tablesize() const { return table_size ; }


   ch_hash_item first_item() const;
   /*{\Xop returns the first used item in |this|.}*/
   
   ch_hash_item next_item(ch_hash_item q) const;
   /*{\Xop returns the first used item after |q| in |this|.}*/

   ch_hash_item stl_next_item(ch_hash_item p) const 
    { return p ? next_item(p) : first_item(); }

   // dummy
   ch_hash_item stl_pred_item(ch_hash_item) const { return 0; }

};

LEDA_END_NAMESPACE

#endif

