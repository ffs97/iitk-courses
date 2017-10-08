/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  win_error.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/window.h>

using namespace leda;

void func1();
void func2();
void func3();

void produce_error()
{ list<int> L;
  L.pop();
}

void func1() { func2(); }
void func2() { func3(); }
void func3() { produce_error(); }


int main()
{ 
  window W(600,400);
  W.display(window::center,window::center);
  func1();
  return W.read_mouse();
}
