/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sortseq_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/tuple.h>
#include <LEDA/core/sortseq.h>

#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif



inline void report(string s)
{
cout << s <<"\n";
cout.flush();
}

int convert(int a, int y){ return y;}

double convert(double a, int y){ return ((double) y); }

four_tuple<int,int,int,int> convert(four_tuple<int,int,int,int> a,int y)
       { return four_tuple<int,int,int,int>(0,0,0,y);}


template <class K>
void complete_test(K a, const char* s)
{ cout << "\n\ntest with type " << s << "\n\n";
  sortseq<K,int> S;
  sortseq<K,int> *A[1000];
  report("decls worked");

{
  sortseq<K,int> S1;
  for (int i = 0; i <1000; i++)
  { int y = rand_int(0,10000);
    K x = convert(a,y);
    while (S.lookup(x) != nil)  
    { assert(S1.lookup(x) != nil);
      y = rand_int(0,10000);
      x = convert(a,y);
    }
 
    S.insert(x,0);
    S1.insert(x,0);
    assert(S1.lookup(x) != nil);
    assert(compare(S1.key(S1.lookup(x)),x) == 0);
    if (S.lookup(x) == nil || compare( S.key(S.lookup(x)),x) != 0 )
       error_handler(1, "insert or lookup");
    A[i] = new sortseq<K,int>;
    A[i]->insert(x,0);
  }
}

  S.check_data_structure("after inserts");
 

  report("insertion worked");


{
  for (int i = 999; i >= 1; i--)
  { int j = rand_int(0,i);
    leda_swap(A[j],A[i]);
    j = rand_int(0,i-1);
    leda_swap(A[j],A[i-1]);
    A[i-1]->merge(* A[i]);
  }
}

  report("merge ran to completion");

  /* we compare S and A[0] element by element */

#if !defined(_MSC_VER)
  if (S != (*(A[0]))) error_handler(1,"error in merge");
#endif

  report("merge correct");


  seq_item finger;


{
  for(int i = 0; i < 10000; i++)
  { int y = rand_int(0,10000);
    K x = convert(a,y);
    finger = S.locate_pred(x);
    if (finger == nil) finger = S.min_item();
    y = rand_int(0,10000);
    x = convert(a,y);

    if (S.lookup(x) != S.finger_lookup(finger,x))
       error_handler(1,"error in lookup");
    if (S.locate_pred(x) != S.finger_locate_pred(finger,x))
       error_handler(1,"error in locate_pred");
    if (S.locate_succ(x) != S.finger_locate_succ(finger,x)) 
       error_handler(1,"error in locate_succ");
    if (S.lookup(x) != S.finger_lookup(x))
            error_handler(1,"error in lookup: dove-tailed");    
    if (S.locate_pred(x) != S.finger_locate_pred(x))
       error_handler(1,"error in locate_pred: dove-tailed");
    if (S.locate_succ(x) != S.finger_locate_succ(x)) 
       error_handler(1,"error in locate_succ: dove-tailed");
   }
}

   report("lookups worked");


{
   for (int i = 0; i < 100; i++)
   { int y = rand_int(0,10000);
     K x = convert(a,y);
     finger = S.locate_pred(x);
     if (finger == nil) finger = S.min_item();

     sortseq<K,int> S1;
     sortseq<K,int> S2;
     S.split(finger,S1,S2,LEDA::after);

     S1.conc(S2,LEDA::after);
     S.conc(S1,LEDA::after);
   }
}


#if !defined(_MSC_VER)
   if (S != (*(A[0]))) error_handler(1,"error in conc or split");
#endif


   report("split and conc worked");


finger = S.min_item();
int i;
for(i = 0; i <= 100; i++) finger = S.succ(finger);
seq_item finger2 = finger;
for(i = 0; i <= 100; i++) finger2 = S.succ(finger2);

seq_item finger_pred = S.pred(finger);

sortseq<K,int> S1;
sortseq<K,int> S2;
sortseq<K,int> S3;


S.delete_subsequence(finger,finger2,S2);
S.split_at_item(finger_pred,S1,S3,LEDA::after);
S.conc(S1,LEDA::after);
S.conc(S2,LEDA::after);
S.conc(S3,LEDA::after);


#if !defined(_MSC_VER)
if (S != (*(A[0]))) error_handler(1,"error in delete subsequence");
#endif


}

int main(){ 

int a1 = 0; 
double a2= 0; 
four_tuple<int,int,int,int> a3(0,0,0,0);

complete_test(a1,"int");
complete_test(a2,"double");
#if !defined(__aCC__)
complete_test(a3,"four_tuple");
#endif

return 0;
}


 
