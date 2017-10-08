/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  _sortseq.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef _LEDA_SORTSEQ_H
#define _LEDA_SORTSEQ_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600029
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/sortseq.h>

LEDA_BEGIN_NAMESPACE

template <class K, class I, class impl> 
class _sortseq : public sortseq<K,I,impl> {};

#if LEDA_ROOT_INCL_ID == 600029
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
