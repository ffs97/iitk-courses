/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  autolink_dll.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef _LINK_GeoW
	#ifndef _LINK_AGD
		#pragma message("Autolinking LEDA (DLL) ... (" __FILE__ ")")
	#else
		#pragma message("Autolinking LEDA (DLL) & AGD (LIB) ... (" __FILE__ ")")
	#endif
#else
	#ifndef _LINK_AGD
		#pragma message("Autolinking LEDA (DLL) & GeoW (LIB) ... (" __FILE__ ")")
	#else
		#pragma message("Autolinking LEDA (DLL), GeoW (LIB) & AGD (LIB) ... (" __FILE__ ")")
	#endif
#endif

#ifndef LEDA_DLL
#define LEDA_DLL
#endif

#ifdef LEDA_LINK_NoOptions

	#pragma message("Autolinking LEDA (DLL) (no options) ...")
	#pragma comment(lib, "leda.lib")
    #ifdef _LINK_GeoW
		#pragma message("Autolinking GeoW (LIB) Opt: DLL ...")
		#pragma comment(lib, "libGeoW_dll.lib")
	#endif // _LINK_GeoW
    #ifdef _LINK_AGD
		#pragma message("Autolinking AGD (LIB) Opt: DLL ...")
		#pragma comment(lib, "libAGD_dll.lib")
	#endif // _LINK_AGD

#elif defined(_DLL)

	#ifndef _MT
		#error Unknown option combination
	#endif

	#ifdef _DEBUG
		#pragma message("Autolinking LEDA (DLL) Opt:MDd ...")
		#pragma comment(lib, "leda_mdd.lib")
        #ifdef _LINK_GeoW
			#pragma message("Autolinking GeoW (LIB) Opt:MDd DLL ...")
			#pragma comment(lib, "libGeoW_mdd_dll.lib")
		#endif // _LINK_GeoW
        #ifdef _LINK_AGD
			#pragma message("Autolinking AGD (LIB) Opt:MDd DLL ...")
			#pragma comment(lib, "libAGD_mdd_dll.lib")
		#endif // _LINK_AGD
	#else
		#pragma message("Autolinking LEDA (DLL) Opt:MD ...")
		#pragma comment(lib, "leda_md.lib")
        #ifdef _LINK_GeoW
			#pragma message("Autolinking GeoW (LIB) Opt:MD DLL ...")
			#pragma comment(lib, "libGeoW_md_dll.lib")
		#endif // _LINK_GeoW
        #ifdef _LINK_AGD
			#pragma message("Autolinking AGD (LIB) Opt:MD DLL ...")
			#pragma comment(lib, "libAGD_md_dll.lib")
		#endif // _LINK_AGD
	#endif // _DEBUG

#else // NOT _DLL

	#ifdef _MT

		#ifdef _DEBUG
			#pragma message("Autolinking LEDA (DLL) Opt:MTd ...")
			#pragma comment(lib, "leda_mtd.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:MTd DLL ...")
				#pragma comment(lib, "libGeoW_mtd_dll.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:MTd DLL ...")
				#pragma comment(lib, "libAGD_mtd_dll.lib")
			#endif // _LINK_AGD
		#else
			#pragma message("Autolinking LEDA (DLL) Opt:MT ...")
			#pragma comment(lib, "leda_mt.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:MT DLL ...")
				#pragma comment(lib, "libGeoW_mt_dll.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:MT DLL ...")
				#pragma comment(lib, "libAGD_mt_dll.lib")
			#endif // _LINK_AGD
		#endif // _DEBUG

	#else // NOT _MT

		#ifdef _DEBUG
			#pragma message("Autolinking LEDA (DLL) Opt:MLd ...")
			#pragma comment(lib, "leda_mld.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:MLd DLL ...")
				#pragma comment(lib, "libGeoW_mld_dll.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:MLd DLL ...")
				#pragma comment(lib, "libAGD_mld_dll.lib")
			#endif // _LINK_AGD
		#else
			#pragma message("Autolinking LEDA (DLL) Opt:ML ...")
			#pragma comment(lib, "leda_ml.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:ML DLL ...")
				#pragma comment(lib, "libGeoW_ml_dll.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:ML DLL ...")
				#pragma comment(lib, "libAGD_ml_dll.lib")
			#endif // _LINK_AGD
		#endif // _DEBUG

	#endif // _MT

#endif

#pragma comment(lib, "wsock32.lib")
