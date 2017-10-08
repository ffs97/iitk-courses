/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  c_client.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/socket.h>
#include <LEDA/system/file.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ofstream;
#endif



void wait_error_handler(leda_socket& sock, string msg)
{ if (msg == "error")
  { sock.send_string("result");
    string s;
    sock.receive_string(s);
    error_handler(1,"AGD Server: " + s);
    exit(1);
   }
 }


int main(int argc, char *argv[])
{
  if (argc != 2 && argc != 4 && argc != 5) {
    cerr <<  endl;
    cerr << "usage: " << endl;
    cerr <<  endl;
    cerr <<  argv[0] << " host" << endl;
    cerr << "    prints list of available algorithms" << endl;
    cerr <<  endl;
    cerr <<  argv[0] << " host alg gml-infile  [gml-outfile]" << endl;
    cerr << "    runs algorithm alg for graph in gml-infile" << endl;
    cerr << "    writes output to cout or gml-outfile (if given)" << endl;
    cerr <<  endl;
    exit(1);
  }

  if (argc >= 4 && !is_file(argv[3])) 
        error_handler(1,string("file %s does not exist.",argv[3]));


  leda_socket sock(argv[1], 9660);
  if (!sock.connect())
  { cerr << sock.get_error() << endl; 
    exit(1);
   }

  sock.set_error_handler(wait_error_handler);

  string s;

  if (argc == 2) 
  { 
    // get list of algorithms

    sock.send_string("c_client");
    sock.wait("ok");
    sock.send_string("list");    // send "list" command
    sock.wait("ok");
    sock.send_string("result");  // send "result" command
    sock.receive(cout);          // receive and show list
    return 0;
   }


// here: argc > 2   
//
// argv[2]: algorithm 
// argv[3]: gml input file
// argv[4]: gml output file (optional)

  sock.send_string("c_client");
  sock.wait("ok");

  sock.send_string("layout");
  sock.wait("ok");

  sock.send_string(argv[2]);        // send name of algorithm 
  sock.wait("ok");

  sock.send_string("options");
  sock.wait("ok");

  sock.ostr() << "option1  value1" << endl;
  sock.ostr() << "option2  value2" << endl;
  sock.ostr() << "option3  value3" << endl;
  sock.ostr() << "option4  value4" << endl;
  sock.send();
  sock.wait("ok");

  sock.send_string("gml");       // send format
  sock.wait("ok");

  sock.send_file(argv[3]);       // send input file
  sock.wait("ok");             

  sock.send_string("result");   // get new layout and write it to cout
  if (argc == 4)
    sock.receive(cout);
  else
   { ofstream out(argv[4]);
     if (out.good()) 
       sock.receive(out);
     else
       error_handler(1,string("cannot write to file %s",argv[4]));
    }

  sock.disconnect();

  return 0;
}
