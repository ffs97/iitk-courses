/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  cuckoo_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/impl/cuckoo_hash.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int main()
{
  int size = 20;

  cuckoo_hash D(20);
  
  int i;
  
  for(i = 1; i < 20; i++) D.insert((GenPtr)i,0);

  for(i = 1; i < 20; i++) 
    if (D.lookup((GenPtr)i))
      cout << i << " in D" << endl;
  
  cout << "Size " << D.size() << endl;

  for(i = 0; i < 5; i++)
  {
    int k = rand_int(1,size-1);
    D.del((GenPtr)k);
    cout << "del " << (int)(GenPtr) k << endl;
  }
 
  cuckoo_hash D1(D), D2 = D;
  
  
  cout << "Size " << D.size() << endl;

  for(i = 1; i < 20; i++) 
    if (D.lookup((GenPtr)i))
      cout << (int)(GenPtr)i << " in D" << endl;
    else
      cout << (int)(GenPtr)i << " not in D" << endl;
  
  cout << "Size " << D1.size() << endl;

  for(i = 1; i < 20; i++) 
    if (D1.lookup((GenPtr)i))
      cout << (int)(GenPtr)i << " in D1" << endl;
    else
      cout << (int)(GenPtr)i << " not in D1" << endl;
 
 cout << "Size " << D2.size() << endl;

  for(i = 1; i < 20; i++) 
    if (D2.lookup((GenPtr)i))
      cout << (int)(GenPtr)i << " in D2" << endl;
    else
      cout << (int)(GenPtr)i << " not in D2" << endl;
 

  cout << "Clear D" << endl;
 
 
  D.clear();
  
  cout << "Size " << D.size() << endl;

  D.size();
  
  for(i = 1; i < 20; i++) 
    if (D.lookup((GenPtr)i))
      cout << (int)(GenPtr)i << " in D" << endl;
    else
      cout << (int)(GenPtr)i << " not in D" << endl;
 
  
  return 0;
}
