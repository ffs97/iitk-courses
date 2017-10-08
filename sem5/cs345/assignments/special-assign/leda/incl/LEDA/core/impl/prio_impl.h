/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  prio_impl.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



LEDA_BEGIN_NAMESPACE

class __exportC PRIO_IMPL_CLASS
{ 

private:

PRIO_IMPL_DATA

virtual int  key_type_id() const = 0;

virtual int  cmp(GenPtr, GenPtr) const = 0;
virtual void clear_key(GenPtr&)  const = 0;
virtual void clear_inf(GenPtr&)  const = 0;
virtual void copy_key(GenPtr&)   const = 0;
virtual void copy_inf(GenPtr&)   const = 0;


protected:

 typedef PRIO_IMPL_ITEM item;

public:

 PRIO_IMPL_CLASS();
 PRIO_IMPL_CLASS(int);
 PRIO_IMPL_CLASS(int,int);
 PRIO_IMPL_CLASS(const PRIO_IMPL_CLASS&);
virtual ~PRIO_IMPL_CLASS();

PRIO_IMPL_CLASS& operator=(const PRIO_IMPL_CLASS&);

PRIO_IMPL_ITEM insert(GenPtr,GenPtr);
PRIO_IMPL_ITEM find_min() const;
PRIO_IMPL_ITEM first_item() const;
PRIO_IMPL_ITEM next_item(PRIO_IMPL_ITEM) const;

const GenPtr& key(PRIO_IMPL_ITEM) const;
const GenPtr& inf(PRIO_IMPL_ITEM) const;

void del_min();
void del_item(PRIO_IMPL_ITEM);
void decrease_key(PRIO_IMPL_ITEM,GenPtr);
void change_inf(PRIO_IMPL_ITEM,GenPtr);
void clear();
 
int  size()  const;

};

LEDA_END_NAMESPACE

