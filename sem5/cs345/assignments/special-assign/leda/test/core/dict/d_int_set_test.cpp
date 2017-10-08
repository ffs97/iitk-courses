/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d_int_set_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/core/int_set.h>
#include <LEDA/core/d_int_set.h>

#include <LEDA/core/list.h>
#include <LEDA/system/regtest.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int main()
{
  LEDA_TEST_START(d_int_set, simple case call test of d_int_sets)
    /* some construction test */

  list<int> Li,Lo;  
  Li.append(-11);
  Li.append(23);
  Li.append(7);
  Li.append(31);
  Li.append(32);
  Li.append(33);
  Li.append(87);
  Li.append(-63);
  Li.append(-64);
  Li.append(-65);
  Li.append(-66);

  d_int_set I,J;
  int i;
  forall(i,Li) I.insert(i);
  LEDA_TEST(I.size()==Li.size(),size)

  LEDA_IO_TEST(I,J)

  Li.sort(); 

  LEDA_TEST(I.min() == Li.front(), min-test)
  LEDA_TEST(I.max() == Li.back(), max-test)
  LEDA_TEST(I.member(-66), member-test)
  LEDA_TEST(!I.member(73), non-member-test)

  for (int l = 0; l<100; l++) {
    i=I.choose();
    if (!I.member(i)) {
      TRACE(i);TRACE(" not in ");TRACEN(I);
    }
  }
 
  J.del(-64);
  LEDA_TEST(I.member(-64) && !J.member(-64), deletion test);
  i=J.choose(); J.del(i);
  LEDA_TEST(I.member(i) && !J.member(i), choose1 test);
  i=J.choose(); J.del(i);
  LEDA_TEST(I.member(i) && !J.member(i), choose2 test);
  LEDA_TEST(J <= I && J < I && I >= J && I > J && I != J, bin-ops1);
  d_int_set K = I-J;
  LEDA_TEST(I== K+J, diff and join)
  K.insert(0);
  K.insert(3);
  LEDA_TEST((K&I) == (I-J), intersect)
  LEDA_TEST((K%I) == ((K-I)|(I-K)), symdiff and or)
  J=I;
  LEDA_TEST((I&=I,I==J),&=op)
  LEDA_TEST((I|=I,I==J),|=op)
  LEDA_TEST((I-=I,I.empty()),-=op)
  I=J;
  LEDA_TEST((I%=I,I.empty()),%=op)

  I=J;
  I.get_element_list(Lo); 
  bool ioloop=true;
  list_item jit = Lo.first();
  forall(i,Li) {
    ioloop = (i == Lo[jit]);
    jit = Lo.succ(jit);
  }
  LEDA_TEST(ioloop, input=output list);

  LEDA_TEST_END

  return 0;
}

