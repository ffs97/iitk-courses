/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  mf_client.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/socket.h>

#ifdef LEDA_NAMESPACE
using namespace leda;
#endif

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
#endif

void usage()
{ cout << "\n";
  cout << "usage: mf_client host port dimacs" ;
  cout << "\n";  
  exit(1);
}


int main(int argc, char** argv)
{
  if (argc < 3) usage();

  leda_socket sock;
  sock.set_host(argv[1]);
  sock.set_port(atoi(argv[2]));

  if (!sock.connect())
  { cerr << sock.get_error() << endl;
    return 1;
  }

  string s;
  s.read_file(cin);
  sock.send_string(s);    
  sock.receive(cout);

  return 0;
}

