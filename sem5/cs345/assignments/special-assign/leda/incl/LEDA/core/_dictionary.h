/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  _dictionary.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef __LEDA_DICTIONARY_H
#define __LEDA_DICTIONARY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600010
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/dictionary.h>

LEDA_BEGIN_NAMESPACE

template <class K, class I, class impl> 
class _dictionary : public dictionary<K,I,impl> {};

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600010
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif
#endif

