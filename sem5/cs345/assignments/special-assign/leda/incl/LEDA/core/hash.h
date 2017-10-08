/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  hash.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_HASH_H
#define LEDA_HASH_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600031
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// hash dictionary (based on hashing with chaining)
//------------------------------------------------------------------------------

#include <LEDA/core/impl/ch_hash.h>

LEDA_BEGIN_NAMESPACE

typedef ch_hash_item hash_item;


template<class ktype, class itype> 

class hash : public ch_hash {

int (*hash_ptr)(const ktype&);
  
int  key_type_id() const { return LEDA_TYPE_ID(ktype); }

bool equal_key(GenPtr x, GenPtr y) const { return LEDA_EQUAL(ktype,x,y); }
int  hash_fct(GenPtr x)   const 
{ return (hash_ptr==0) ? LEDA_HASH(ktype,x):(*hash_ptr)(LEDA_CONST_ACCESS(ktype,x)); }
void clear_key(GenPtr& x) const { LEDA_CLEAR(ktype,x); }
void clear_inf(GenPtr& x) const { LEDA_CLEAR(itype,x); }
void copy_key(GenPtr& x)  const { LEDA_COPY(ktype,x); }
void copy_inf(GenPtr& x)  const { LEDA_COPY(itype,x); }

//void print_key(GenPtr x)  const { LEDA_PRINT(ktype,x,cout); }
//void print_inf(GenPtr x)  const { LEDA_PRINT(itype,x,cout); }

public:

hash_item lookup(const ktype& y)  const { return ch_hash::lookup(leda_cast(y)); }
int       defined(const ktype& x) const { return (lookup(x)) ? false : true; }
void      change_inf(hash_item it, const itype& i)
                               { ch_hash::change_inf(it,leda_cast(i)); }
hash_item insert(const ktype& y, const itype& x)
                               { return ch_hash::insert(leda_cast(y),leda_cast(x));}
void   del(const ktype& y)            { ch_hash::del(leda_cast(y)); } 
void   del_item(hash_item it)    { del(key(it)); } 

const ktype& key(hash_item it) const
{ return LEDA_CONST_ACCESS(ktype,ch_hash::key(it)); }

const itype& inf(hash_item it) const
{ return LEDA_CONST_ACCESS(itype,ch_hash::inf(it)); }

hash()                       { hash_ptr=0; }
hash(int (*f)(const ktype&)) { hash_ptr=f; }

hash(int s)  : ch_hash(s) { hash_ptr=0;}
hash(int s, int (*f)(const ktype&)) : ch_hash(s) { hash_ptr=f;}

~hash() { clear(); }

} ;



#if LEDA_ROOT_INCL_ID == 600031
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
