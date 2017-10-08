/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  autolink_static.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef _LINK_GeoW
	#ifndef _LINK_AGD
		#pragma message("Autolinking LEDA (LIB) ... (" __FILE__ ")")
	#else
		#pragma message("Autolinking LEDA (LIB) & AGD (LIB) ... (" __FILE__ ")")
	#endif
#else
	#ifndef _LINK_AGD
		#pragma message("Autolinking LEDA (LIB) & GeoW (LIB) ... (" __FILE__ ")")
	#else
		#pragma message("Autolinking LEDA (LIB), GeoW (LIB) & AGD (LIB) ... (" __FILE__ ")")
	#endif
#endif

#ifdef LEDA_DLL
#undef LEDA_DLL
#endif

#ifdef LEDA_LINK_NoOptions

	#pragma message("Autolinking LEDA (LIB) (no options) ...")
	#pragma comment(lib, "libL.lib")
	#pragma comment(lib, "libG.lib")
	#pragma comment(lib, "libP.lib")
	#pragma comment(lib, "libW.lib")
	#ifdef _LINK_GeoW
		#pragma message("Autolinking GeoW (LIB) (no options) LIB ...")
		#pragma comment(lib, "libGeoW.lib")
	#endif // _LINK_GeoW
	#ifdef _LINK_AGD
		#pragma message("Autolinking AGD (LIB) (no options) LIB ...")
		#pragma comment(lib, "libAGD_lib.lib")
	#endif // _LINK_AGD

#elif defined(_DLL)

	#ifndef _MT
		#error Unknown option combination
	#endif

	#ifdef _DEBUG
		#pragma message("Autolinking LEDA (LIB) Opt:MDd ...")
		#pragma comment(lib, "libL_mdd.lib")
		#pragma comment(lib, "libG_mdd.lib")
		#pragma comment(lib, "libP_mdd.lib")
		#pragma comment(lib, "libW_mdd.lib")
        #ifdef _LINK_GeoW
			#pragma message("Autolinking GeoW (LIB) Opt:MDd LIB ...")
			#pragma comment(lib, "libGeoW_mdd_lib.lib")
		#endif // _LINK_GeoW
        #ifdef _LINK_AGD
			#pragma message("Autolinking AGD (LIB) Opt:MDd LIB ...")
			#pragma comment(lib, "libAGD_mdd_lib.lib")
		#endif // _LINK_AGD
	#else
		#pragma message("Autolinking LEDA (LIB) Opt:MD ...")
		#pragma comment(lib, "libL_md.lib")
		#pragma comment(lib, "libG_md.lib")
		#pragma comment(lib, "libP_md.lib")
		#pragma comment(lib, "libW_md.lib")
        #ifdef _LINK_GeoW
			#pragma message("Autolinking GeoW (LIB) Opt:MD LIB ...")
			#pragma comment(lib, "libGeoW_md_lib.lib")
		#endif // _LINK_GeoW
        #ifdef _LINK_AGD
			#pragma message("Autolinking AGD (LIB) Opt:MD LIB ...")
			#pragma comment(lib, "libAGD_md_lib.lib")
		#endif // _LINK_AGD
	#endif // _DEBUG

#else // NOT _DLL

	#ifdef _MT

		#ifdef _DEBUG
			#pragma message("Autolinking LEDA (LIB) Opt:MTd ...")
			#pragma comment(lib, "libL_mtd.lib")
			#pragma comment(lib, "libG_mtd.lib")
			#pragma comment(lib, "libP_mtd.lib")
			#pragma comment(lib, "libW_mtd.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:MTd LIB ...")
				#pragma comment(lib, "libGeoW_mtd_lib.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:MTd LIB ...")
				#pragma comment(lib, "libAGD_mtd_lib.lib")
			#endif // _LINK_AGD
		#else
			#pragma message("Autolinking LEDA (LIB) Opt:MT ...")
			#pragma comment(lib, "libL_mt.lib")
			#pragma comment(lib, "libG_mt.lib")
			#pragma comment(lib, "libP_mt.lib")
			#pragma comment(lib, "libW_mt.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:MT LIB ...")
				#pragma comment(lib, "libGeoW_mt_lib.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:MT LIB ...")
				#pragma comment(lib, "libAGD_mt_lib.lib")
			#endif // _LINK_AGD
		#endif // _DEBUG

	#else // NOT _MT

		#ifdef _DEBUG
			#pragma message("Autolinking LEDA (LIB) Opt:MLd ...")
			#pragma comment(lib, "libL_mld.lib")
			#pragma comment(lib, "libG_mld.lib")
			#pragma comment(lib, "libP_mld.lib")
			#pragma comment(lib, "libW_mld.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:MLd LIB ...")
				#pragma comment(lib, "libGeoW_mld_lib.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:MLd LIB ...")
				#pragma comment(lib, "libAGD_mld_lib.lib")
			#endif // _LINK_AGD
		#else
			#pragma message("Autolinking LEDA (LIB) Opt:ML ...")
			#pragma comment(lib, "libL_ml.lib")
			#pragma comment(lib, "libG_ml.lib")
			#pragma comment(lib, "libP_ml.lib")
			#pragma comment(lib, "libW_ml.lib")
            #ifdef _LINK_GeoW
				#pragma message("Autolinking GeoW (LIB) Opt:ML LIB ...")
				#pragma comment(lib, "libGeoW_ml_lib.lib")
			#endif // _LINK_GeoW
            #ifdef _LINK_AGD
				#pragma message("Autolinking AGD (LIB) Opt:ML LIB ...")
				#pragma comment(lib, "libAGD_ml_lib.lib")
			#endif // _LINK_AGD
		#endif // _DEBUG

	#endif // _MT

#endif // _DLL

#pragma comment(lib, "wsock32.lib")
