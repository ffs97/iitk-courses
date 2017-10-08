/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  _prio.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef _LEDA_PRIO_H 
#define _LEDA_PRIO_H 

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600035
#include <LEDA/internal/PREAMBLE.h>
#endif
#include <LEDA/core/prio.h>

LEDA_BEGIN_NAMESPACE

template <class ktype, class I, class impl> 
class _priority_queue : public priority_queue<ktype,I,impl> {};


#if LEDA_ROOT_INCL_ID == 600035
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
