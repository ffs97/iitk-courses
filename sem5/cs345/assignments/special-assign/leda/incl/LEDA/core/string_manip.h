/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  string_manip.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_STRING_MANIP_H
#define LEDA_STRING_MANIP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600018
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/string.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE

extern __exportF void break_into_words(string s, int&, char**&);

extern __exportF list<string>  break_into_words(string s);
/*{\Mfunc   returns the list of words (separated by white space) of |s|. }*/

extern __exportF list<string>  break_into_lines(string s);
/*{\Mfunc   returns the list of lines (separated by newline) of |s|. }*/

inline void remove_quotes(string& s)
{
	if (s.length() >= 2 && (s[0] == '\'' || s[0] == '"') 
		&& (s[0] == s[s.length()-1]))
	{ s = s(1,s.length()-2); }
}
/*{\Mfunc   if |s| starts and ends with matching quotes, these quotes are 
            removed from |s|. }*/

#if LEDA_ROOT_INCL_ID == 600018
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

