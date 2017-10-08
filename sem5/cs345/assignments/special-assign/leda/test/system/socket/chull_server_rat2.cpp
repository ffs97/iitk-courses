/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  chull_server_rat2.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/rat_geo_alg.h>
#include <LEDA/socket.h>
#include <iostream.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif



int main()
{
  leda_socket sock;

  sock.set_port(9999);
  
  sock.set_limit(50);

  if (!sock.listen())
  { cerr << sock.get_error() << endl; 
    return 1;
   }
  
  list<rat_point> inp;
  rat_point pin;

  for (;;) {

    if (!sock.accept())
    { cerr << sock.get_error() << endl; 
      continue;
     }

    int numb,i;
    socket_receive_object(numb, sock);

    inp.clear();
    
    for(i=0;i<numb;i++){
      socket_receive_object(pin, sock);
      inp.append(pin);
    }
    
    cout << inp << "\n";
    
    list<rat_point> hull_points = CONVEX_HULL(inp);
    
    // send the hull back ...
    socket_send_object(hull_points.size(), sock);
    forall(pin,hull_points) socket_send_object(pin, sock);   
    
   
    sock.disconnect();
  }
}

