/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  mf_server.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_CHECKING_OFF

#include <LEDA/graph/graph.h>
#include <LEDA/graph/dimacs_graph.h>

#include <LEDA/graph/max_flow.h>

#include <LEDA/system/stream.h>
#include <LEDA/system/socket.h>

#include <fstream>

#ifdef LEDA_NAMESPACE
using namespace leda;
#endif

#if defined(LEDA_STD_IO_HEADERS)
using std::ifstream;
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
#endif

void usage()
{ cout << "\n";
  cout << "usage: mf_server port \n";
  cout << "\n";  
  exit(1);
}

string maxflow(ifstream& is)
{   
  graph G(3,2);

  node s,t;

  edge_array<int,graph> cap(G);
  edge_array<int,graph> flow(G);
 
  cap.use_edge_data(G);
  flow.use_edge_data(G);

  read_dimacs_mf(is,G,s,t,cap);

  float T = used_time();

  int f = MAX_FLOW(G,s,t,cap,flow);

  return string("time: %6.2f sec  f = %d", used_time(T), f);
}


int main(int argc, char** argv)
{
  if (argc < 2) usage();
  
  int port = atoi(argv[1]);
  

  leda_socket sock;
  sock.set_port(port);
  sock.set_limit(MAXINT);

  if (!sock.listen())
  { cerr << sock.get_error() << endl; 
    return 1;
  }

  for (;;) 
  { if (!sock.accept())
    { cerr << sock.get_error() << endl; 
      continue;
    }

    string s = "tmp.dimacs";
    sock.receive_file(s);    

    ifstream in;
    in.open(s);
    
    if (in)
    { sock.send_string(maxflow(in));
      in.close();
      //remove(s);
    }
    else
      sock.send_string(string("can't open temporary file %s",s));

    sock.disconnect();
  }
}

