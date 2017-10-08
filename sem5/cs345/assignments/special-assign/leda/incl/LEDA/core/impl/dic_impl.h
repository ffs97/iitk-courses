/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dic_impl.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




LEDA_BEGIN_NAMESPACE

typedef int dic_impl_item;

class __exportC dic_impl {

private:

virtual int  key_type_id()       const = 0;

virtual int  cmp(GenPtr, GenPtr) const = 0;
virtual void clear_key(GenPtr&)  const = 0;
virtual void clear_inf(GenPtr&)  const = 0;
virtual void copy_key(GenPtr&)   const = 0;
virtual void copy_inf(GenPtr&)   const = 0;

//
// private data
//


public:

typedef dic_impl_item item;

 dic_impl();
 dic_impl(const dic_impl&);
~dic_impl();

dic_impl& operator=(const dic_impl&);

const GenPtr& key(dic_impl_item p)  const;
GenPtr&       inf(dic_impl_item p)  const;

dic_impl_item insert(GenPtr,GenPtr);
dic_impl_item lookup(GenPtr)  const;
dic_impl_item first_item()    const;
dic_impl_item next_item(dic_impl_item) const;


void    change_inf(dic_impl_item,GenPtr);
void    del_item(dic_impl_item);
void    del(GenPtr);
void    clear();

int     size() const;

};


LEDA_END_NAMESPACE


