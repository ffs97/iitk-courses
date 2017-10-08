/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  regtest.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_REGTEST_H
#define LEDA_REGTEST_H

#include <LEDA/system/stream.h>

LEDA_BEGIN_NAMESPACE

#define LEDA_TEST_START(testid,testtask) \
file_ostream F_log(#testid".log"); \
file_ostream F_err(#testid".err"); \
bool leda_test_success=true;       \
string testproject(#testid);        \
F_log << "Name: "#testid" test\n"; \
F_log << "Task: "#testtask"\n"; \
F_err << "Name: "#testid" test\n"; \
F_err << "Task: "#testtask"\n"; 


#define LEDA_TEST(cond,fstr)   \
if (cond) {        \
  F_log << #fstr" ok\n"; F_log.flush();  \
}else{ \
  leda_test_success = false; \
  F_log << #fstr" error\n"; F_log.flush();  \
  F_err << "Error in "#fstr":\n  Test failed: "#cond; \
  F_err << "\n  In file " << __FILE__ << " at line " << __LINE__ << "\n"; \
  F_err.flush();\
}

#define LEDA_IO_TEST(datao,datai) { \
file_ostream FO(testproject+".io"); FO<<datao; FO.close(); \
file_istream FI(testproject+".io"); FI>>datai; FI.close(); \
if (datao == datai) { \
  F_log << "OutputInput-test ok\n"; \
} else { \
  F_log << "OutputInput-test error\n"; \
  F_err << "Error in IO-operators, output file: " << testproject << ".io\n"; \
}}

#define LEDA_TEST_END \
if (leda_test_success) { \
  F_log << testproject << "-test finished successfully!\n"; \
  F_err << testproject << "-test finished successfully!\n"; \
} else { \
  F_log << testproject << "-test finished with errors!\n"; \
  F_err << testproject << "-test finished with errors!\n"; \
} F_log.close(); F_err.close();


#define TRACE(t) cout << t
#define TRACEN(t) cout << t << endl
#define TRACEV(t) cout << #t << " = " << (t)  << endl;


LEDA_END_NAMESPACE

#endif 


