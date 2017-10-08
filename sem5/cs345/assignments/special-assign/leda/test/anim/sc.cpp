/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sc.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <algorithm>

#include <LEDA/anim/iterator.h>


using namespace leda;
using std::cout;
using std::endl;


unsigned cell<int>::count = 0;
unsigned v_iterator<int>::count = 0;


int main()
{     
/*
  cell<int>* A = new cell<int>[10];  
  cell<int>* B = new cell<int>[10];  

  v_iterator<int> a1 = A; 
  v_iterator<int> a2 = A+10; 
  v_iterator<int> b1 = B; 
  v_iterator<int> b2 = B+10; 
*/

  v_vector<int> A(10);
  v_vector<int> B(10);

  v_iterator<int> a1 = A.begin(); 
  v_iterator<int> a2 = A.end(); 
  v_iterator<int> b1 = B.begin(); 
  v_iterator<int> b2 = B.end(); 


  int i;
  for (i = 0; i < 10; i++) B[i] = i;
    
  std::copy(b1,b2,a1);    
  for(i=0; i<10; i++) cout << A[i] << " ";
  cout << endl;

  std::random_shuffle(a1,a2);    
  for(i=0; i<10; i++) cout << A[i] << " ";
  cout << endl;

  std::sort(a1,a2);         
  for(i=0; i<10; i++) cout << A[i] << " ";
  cout << endl;
  
  return 0;
}
