/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  pair.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#if __GNUC__ >= 4

class Pair;
namespace leda {
int compare(const Pair& p, const Pair& q);
int compare(Pair* const & p, Pair* const & q);
}

#endif


#include <LEDA/core/list.h>

using std::ostream;
using std::istream;
using std::cout;
using std::endl;

class Pair {

 double  x;
 double  y;

public:

Pair() { x = y = 0; }
Pair(const Pair& p) { x = p.x; y = p.y; }

double xc() const { return x; }
double yc() const { return y; }

friend istream& operator>>(istream& is, Pair& p)
{ return is >> p.x >> p.y; }

friend ostream& operator<<(ostream& os, const Pair& p)
{ return os << p.x << " " << p.y; }

};


namespace leda {

int compare(const Pair& p, const Pair& q)
{  if (p.xc() < q.xc()) return -1; 
   if (p.xc() > q.xc()) return  1; 
   if (p.yc() < q.yc()) return -1; 
   if (p.yc() > q.yc()) return  1; 
   return 0;  
 }

int compare(Pair* const & p, Pair* const & q) { return compare(*p,*q); }

};


using namespace leda;

int main()
{ 
  list<Pair> L;
  L.read("list of Pairs: ");
  L.sort();
  Pair p;
  forall(p,L) cout << p << endl;
  cout << endl;

  // do it with pointers

  list<Pair*> L1;
  forall(p,L) L1.append(new Pair(p));
  L1.sort();
  Pair* q;
  forall(q,L1) 
  { cout << *q << endl;
    delete q;
   }
  cout << endl;

  return 0;
}
