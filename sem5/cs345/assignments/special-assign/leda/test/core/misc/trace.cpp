/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  trace.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/list.h>

using namespace leda;



void produce_error() {
 list<int> L;
 L.pop();
 
 //int* p = (int*)0;
 //*p = 123456789;
}

void func2() {
  produce_error();
}

void func1() {
  func2();
}

void func0() {
  func1();
}

int main() { 
 //catch_system_errors();
 func0(); 
 return 0;
}
