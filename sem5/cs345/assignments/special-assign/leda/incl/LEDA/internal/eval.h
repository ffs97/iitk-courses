/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  eval.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/
#ifndef LEDA_EVAL_H
#define LEDA_EVAL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600266
#include <LEDA/internal/PREAMBLE.h>
#endif

LEDA_BEGIN_NAMESPACE


	class EvalMsg
	{
		static string GetWinTmpDir();
		static string GetLogFileName();
		static void LogMsg(const char* msg);
		
	public:
		static void print(const char* msg, bool append_eval_msg = true);
	};

	extern EvalMsg evalHandler;

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600266
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
