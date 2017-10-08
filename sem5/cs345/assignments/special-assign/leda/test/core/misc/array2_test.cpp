#include <LEDA/core/array2.h>
#include <LEDA/core/list.h>

using namespace leda;
using std::cout;
using std::endl;

int main()
{
   list<int> L;
   L.append(17);

   array2<list<int> > A(-1,10,-1,10);

   A.init(L);

   for(int i=-1; i<10; i++)
     for(int j=-1; j<10; j++)
          for(int k =0; k< i+j+2; k++) A(i,j).append(k);

   cout << endl;
   cout << "copy constructor" << endl;
   cout << endl;

   array2<list<int> > B(A);

   for(int i=-1; i<10; i++)
     for(int j=-1; j<10; j++)
     { int x=0;
       forall(x,B(i,j)) cout << x << " ";
       cout << endl; 
      }

   array2<list<int> > C(1,1);

   cout << endl;
   cout << "assignment operator" << endl;
   cout << endl;

   C = B;

   for(int i=-1; i<10; i++)
     for(int j=-1; j<10; j++)
     { int x=0;
       forall(x,C(i,j)) cout << x << " ";
       cout << endl; 
      }

   return 0;
}
