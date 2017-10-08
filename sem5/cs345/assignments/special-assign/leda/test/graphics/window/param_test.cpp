/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  param_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/core/param_handler.h>
#include <LEDA/graphics/param_panel.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


main(int argc, char* argv[]) 
{
  param_handler P(argc,argv,".testparamfile1");
  param_handler Q(argc,argv,".testparamfile2",false);

  enum {Morning=10,Noon,Evening};
  P.add_parameter("Input File:-f:string:I-File");
  P.add_parameter("Rounds:-nr:int:10");
  P.add_parameter("Start Value:-s:double:0.1");
  P.add_parameter("Read from file:-r:bool:false");
  P.add_parameter("Daytime:-dt:enum(Morning=10,Noon,Evening):Morning");

  Q.add_parameter("Titel:-t:string:two handler test");
  Q.add_parameter("Number:-no:int:11");
  Q.add_parameter("Eps:-e:double:0.001");
  Q.add_parameter("Use all:-a:bool:true");
  Q.add_parameter("Search mode:-sm:enum(DFS,BFS,BESTFIRST):DFS");

  param_handler::init_all();

  cout << endl;
  cout << P << endl; 
  cout << Q << endl;

  string inputfile;
  int number_of_rounds;
  bool doit;
  double start;
  int dtime;

  P.get_parameter("-f",inputfile);
  P.get_parameter("Rounds",number_of_rounds);
  P.get_parameter("Read from file",doit);
  P.get_parameter("-s",start);
  P.get_parameter("-dt",dtime);

  cout << "\nValues retrieved with get_parameter:\n";
  cout << "-f  " <<  inputfile << endl;
  cout << "-nr " <<  number_of_rounds << endl;
  cout << "-s  " <<  start << endl;
  cout << "-r  " <<  doit << endl;
  cout << "-dt " <<  dtime << "/";
  switch (dtime) {
  case Morning: cout << "Good Morning" << endl; break;
  case Noon: cout << "I'm hungry" << endl; break;
  case Evening: cout << "Good Night" << endl; break;
  default: cerr << "Error retrieving enum ..." << endl; exit(-1);
  }

  Q.set_parameter("Titel","two handler test - changed !");
  Q.set_parameter("-no",4711);
  Q.set_parameter("Eps",1e10);
  Q.set_parameter("-a",false);

  cout << "\n\nValues of second object are changed with set_parameter\n\n";
  cout << P << endl; 
  cout << Q << endl;

  param_panel PP(P);
  param_panel QP(Q);

  PP.open();
  QP.open();
  cout << "\n\nValues after editing\n\n";
  cout << P << endl; 
  cout << Q << endl;
  
}

