/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  os2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SYS_OS2_H
#define LEDA_SYS_OS2_H

#define __os2__

#define LITTLE_ENDIAN_MACHINE

#if defined(__IBMCPP__)
#define __EIGHT_CHAR_FILE_NAMES__
#endif

#if defined(__STDC__)
#define STRINGIZE(x) #x
#else
#define STRINGIZE(x) "x"
#endif


//------------------------------------------------------------------------------
//  DLL definitions
//------------------------------------------------------------------------------

#if defined(BUILD_DLL)
#define __exportC __export
#define __exportF __export
#define __exportD __export
#else
#define __exportC
#define __exportF
#define __exportD
#endif

#define __exportGeoW
#endif
