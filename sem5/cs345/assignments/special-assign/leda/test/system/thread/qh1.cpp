#include <LEDA/geo/plane_alg.h>
#include <LEDA/geo/rat_point.h>
#include <LEDA/core/array.h>
#include <LEDA/numbers/fp.h>

using namespace leda;

using std::cout;
using std::cin;
using std::endl;
using std::flush;

class rp_smaller {
public:
bool operator()(const rat_point& p, const rat_point& q) const
{ return rat_point::cmp_xy(p,q) < 0; }
};

class rp_smaller_F {
public:
 bool operator()(const rat_point& a, const rat_point& b) const
 { double d = a.XD()*b.WD() - b.XD()*a.WD();
   if (d == 0) d = a.YD()*b.WD() - b.YD()*a.WD();
   return d < 0;
 }
};


class rp_greater: public rp_smaller {
public:
bool operator()(const rat_point& p, const rat_point& q) const
{ return rp_smaller::operator()(q,p); }
};

class rp_greater_F: public rp_smaller_F {
public:
bool operator()(const rat_point& p, const rat_point& q) const
{ return rp_smaller_F::operator()(q,p); }
};


void quick_hull5(rat_point* l, rat_point* r, list<rat_point>& result, int delta)
{
  if (r <= l+1) return;

  rat_point a = *l;
  rat_point b = *r;

  if (r == l+2) 
  { //if (orientation(a,*(l+1),b) < 0) 
    result.append(*(l+1));
    return;
   }

  if (r <= l+delta) 
  {  // graham scan
    if (rat_point::cmp_xy(a,b) < 0)
       quicksort(l,r,rp_smaller(),rp_smaller_F());
    else
       quicksort(l,r,rp_greater(),rp_greater_F());

    rat_point* j = l+1;
    for(rat_point* p = l+2; p<=r; p++)
    { rat_point pp = *p;
      while(orientation(*(j-1),*j,pp) >= 0) j--;
      *++j = pp;
     }
    for(rat_point*p = l+1; p<j; p++) result.append(*p);
    return;
   }


  rat_vector vec = b-a;
  rat_point* k = l+1;
  rat_point c = *k;
  rat_point d = c+vec;

  for(rat_point* p=l+2; p<r; p++)
  {  int orient = orientation(c,d,*p);
     //if (orient > 0 || orient == 0 && side_of_halfspace(c,d,*p) > 0) 
     if (orient > 0 || orient == 0 && compare(c,*p) > 0) 
     { k = p; 
       c = *k; 
       d = c+vec; 
      } 
   }

  if (orientation(a,b,c) == 0) return; 


  swap(*(l+1),*k);
  rat_point* i = l+1;
  rat_point* j = r;

  rat_point* p = i+1;
  while (p < j)
  { if (orientation(a,c,*p) > 0)
      swap(*++i,*p++);
    else
      if (orientation(c,b,*p) > 0)
        swap(*--j,*p);
      else
        p++;
   }

  swap(*(l+1),*i);
  *--j = c;

  quick_hull5(l,i,result,delta);
  result.append(c);
  quick_hull5(j,r,result,delta);
}


list<rat_point> quick_hull5(const list<rat_point>& L, int d = 256)
{ int n = L.length();
  rat_point* A = new rat_point[n+2];

  list<rat_point> result;

  rat_point a = L.front();
  rat_point b = L.front();

  rat_point p;
  forall(p,L)
  { if (rat_point::cmp_xy(p,a) < 0) a = p;
    if (rat_point::cmp_xy(p,b) > 0) b = p;
   }

  rat_point* l = A+1;
  rat_point* r = A+n+1;

  A[0] = b;

  *l = a;
  *r = a;
  forall(p,L) 
  { if (identical(p,a) || identical(p,b)) continue;
    if (orientation(a,b,p) > 0) 
      *++l = p;
    else
      *--r = p;
   }
  *++l = b;

  result.append(A[1]);
  quick_hull5(A+1,l,result,d);
  result.append(*l);
  quick_hull5(l,A+n+1,result,d);
  delete[] A;
  return result;
}




int main()
{
  int n = read_int("n = ");

  list<rat_point>  L;
  string gen;

  while (gen == "")
  { gen  = read_string("generator d(isk)/s(quare)/c(ircle) : ");
    if (gen[0] == 'd')
       random_points_in_disc(n,10000,L);
    else if (gen[0] == 's')
       random_points_in_square(n,10000,L);
    else if (gen[0] == 'c')
       random_points_on_circle(n,10000,L);
    else gen = "";
  }


  cout << endl;

  list<rat_point> H;

  float T = used_time();

  cout << "quick_hull5      " << flush;
  H =  quick_hull5(L);
  cout << string("|C| = %d  time = %.2f",H.length(),used_time(T)) << endl;
  cout << endl;


  return 0;
}
