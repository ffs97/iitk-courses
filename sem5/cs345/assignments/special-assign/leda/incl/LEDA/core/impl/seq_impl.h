/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  seq_impl.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



LEDA_BEGIN_NAMESPACE

typedef int seq_impl_item;

class __exportC seq_impl  {

virtual int  key_type_id() const = 0;

virtual int  cmp(GenPtr, GenPtr) const = 0;
virtual void clear_key(GenPtr&)  const = 0;
virtual void clear_inf(GenPtr&)  const = 0;
virtual void copy_key(GenPtr&)   const = 0;
virtual void copy_inf(GenPtr&)   const = 0;


public:

typedef seq_impl_item item;

 seq_impl();
 seq_impl(const seq_impl&);
~seq_impl();

seq_impl& operator=(const seq_impl&);
seq_impl& conc(seq_impl&);

seq_impl_item insert(GenPtr,GenPtr);
seq_impl_item insert_at_item(seq_impl_item,GenPtr,GenPtr);
seq_impl_item lookup(GenPtr)      const;
seq_impl_item locate(GenPtr)      const;
seq_impl_item locate_pred(GenPtr) const;
seq_impl_item succ(seq_impl_item) const;
seq_impl_item pred(seq_impl_item) const;


GenPtr key(seq_impl_item) const;
GenPtr inf(seq_impl_item) const;

void reverse_items(seq_impl_item,seq_impl_item); 

void del(GenPtr); 
void del_item(seq_impl_item); 
void split_at_item(seq_impl_item,seq_impl&,seq_impl&);
void change_inf(seq_impl_item,GenPtr);
void clear();

int  size()  const;

};

LEDA_END_NAMESPACE

