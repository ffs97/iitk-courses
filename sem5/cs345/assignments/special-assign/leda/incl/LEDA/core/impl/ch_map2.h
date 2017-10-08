/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ch_map2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_CH_MAP2_H
#define LEDA_CH_MAP2_H

//------------------------------------------------------------------------------
// Two-dimensional Hashing Map  (sparse matrix)
//
// S. Naeher  (1997)
//
// iteration methods (first_item & next item)  02/2001
//
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE
 

class __exportC ch_map2_elem 
{
  friend class __exportC ch_map2;

  unsigned long    k1;
  unsigned long    k2;
  GenPtr           i;
  ch_map2_elem*  succ;
};

typedef ch_map2_elem*  ch_map2_item;


//--------------------------------------------------------------------
// class ch_map2
//--------------------------------------------------------------------



class __exportC ch_map2 
{
   ch_map2_elem STOP;

   ch_map2_elem* table;
   ch_map2_elem* table_end;
   ch_map2_elem* free;
   ch_map2_elem* iterator;

   int table_size;           
   int table_size_1;           
   int shift;

   virtual void clear_inf(GenPtr&)  const { }
   virtual void copy_inf(GenPtr&)   const { }
   virtual void init_inf(GenPtr&)   const { }



   ch_map2_elem*  HASH(unsigned long x, unsigned long y)  const
   { return table + ((x+(y<<16)) & table_size_1); }

   void init_table(int);
   void rehash();

   GenPtr& access(ch_map2_item, unsigned long, unsigned long);
   ch_map2_item lookup(ch_map2_item, unsigned long, unsigned long) const;

   protected:

   typedef ch_map2_item item;

   unsigned long index1(ch_map2_item it) const { return it->k1; }
   unsigned long index2(ch_map2_item it) const { return it->k2; }

   const GenPtr& inf(ch_map2_item it) const { return it->i; }

   void clear_entries();

   GenPtr& access(unsigned long, unsigned long);
   ch_map2_item lookup(unsigned long, unsigned long) const;

   void    clear();

   ch_map2_item first_item() const;
   ch_map2_item next_item(ch_map2_item) const;

   ch_map2& operator=(const ch_map2&);
   ch_map2(const ch_map2&);
   ch_map2(int s = 0, int n=1024); 

   virtual ~ch_map2() { delete[] table; }

};



inline GenPtr& ch_map2::access(unsigned long x, unsigned long y)
{ 
  ch_map2_item p = HASH(x,y);
  if (p->k1 == x && p->k2 == y) 
    return p->i; 
  else
    if (p->k1 == 0xFFFFFFFF && p->k2 == 0xFFFFFFFF)
       { p->k1 = x;
         p->k2 = y;
         init_inf(p->i);
         return p->i;
        }
     else
       return access(p,x,y);
 }


inline ch_map2_item ch_map2::lookup(unsigned long x, unsigned long y) const
{ 
  ch_map2_item p = HASH(x,y);
  if (p->k1 == x && p->k2 == y) 
    return p; 
  else
    if (p->k1 == 0xFFFFFFFF && p->k2 == 0xFFFFFFFF)
       return nil;
     else
       return lookup(p->succ,x,y);
 }


LEDA_END_NAMESPACE

#endif
