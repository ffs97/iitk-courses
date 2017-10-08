/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  set_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/set.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif



int main()
{

set<string> S;
set<string> S1;
set<string> S2;

string s;

cout << "S1 = ";
while (cin >> s) 
{ if (s == ".") break;
  S1.insert(s);
 }

cout << "S2 = ";
while (cin >> s) 
{ if (s == ".") break;
  S2.insert(s);
 }


S = S1 + S2;
cout << "S1 + S2  = ";
forall(s,S) cout << s << " ";
cout << endl;

S = S1 - S2;
cout << "S1 - S2 = ";
forall(s,S) cout << s << " ";
cout << endl;

S = S2 - S1;
cout << "S2 - S1 = ";
forall(s,S) cout << s << " ";
cout << endl;


S = S1 & S2;
cout << "S1 & S2 = ";
forall(s,S) cout << s << " ";
cout << endl;

S = S1 % S2;
cout << "S1 % S2 = ";
forall(s,S) cout << s << " ";
cout << endl;


cout << "S1 == S2 = " << (S1 == S2) << endl;;

cout << "S1 != S2 = " << (S1 != S2) << endl;;

cout << "S1 <= S2 = " << (S1 <= S2) << endl;;

cout << "S1 >= S2 = " << (S1 >= S2) << endl;;

cout << "S1 < S2 = " << (S1 < S2) << endl;;

cout << "S1 > S2 = " << (S1 > S2) << endl;;




return 0;

}

