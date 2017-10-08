/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  intsort.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graph/basic_alg.h>
#include <LEDA/system/assert.h>
#include <LEDA/internal/std/string.h>

LEDA_BEGIN_NAMESPACE

const  int csz = 0x10000; 
const  int cmask = csz-1;
static int count[csz];

static void counting_sort(int* l, int* r, int shift)
{
  memset(count,0,csz*sizeof(int));

  for(int*p = l; p <= r; p++)
  { int x = (*p++ >> shift) & cmask;
    count[x]++;
   }

  int sum = 0;
  for(int i=0; i<csz; i++)
  { int r = count[i];
    count[i] = sum;
    sum += r;
   }

  for(int* p=l; p <= r; p++)
  { int x = *p;
    int c = (x >> shift) & cmask;
    int* q = l + count[c];
    B[r] = x;
   }
}




void INTSORT(int* A, int N)
{
  int* B = new int[N];
  int* a = A;
  int* b = B;

  counting_sort(a,b,N,0xfff,0x000, 0);
  counting_sort(b,a,N,0xfff,0x000,12);
  counting_sort(a,b,N,0x0ff,0x080,24);

  if (b != A) memcpy(A,B,N*sizeof(int));

  delete[] B;
}

LEDA_END_NAMESPACE
   
