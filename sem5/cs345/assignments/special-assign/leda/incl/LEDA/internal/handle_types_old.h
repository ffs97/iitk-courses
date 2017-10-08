/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  handle_types_old.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//------------------------------------------------------------------------------
// handle_base/rep: base classes for handle types string, point, segment,...
//------------------------------------------------------------------------------

LEDA_BEGIN_NAMESPACE

class __exportC handle_base;

class __exportC handle_rep  {

friend class __exportC handle_base;

public:
 LEDA_MEMORY(handle_rep)

protected:

 leda_lock mutex_count;

 int  count;

 handle_rep()  { count = 1; }
 

 unsigned long ref_num() const { return count; }

 virtual ~handle_rep()  {}

public:
 unsigned long ref_plus() 
 { mutex_count.lock();
   count++;
   mutex_count.unlock();
   return count;
  }

 unsigned long ref_minus() 
 { mutex_count.lock();
   count--;
   mutex_count.unlock();
   return count;
  }
};



class __exportC handle_base {

protected:

handle_rep* PTR;

public:

LEDA_MEMORY(handle_base)

handle_base() {}

handle_base(const handle_base& x) 
{ PTR = x.PTR;  
  PTR->count++; 
}

~handle_base() 
{ PTR->mutex_count.lock();
  if (--(PTR->count) == 0) delete PTR;
  else PTR->mutex_count.unlock();
 }

handle_base& operator=(const handle_base& x)
{ 
  x.PTR->count++;
  PTR->mutex_count.lock();
  if (--(PTR->count) == 0) delete PTR;
  else PTR->mutex_count.unlock();
  PTR = x.PTR;
  return *this;
 }

int refs() const { return PTR->count; }


friend unsigned long ID_Number(const handle_base& x)
{ return (unsigned long)x.PTR; }

friend bool identical(const handle_base& x, const handle_base& y)
{ return x.PTR == y.PTR; }

};

#define HANDLE_BASE(T) handle_base


LEDA_END_NAMESPACE
