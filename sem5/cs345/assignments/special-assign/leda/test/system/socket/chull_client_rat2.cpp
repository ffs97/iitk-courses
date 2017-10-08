/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  chull_client_rat2.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/rat_window.h>
#include <LEDA/socket.h>
#include <LEDA/file.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#endif


#if defined(WINMAIN)

int main() {
  int    argc;
  char** argv;
  get_command_line(argc,argv);

#else

int main(int argc, char** argv)
{
#endif

  leda_socket sock;
  window win(600,400,"Convex hull client");

  if (argc < 3)
  { cerr << "usage: tclient host port" << endl;
    return 1;
   }

  sock.set_host(argv[1]);
  sock.set_port(atoi(argv[2]));
  
  win.init(0,400,0);
  win.display();
  rat_point p;
  list<rat_point> pts, hull;

  while(win >> p)
  {
    win << p;
    pts.append(p);
    hull.clear(); 

    if (!sock.connect())
    { cerr << sock.get_error() << endl;
      return 1;
     } 

    socket_send_object(pts.size(),sock);

    forall(p,pts) socket_send_object(p,sock);
   
    int i,numb;
    socket_receive_object(numb,sock);
  
    for(i=0;i<numb;i++) {
      socket_receive_object(p,sock);
      hull.append(p);
    }
    
    rat_polygon pol(hull);   
    win.clear();    
    forall(p,pts) win << p;
    win << pol;

    sock.disconnect();
  }
  return 0;
}

