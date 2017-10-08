/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dph_array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DPHARRAY_H
#define LEDA_DPHARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600041
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// dph_array  
//------------------------------------------------------------------------------ 
#include <LEDA/system/basic.h> 
#include <LEDA/core/slist.h> 
#include <LEDA/core/impl/dp_hash.h> 

LEDA_BEGIN_NAMESPACE


template<class itype, class etype>

class dph_array : public dp_hash {
typedef dp_hash base;

void clear_key(GenPtr& x)   const { LEDA_CLEAR(itype,x); }
void clear_inf(GenPtr& x)   const { LEDA_CLEAR(etype,x); }
void copy_key(GenPtr& x)    const { LEDA_COPY(itype,x);  }
void copy_inf(GenPtr& x)    const { LEDA_COPY(etype,x);  }


etype edef;

SLIST def_list;
slist_item iterator;

public:

etype& operator[](const itype& y) 
{ stp i=base::lookup(leda_cast(y));
  if (i==nil) 
  { GenPtr p = leda_cast(edef);
    i=base::insert(leda_cast(y),p);
    def_list.append(leda_cast(y));  
   }
  return LEDA_ACCESS(etype,inf(i)); 
}

const etype& operator[](const itype& y) const
{ stp it = base::lookup(leda_cast(y));
  if (it) return LEDA_CONST_ACCESS(etype,inf(it));
  else return edef;
 }

bool defined(const itype& y) const { return (base::lookup(leda_cast(y)) != nil); }

slist_item first_item()             const { return def_list.first(); }
slist_item next_item(slist_item it) const { return it ? def_list.succ(it) : 0; }

itype inf(slist_item it) const { return LEDA_CONST_ACCESS(itype,def_list[it]); }



 dph_array() { }
 dph_array(int n,itype* I,etype* E): dp_hash(n,(GenPtr*)I,(GenPtr*)E) { }
 dph_array(etype i) { edef = i; }

 dph_array(const dph_array<itype,etype>& A): dp_hash((dp_hash&)A) { edef = A.edef; }

 virtual ~dph_array() { def_list.clear(); }
};



#if LEDA_ROOT_INCL_ID == 600041
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
