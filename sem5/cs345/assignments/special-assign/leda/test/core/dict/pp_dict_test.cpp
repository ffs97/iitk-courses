/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  pp_dict_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/array.h>
#include <LEDA/core/pp_dictionary.h>
#include <LEDA/system/stream.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::ostream;
using std::istream;
using std::ostringstream;
using std::istringstream;
using std::cout;
using std::cerr;
using std::endl;
#endif


#define LEDA_TEST_START int leda_test_res=0

#define LEDA_TEST(b) if (!(b)) { ++leda_test_res; \
cerr << "ERROR: Test " << #b << " failed." << endl; } \
else

#define LEDA_IO_TEST(datao,datai) { \
ostringstream OS; OS<<datao<<'\0'; \
istringstream IS(OS.str()); IS>>datai; \
if (datao != datai) { ++leda_test_res; \
cerr<<"ERROR in IO of "<<#datao<<" "<<#datai<<" : "\
    <<OS.str()<<" failed."<<endl;\
}}
   
#define LEDA_TEST_END return leda_test_res


typedef pp_dictionary<int,int> PPDICT;
typedef array<PPDICT>          PPDICTARRAY;

const int size = 10;

void print(ostream& os, const PPDICT& D)
{ pp_dic_item ppit;
  forall_items(ppit,D)
    os << "[" << D.key(ppit) << "," << D.inf(ppit) << "] ";
}

/* in the even rounds i we add entries with info i
   in the odd rounds i we delete entries mod i/2+1 */

int main()
{
  LEDA_TEST_START;
  PPDICTARRAY A(size);
  for (int i=0; i<size; ++i) {
    cout << "Round " << i << " ";
    if (i) A[i]=A[i-1]; 
    // transport persistent dict to next slot
    PPDICT& D = A[i];
    pp_dic_item ppit;
    for (int j=0; j<size; ++j) {
      if ( i%2==0 ) D.insert(j,i);
      if ( (ppit=D.lookup(j)) && (i%2!=0) && (j%(i/2+1)==0) ) 
        D.del_item(ppit);
    }
    print(cout,D);
    cout << endl;
  }
  for (int l=0; l< (10*size); ++l) {
    int a = rand_int(0,size-1);
    PPDICT& D = A[a];
    pp_dic_item ppit = D.lookup(a);
    if ( a%2 == 0 ) {
      LEDA_TEST(ppit && D.inf(ppit)==a);
    } else { // a%2 != 0
      LEDA_TEST(ppit || (a%(a/2+1)==0));
    }
  }
  LEDA_TEST( A[0].size()==size && A[1].size()==0 );
  LEDA_TEST( A[1].empty() );
  PPDICT& D = A[3];
  LEDA_TEST( !D.lookup(2) );
  LEDA_TEST( D.key(D.locate_succ(2)) == 3 );
  LEDA_TEST( D.key(D.locate_pred(2)) == 1 );
  PPDICT& DD = A[size-1];
  LEDA_TEST( DD.inf(DD.change_inf(DD.first_item(),111)) == 111 );
  LEDA_TEST( (DD.clear(),DD.empty()) );
  LEDA_TEST_END;
}

