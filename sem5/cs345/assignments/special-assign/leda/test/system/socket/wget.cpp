#include <LEDA/system/file.h>
#include <LEDA/system/http.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
#endif


int main(int argc, char** argv)
{
  if (argc < 2)
  { cerr << "usage: wget url" << endl;
    return 1;
   }


/*
  list<string> header;
  if (wget(argv[1],header,cout,10) == 0)
  { string status = header.front();
    cerr << status << endl;
    return 1;
   }
*/

  http H;

  if (!H.connect(argv[1]))
  { cerr << H.get_error() << endl;
    cerr << endl;
    list<string> header = H.get_header();
    string s;
    forall(s,header) cerr << s << endl;
    cerr << endl;
    return 1;
   }


  string line;
  while (H.get_line(line)) cout << line.utf_to_iso8859() << endl;

  return 0;
}

