// #define EQ_BY_COMPARE

#if __GNUC__ >= 4
struct abc;

namespace leda {
int compare(abc* const& x, abc* const& y);
}

#endif


#include <LEDA/core/list.h>

using std::ostream;
using std::cout;
using std::endl;

struct abc {
 char* str;
 abc(const char* s) : str(strdup(s)) {}
};

ostream& operator<<(ostream& os, const abc* x) 
{ return os << x->str; }


namespace leda {

int compare(abc* const& x, abc* const& y)
{ return strcmp(x->str,y->str); }

}


int main()
{
  leda::list<abc*> L;

  L.append(new abc("abcd"));
  L.append(new abc("xbcd"));
  L.append(new abc("abcd"));
  L.append(new abc("xbcy"));
  L.append(new abc("xbcd"));
  L.append(new abc("xbcy"));

  L.sort();

  cout << endl;
  L.print('\n');
  cout << endl;

  L.unique();

  cout << endl;
  L.print('\n');
  cout << endl;

  leda::std_memory.print_statistics();

  return 0;
}


