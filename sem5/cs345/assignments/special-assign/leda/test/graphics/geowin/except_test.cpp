/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  except_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geowin.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif



int main()
{
 GeoWin gw;
 
 set_error_handler(exception_error_handler);
 
 list<polygon> LP;
 
 geo_scene sc0 = gw.new_scene(LP);

 try {
  gw.edit(sc0);
 }
 catch (leda_exception e){
   cout << e.get_msg() << endl; 
   cout << e.get_numr() << endl;
 }

 return 0; 
}
