/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  tclient.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/socket.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
#endif


int main(int argc, char** argv)
{
  if (argc < 3)
  { cerr << "usage: tclient host port" << endl;
    return 1;
   }

  for(;;)
  {
    leda_socket sock;
    sock.set_host(argv[1]);
    sock.set_port(atoi(argv[2]));

    // connect

    if (!sock.connect())
    { cerr << sock.get_error() << endl;
      return 1;
     }

    // receive string (time) and write it to cout 
  
    sock.receive(cout);
    leda_wait(10);
  }

  return 0;
}

