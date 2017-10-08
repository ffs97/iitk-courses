/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  matrix_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/numbers/matrix.h>
#include <LEDA/numbers/matrix.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif

typedef matrix                   matrix_type;
//typedef real_matrix              matrix_type;
typedef matrix_type::value_type  value_type;
typedef matrix_type::vector_type vector_type;

inline bool is_exact_type(double*) { return false; }
inline bool is_exact_type(real*)   { return true; }
#define IS_EXACT(VT) is_exact_type((VT*)0)

int main()

{ 
  cout << "matrix_type: " << leda_tname((matrix_type*) 0) << endl;
  int d = read_int("dimension = ");

  matrix_type      A(d,d);
  matrix_type      I(d,d);
  value_type       det;


  for(int i=0;i<d;i++)
    for(int j=0;j<d;j++)
      A(i,j) = value_type(rand_int(-1000,1000))/100;

  float T = used_time();


  cout << "A.inv():  ";
  cout.flush();
  I =  A.inv();
  cout << string("%5.2f sec\n",used_time(T));

  cout << "A.det():  ";
  cout.flush();
  det = A.det();
  cout << string("%5.2f sec \n",used_time(T));
  cout << endl;

/*
  cout << "A*A.inv() = \n" << A*I << "\n";
  cout << endl;
*/

  while (Yes("A.solve(v) ? "))
  {
    vector_type v(d), x(d), y(d);

    for(int i=0;i<d;i++) v[i] = value_type(rand_int(-1000,1000))/100;
    
    used_time(T);
    x = A.solve(v);
    cout << string("time for solve:  %5.2f \n",used_time(T));
    cout << endl;

	if (IS_EXACT(value_type)) {
	    used_time(T);
		bool ok = A*x == v;
	    cout << string("time for check:  %5.2f \t",used_time(T));

		if (!ok)
			cout << "A*x != v ERROR\n";
		else
			cout << "A*x == v okay\n";
	}
	else {
		cout << "v   = " << v   << "\n";
		cout << "A*x = " << A*x << "\n";
	}
	cout << endl;
  }

  return 0;

}
