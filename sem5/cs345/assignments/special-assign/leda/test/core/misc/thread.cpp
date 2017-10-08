/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  thread.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/list.h>
#include <stdio.h>

#include <windows.h>

using namespace leda;

unsigned long id1 = 0;
unsigned long id2 = 0;
unsigned long id3 = 0;
unsigned long id4 = 0;
unsigned long id5 = 0;

static list<int> L;


DWORD WINAPI thread_func(LPVOID ptr)
{ unsigned long i = GetCurrentThreadId();
  L.append(int(i));
  leda_wait(10);
  *(unsigned long*)ptr = i;
  return 0;
}


main()
{

  unsigned long id = 1;
  CreateThread(NULL,0,thread_func,&id1,0,&id);
  CreateThread(NULL,0,thread_func,&id2,0,&id);
  CreateThread(NULL,0,thread_func,&id3,0,&id);
  CreateThread(NULL,0,thread_func,&id4,0,&id);
  CreateThread(NULL,0,thread_func,&id5,0,&id);

  while (id1 == 0 || id2 == 0 || id3 == 0 || id4 == 0 || id5 == 0);

  printf("id1 = %d\n",id1);
  printf("id2 = %d\n",id2);
  printf("id3 = %d\n",id3);
  printf("id4 = %d\n",id4);
  printf("id5 = %d\n",id5);

  L.print();

  return 0;
}

