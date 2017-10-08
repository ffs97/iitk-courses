/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  time_server.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/socket.h>
#include <LEDA/system/file.h>
#include <LEDA/internal/std/time.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
#endif


#if defined(LEDA_STD_HEADERS)
#include <ctime>
#else
#include <time.h>
#endif

string get_current_time()
{ time_t clock; 
  time(&clock);
  tm* T = localtime(&clock); 
  return string("%2d:%02d:%02d", T->tm_hour,T->tm_min,T->tm_sec);
}


int main()
{
  leda_socket sock;

  sock.set_port(9666);

  if (!sock.listen())
  { cerr << sock.get_error() << endl; 
    return 1;
   }

  string fname = tmp_file_name();

  for (;;) {

    if (!sock.accept())
    { cerr << sock.get_error() << endl; 
      continue;
     }

/*
    string cmd;
    sock.receive_string(cmd);
    system(cmd + " > " + fname + " 2>&1");
    sock.send_file(fname);
    delete_file(fname);
*/
    system("xterm&");

    string time_str = get_current_time();
    sock.send_string(time_str);

    sock.disconnect();
  }

}

