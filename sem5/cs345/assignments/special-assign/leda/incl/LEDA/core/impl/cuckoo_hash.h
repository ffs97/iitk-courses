/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  cuckoo_hash.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


/*
 * cuckoo_hash.h
 *
 * original Authors:
 *     Rasmus Pagh and Flemming Friche Rodler
 *     BRICS (Basic Research In Computer Science}
 *     Department of Computer Science
 *     University of Aarhus, Denmark
 *     {pagh,ffr}@brics.dk
 * 
*/

#ifndef CUCKOO_HASH_H
#define CUCKOO_HASH_H

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

//---------------------------------------------------------------
// class cuckoo_hash_elem  
//---------------------------------------------------------------

class __exportC cuckoo_hash_elem 
{
  friend class __exportC cuckoo_hash;

  bool   used;
  GenPtr key;
  GenPtr inf;

  public:

  LEDA_MEMORY(cuckoo_hash_elem);
};

typedef cuckoo_hash_elem* cuckoo_hash_item;


//----------------------------------------------------------------
// class cuckoo_hash
//----------------------------------------------------------------

class __exportC cuckoo_hash
{
  int table_size;       /* size of hash tables */
  int min_size;         /* rehash trigger sizes */
  int mean_size;
  int count;            /* current size */
  int max_chain;        /* max. interations in insert */
  
  int a1[4];            /* values for hash function 1 : a[3] : shift */
  int a2[4];            /* values for hash function 2  */

  cuckoo_hash_elem* T1; /* pointer to hash table 1 */
  cuckoo_hash_elem* T2; /* pointer to hash table 2 */
  
  typedef unsigned long ulong;


  void init_hashfunction(int* a)
  {
    a[0] = (rand_int() << 1) + 1;
    a[1] = (rand_int() << 1) + 1;
    a[2] = (rand_int() << 1) + 1;
  }


  void hash_cuckoo(ulong& hkey, const int* a, const int& key) const
  {
    hkey = (a[0]*key) ^ (a[1]*key) ^ (a[2]*key);
#ifdef WORD_LENGTH_64
    hkey &= 0xffffffff;
#endif
    hkey = hkey >> a[3];
  }
  
  
  bool rehash_insert(const cuckoo_hash_elem& el)
  {
    cuckoo_hash_elem x = el, tmp;
  
    ulong hkey;
  
    int i; 
    for (i = 0; i < max_chain; i++)
    {
      hash_cuckoo(hkey,a1,hash_fct(x.key));  
      tmp = T1[hkey];      
      T1[hkey] = x;    
      if (!tmp.used) return true;
    
      x = tmp;

      hash_cuckoo(hkey,a2,hash_fct(x.key));    
      tmp = T2[hkey];    
      T2[hkey] = x;
      if (!tmp.used) return true;
    
      x = tmp;
    }
 
    /* bad hash functions */
	for (i = 0; i < table_size; i++) {
		T1[i].used = false;
		T2[i].used = false;
	}

    init_hashfunction(a1);
    init_hashfunction(a2); 

    return false;
  }

  void init(int); 
  void rehash(int);
  void destroy();
  void assign(const cuckoo_hash& D);
  void insert_new_element(GenPtr k, GenPtr i);
  
//----------------------------------------------------------------
    
  virtual int key_type_id() const { return UNKNOWN_TYPE_ID; }

  virtual int hash_fct(GenPtr) const 
  { LEDA_EXCEPTION(1,"cuckoo::hash_fct() called"); return 0; }
  
  virtual bool equal_key(GenPtr, GenPtr) const { return false; }
  virtual void clear_key(GenPtr&) const {}
  virtual void clear_inf(GenPtr&) const {}
  virtual void copy_key(GenPtr&)  const {}
  virtual void copy_inf(GenPtr&)  const {}
  virtual void print_key(GenPtr)  const {}

  public:
  
  typedef cuckoo_hash_item item;

  cuckoo_hash(int ts = 4) { init(ts); }
  
  cuckoo_hash(const cuckoo_hash& D) { assign(D); }
  cuckoo_hash& operator=(const cuckoo_hash& D) { destroy(); assign(D); return *this; }
  
  virtual ~cuckoo_hash() { destroy(); }
  
  void clear() { destroy(); init(1 << 8); }
  
  cuckoo_hash_item lookup(GenPtr) const;
  cuckoo_hash_item insert(GenPtr, GenPtr);
  
  void del(GenPtr k) { del_item(lookup(k)); }
  void del_item(cuckoo_hash_item);
  
  bool member(GenPtr x) const { return lookup(x) ? true : false; }
  
  const GenPtr& key(cuckoo_hash_item it) const { return it->key; }
  GenPtr&       inf(cuckoo_hash_item it) const { return it->inf; }

  void change_inf(cuckoo_hash_item, GenPtr);

  bool empty()     const { return count == 0; }
  int  size()      const { return count; }
  int  tablesize() const { return table_size; }
};

LEDA_END_NAMESPACE

#endif
