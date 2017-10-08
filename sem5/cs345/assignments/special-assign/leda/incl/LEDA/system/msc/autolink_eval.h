/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  autolink_eval.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#pragma message("Autolinking LEDA (DLL) ... (" __FILE__ ")")

#ifndef LEDA_DLL
#define LEDA_DLL
#endif

#if defined(_DLL)

	#ifndef _MT
		#error Unknown option combination
	#endif

	#pragma message("Autolinking LEDA (DLL) Opt:MDd ...")
	#pragma comment(lib, "leda_mdd.lib")
	#pragma message("Autolinking GeoW (LIB) Opt:MDd DLL ...")
	#pragma comment(lib, "libGeoW_mdd_dll.lib")

#else // NOT _DLL

    #error Wrong setting, in Project Settings choose C/C++ | Code Generation | Runtime library and enter option /MD (or /MDd)

#endif

#pragma comment(lib, "wsock32.lib")
