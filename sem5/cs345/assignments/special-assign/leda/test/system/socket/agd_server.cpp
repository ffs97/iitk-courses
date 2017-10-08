/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  agd_server.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//----------------------------------------------------------------------------- //
//    agd_server
//
//    S. Naeher
//
//    February 2000
//
//-----------------------------------------------------------------------------

#define VERSION 1.0
#define PORTNUM 9660
#define TIMEOUT 15 

#include <LEDA/graphwin.h>
#include <LEDA/file.h>
#include <LEDA/socket.h>
#include <LEDA/stream.h>
#include <LEDA/std/stdio.h>

using std::ofstream;
using std::ifstream;
using std::ostream;
using std::istream;
using std::ios;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;



#include <AGD/FPPLayout.h>
#include <AGD/SchnyderLayout.h>
#include <AGD/ConvexLayout.h>
#include <AGD/ConvexDrawLayout.h>
#include <AGD/PlanarStraightLayout.h>
#include <AGD/PlanarDrawLayout.h>
#include <AGD/MixedModelLayout.h>
#include <AGD/TreeLayout.h>
#include <AGD/PlanarizationLayout.h>
#include <AGD/SugiyamaLayout.h>
#include <AGD/SpringLayout.h>
#include <AGD/TutteLayout.h>
#include <AGD/PureOrthogonalLayout.h>
#include <AGD/QuasiOrthogonalLayout.h>
#include <AGD/OrthogonalLayout.h>
#include <AGD/VisibilityRepresentation.h>
#include <AGD/GraphWinInterface.h>
#include <AGD/MaxAllMapper.h>

#include <AGD/LEDAMakeAcyclic.h>

#include <AGD/GreedyCycleRemoval.h>
#include <AGD/FastHierarchyLayout.h>
#include <AGD/DfsRanking.h>
#include <AGD/LongestPathRanking.h>
#include <AGD/CoffmanGrahamRanking.h>
#include <AGD/BarycenterHeuristic.h>
#include <AGD/MedianHeuristic.h>
#include <AGD/WeightedMedianHeuristic.h>
#include <AGD/GreedyInsertHeuristic.h>
#include <AGD/GreedySwitchHeuristic.h>
#include <AGD/SplitHeuristic.h>
#include <AGD/SiftingHeuristic.h>

#include <AGD/SubgraphPlanarizer.h>
#include <AGD/PlanarSubgraph.h>
#include <AGD/ShortestPathInserter.h>

//#include <AGD/MaximumPlanarSubgraph.h>


#include <ctype.h>


#define WRITE_OPTION(out,x,name,type)\
  out << #name << " " << #type << " " << x.name()  << endl;

#define READ_OPTION(in,x,s,name,type)\
if (s == #name) { type val; in >> val; x.name(val); }


//LEDA_BEGIN_NAMESPACE

using namespace leda;

void fpp_write_opts(ostream&, LayoutModule&) {}
void fpp_read_opts(istream&, LayoutModule&) {}

void schnyder_write_opts(ostream&, LayoutModule&) {}
void schnyder_read_opts(istream&, LayoutModule&) {}

void tutte_write_opts(ostream&, LayoutModule&) {}
void tutte_read_opts(istream&, LayoutModule&) {}

void visibility_write_opts(ostream&, LayoutModule&) {}
void visibility_read_opts(istream&, LayoutModule&) {}


void convex_write_opts(ostream& out, LayoutModule& LM) 
{ ConvexLayout& x = (ConvexLayout&)LM;
  WRITE_OPTION(out,x,size_opt,bool);
  WRITE_OPTION(out,x,base_ratio,float);
}

void convex_read_opts(istream& in, LayoutModule& LM) 
{ ConvexLayout& x = (ConvexLayout&)LM;
  string s;
  while (in >> s) { 
    READ_OPTION(in,x,s,size_opt,bool);
    READ_OPTION(in,x,s,base_ratio,float);
  }
}
   

void convexdraw_write_opts(ostream& out, LayoutModule& LM) 
{ ConvexDrawLayout& x =  (ConvexDrawLayout&)LM;
  WRITE_OPTION(out,x,size_opt,bool);
  WRITE_OPTION(out,x,side_opt,bool);
  WRITE_OPTION(out,x,base_ratio,float);
}

void convexdraw_read_opts(istream& in, LayoutModule& LM) 
{ ConvexDrawLayout& x = (ConvexDrawLayout&)LM;
  string s;
  while (in >> s) { 
    READ_OPTION(in,x,s,size_opt,bool);
    READ_OPTION(in,x,s,side_opt,bool);
    READ_OPTION(in,x,s,base_ratio,float);
  }
}
   


void planarstraight_write_opts(ostream& out, LayoutModule& LM) 
{ PlanarStraightLayout& x = (PlanarStraightLayout&)LM;
  WRITE_OPTION(out,x,size_opt,bool);
  WRITE_OPTION(out,x,max_outer,bool);
  WRITE_OPTION(out,x,base_ratio,float);
}

void planarstraight_read_opts(istream& in, LayoutModule& LM) 
{ PlanarStraightLayout& x = (PlanarStraightLayout&)LM;
  string s;
  while (in >> s) { 
    READ_OPTION(in,x,s,size_opt,bool);
    READ_OPTION(in,x,s,max_outer,bool);
    READ_OPTION(in,x,s,base_ratio,float);
  }
}
   




void planardraw_write_opts(ostream& out, LayoutModule& LM) 
{ PlanarDrawLayout& x = (PlanarDrawLayout&)LM;
  WRITE_OPTION(out,x,size_opt,bool);
  WRITE_OPTION(out,x,side_opt,bool);
  WRITE_OPTION(out,x,max_outer,bool);
  WRITE_OPTION(out,x,base_ratio,float);
}

void planardraw_read_opts(istream& in, LayoutModule& LM) 
{ PlanarDrawLayout& x = (PlanarDrawLayout&)LM;
  string s;
  while (in >> s) { 
    READ_OPTION(in,x,s,size_opt,bool);
    READ_OPTION(in,x,s,side_opt,bool);
    READ_OPTION(in,x,s,max_outer,bool);
    READ_OPTION(in,x,s,base_ratio,float);
  }
}
   



void mixedmodel_write_opts(ostream& out, LayoutModule& LM) 
{ MixedModelLayout& x = (MixedModelLayout&)LM;
  WRITE_OPTION(out,x,base_ratio,float);
}

void mixedmodel_read_opts(istream& in, LayoutModule& LM) 
{ MixedModelLayout& x = (MixedModelLayout&)LM;
  string s;
  while (in >> s) { 
   READ_OPTION(in,x,s,base_ratio,float);
  }
}
   


void tree_write_opts(ostream& out, LayoutModule& LM) 
{ TreeLayout& x = (TreeLayout&)LM;
  WRITE_OPTION(out,x,sib_separation,float);
  WRITE_OPTION(out,x,subtree_separation,float);
  WRITE_OPTION(out,x,level_separation,float);
  WRITE_OPTION(out,x,left_margin,float);
  WRITE_OPTION(out,x,upper_margin,float);
}

void tree_read_opts(istream& in, LayoutModule& LM) 
{ TreeLayout& x = (TreeLayout&)LM;
  string s;
  while (in >> s) {
    READ_OPTION(in,x,s,sib_separation,float);
    READ_OPTION(in,x,s,subtree_separation,float);
    READ_OPTION(in,x,s,level_separation,float);
    READ_OPTION(in,x,s,left_margin,float);
    READ_OPTION(in,x,s,upper_margin,float);
  }
}
   


void sugiyama_write_opts(ostream& out, LayoutModule& LM) 
{ SugiyamaLayout& x = (SugiyamaLayout&)LM;

  out << "ranking  enum 3  Longest-Path Coffman-Graham DFS ";
  out << "Longest-Path" << endl;

  out << "cycle_removal  enum 2  DFS Greedy ";
  out << "DFS" << endl;
 
  out << "cross_min  enum 5 ";
  out << "Barycenter Median Weighted-Median Split Sifting ";
  out << "Barycenter" << endl;

  WRITE_OPTION(out,x,runs,int);
  WRITE_OPTION(out,x,fails,int);
  WRITE_OPTION(out,x,init_order,bool);
  WRITE_OPTION(out,x,transpose,bool);

  FastHierarchyLayout fhl;
  WRITE_OPTION(out,fhl,node_distance,float);
  WRITE_OPTION(out,fhl,layer_distance,int);
  WRITE_OPTION(out,fhl,fixed_layer_distance,bool);

}


void sugiyama_read_opts(istream& in, LayoutModule& LM) 
{ SugiyamaLayout& x = (SugiyamaLayout&)LM;

  static FastHierarchyLayout  fhl;
  static LongestPathRanking   lpr;
  static CoffmanGrahamRanking cgr;
  static GreedyCycleRemoval   greedy_cycle_removal;
  static LEDAMakeAcyclic      make_acyclic;

  string s;
  while (in >> s)
  { 
    READ_OPTION (in,fhl,s,node_distance,float);
    READ_OPTION (in,fhl,s,layer_distance,int);
    READ_OPTION (in,fhl,s,fixed_layer_distance,bool);

    READ_OPTION (in,x,s,runs,int);
    READ_OPTION (in,x,s,fails,int);
    READ_OPTION (in,x,s,init_order,bool);
    READ_OPTION (in,x,s,transpose,bool);

    if (s == "cycle_removal") 
    { in >> s;
      if (s == "DFS")     
      { lpr.set_subgraph(make_acyclic);
        cgr.set_subgraph(make_acyclic);
       }
      if (s == "Greedy")  
      { lpr.set_subgraph(greedy_cycle_removal);
        cgr.set_subgraph(greedy_cycle_removal);
       }
     }


    if (s == "ranking")
    { in >> s;
      if (s == "Longest-Path")   x.set_ranking(lpr);
      if (s == "Coffman-Graham") x.set_ranking(cgr);
      if (s == "DFS")            x.set_ranking(DfsRanking());
     }

    if (s == "cross_min")     
    { in >> s;
      if (s == "Barycenter")      x.set_cross_min(BarycenterHeuristic());
      if (s == "Median")          x.set_cross_min(MedianHeuristic());
      if (s == "Weighted-Median") x.set_cross_min(WeightedMedianHeuristic());
      if (s == "Split")           x.set_cross_min(SplitHeuristic());
      if (s == "Sifting")         x.set_cross_min(SiftingHeuristic());
     }
  }

  x.set_comp_coord(fhl);
}



void spring_write_opts(ostream& out, LayoutModule& LM) 
{ SpringLayout& x = (SpringLayout&)LM;
  WRITE_OPTION(out,x,noise,bool);
  WRITE_OPTION(out,x,iterations,int);
}

void spring_read_opts(istream& in, LayoutModule& LM) 
{ SpringLayout& x = (SpringLayout&)LM;
  string s;
  while (in >> s)
  { READ_OPTION(in,x,s,noise,bool);
    READ_OPTION(in,x,s,iterations,int);
   }
}


void pureorthogonal_write_opts(ostream& out, LayoutModule& LM) 
{ PureOrthogonalLayout& x = (PureOrthogonalLayout&)LM;
  //WRITE_OPTION(out,x,output_level,int);
  WRITE_OPTION(out,x,verify,bool);
}

void pureorthogonal_read_opts(istream& in, LayoutModule& LM) 
{ PureOrthogonalLayout& x = (PureOrthogonalLayout&)LM;
  string s;
  while (in >> s)
  { READ_OPTION(in,x,s,verify,bool);
    READ_OPTION(in,x,s,output_level,int);
   }
}


void quasiorthogonal_write_opts(ostream& out, LayoutModule& LM) 
{ QuasiOrthogonalLayout& x = (QuasiOrthogonalLayout&)LM;
  //WRITE_OPTION(out,x,output_level,int);
  WRITE_OPTION(out,x,verify,bool);
 }

void quasiorthogonal_read_opts(istream& in, LayoutModule& LM) 
{ QuasiOrthogonalLayout& x = (QuasiOrthogonalLayout&)LM;
  string s;
  while (in >> s)
  { READ_OPTION(in,x,s,verify,bool);
    READ_OPTION(in,x,s,output_level,int);
   }
}


void orthogonal_write_opts(ostream& out, LayoutModule& LM) 
{ OrthogonalLayout& x = (OrthogonalLayout&)LM;
  //WRITE_OPTION(out,x,output_level,int);
  WRITE_OPTION(out,x,verify,bool);
 }

void orthogonal_read_opts(istream& in, LayoutModule& LM) 
{ OrthogonalLayout& x = (OrthogonalLayout&)LM;
  string s;
  while (in >> s)
  { READ_OPTION(in,x,s,verify,bool);
    READ_OPTION(in,x,s,output_level,int);
   }
}


void planarization_write_opts(ostream& out, LayoutModule& LM) 
{ PlanarizationLayout& x = (PlanarizationLayout&)LM;
  out << "planar_layouter enum 3  Mixed-Model Orthogonal Quasi-Orthogonal ";
  out << "Orthogonal" << endl;
}

void planarization_read_opts(istream& in, LayoutModule& LM) 
{ PlanarizationLayout& x = (PlanarizationLayout&)LM;

  static MixedModelLayout      mixed;
  static OrthogonalLayout      ortho;
  static QuasiOrthogonalLayout quod;

  string s;
  while (in >> s)
  { if (s == "planar_layouter")
    { in >> s;
      if (s == "Mixed-Model")      x.set_planar_layouter(mixed);
      if (s == "Orthogonal")       x.set_planar_layouter(ortho);
      if (s == "Quasi-Orthogonal") x.set_planar_layouter(quod);
     }
   }
}



typedef void (*write_opt_func)(ostream&,LayoutModule&);
typedef void (*read_opt_func)(istream&,LayoutModule&);



class agd_server {

   enum { L_GRID=1, L_RECT=2 };

   LayoutModule*  module_list[256];
   unsigned       module_opts[256];
   write_opt_func module_write_opts[256];
   read_opt_func  module_read_opts[256];
   int            module_num;

   leda_socket sock;

   string agd_directory;

   ofstream log;

   GraphWin gw;

   int  get_module_index(string cmd);

   int  get_algorithm();

   void send_version();
   void send_functions();
   void send_options(int i);
   void send_infos(int i);

   bool call_function(int i);

   bool receive_graph();
   bool send_graph();

   string graph_format;

   string tmp_fname;

public:

  void error_msg(string  msg) 
  { log << "error message: " << msg << endl;
    sock.send_string("error");
    string cmd;
    if (!sock.receive_string(cmd)) 
      log << "receive_string (cmd): " + sock.get_error() << endl;
    else
    { log << "cmd   = " << cmd << endl;
      if (cmd == "result") sock.send_string(msg);
     }
   }

  agd_server(int port, int qlen, int timeout, string dname);
 ~agd_server();

  void set_port(int p) { sock.set_port(p); }

  bool listen() 
  { if (sock.listen()) return true;
    cerr << sock.get_error() << endl;
    return false;
   }

  bool accept() 
  { log << string("AGD Server %.2f ready to serve.",VERSION) << endl;
    if (!sock.accept())
    { log << sock.get_error() << endl;
      return false;
     }
    return true;
   }

  void close()  
  { sock.disconnect();
    if (is_file(tmp_fname)) delete_file(tmp_fname);
    log << "connection closed" << endl;
    log << endl;
   }

  void handle_request();

};


agd_server::agd_server(int port, int qlen, int timeout, string dname) 
                                                            : gw("agd_server")
{ 
  sock.set_port(port);
  sock.set_qlength(qlen);
  sock.set_timeout(timeout);

  agd_directory = dname;

  if (is_directory(dname)) 
    log.open(agd_directory + "/LOGFILE",ios::app);
  else
    { log.open("/dev/null");
      cerr << dname << ": no such directory" << endl;
      cerr << endl;
     }


  log << endl << endl;
  log << "AGD Server started at " << time_string() << endl;
  log << endl;


  tmp_fname = tmp_file_name();

  int i = 0;

  module_list[i] = new FPPLayout;
  module_opts[i] = L_GRID;
  module_write_opts[i] = fpp_write_opts;
  module_read_opts[i] = fpp_read_opts;
  i++;
  module_list[i] = new SchnyderLayout;
  module_opts[i] = L_GRID;
  module_write_opts[i] = schnyder_write_opts;
  module_read_opts[i] = schnyder_read_opts;
  i++;
  module_list[i] = new PlanarStraightLayout;
  module_opts[i] = L_GRID;
  module_write_opts[i] = planarstraight_write_opts;
  module_read_opts[i] = planarstraight_read_opts;
  i++;
  module_list[i] = new PlanarDrawLayout;
  module_opts[i] = L_GRID;
  module_write_opts[i] = planardraw_write_opts;
  module_read_opts[i] = planardraw_read_opts;
  i++;
  module_list[i] = new MixedModelLayout;
  module_opts[i] = L_GRID;
  module_write_opts[i] = mixedmodel_write_opts;
  module_read_opts[i] = mixedmodel_read_opts;
  i++;
  module_list[i] = new ConvexLayout;
  module_opts[i] = L_GRID;
  module_write_opts[i] = convex_write_opts;
  module_read_opts[i] = convex_read_opts;
  i++;
  module_list[i] = new ConvexDrawLayout;
  module_opts[i] = L_GRID;
  module_write_opts[i] = convexdraw_write_opts;
  module_read_opts[i] = convexdraw_read_opts;
  i++;
  module_list[i] = new VisibilityRepresentation;
  module_opts[i] = L_GRID | L_RECT;
  module_write_opts[i] = visibility_write_opts;
  module_read_opts[i] = visibility_read_opts;
  i++;
  module_list[i] = new OrthogonalLayout;
  module_opts[i] = L_RECT;
  module_write_opts[i] = orthogonal_write_opts;
  module_read_opts[i] = orthogonal_read_opts;
  i++;
  module_list[i] = new PureOrthogonalLayout;
  module_opts[i] = L_RECT;
  module_write_opts[i] = pureorthogonal_write_opts;
  module_read_opts[i] = pureorthogonal_read_opts;
  i++;
  module_list[i] = new QuasiOrthogonalLayout;
  module_opts[i] = L_RECT;
  module_write_opts[i] = quasiorthogonal_write_opts;
  module_read_opts[i] = quasiorthogonal_read_opts;
  i++;
  module_list[i] = new TreeLayout;
  module_opts[i] = 0;
  module_write_opts[i] = tree_write_opts;
  module_read_opts[i] = tree_read_opts;
  i++;
  module_list[i] = new SugiyamaLayout;
  module_opts[i] = 0;
  module_write_opts[i] = sugiyama_write_opts;
  module_read_opts[i] = sugiyama_read_opts;
  i++;
  module_list[i] = new SpringLayout ;
  module_opts[i] = 0;
  module_write_opts[i] = spring_write_opts;
  module_read_opts[i] = spring_read_opts;
  i++;
  module_list[i] = new TutteLayout ;
  module_opts[i] = 0;
  module_write_opts[i] = tutte_write_opts;
  module_read_opts[i] = tutte_read_opts;
  i++;
  module_list[i] = new PlanarizationLayout;
  module_opts[i] = 0;
  module_write_opts[i] = planarization_write_opts;
  module_read_opts[i] = planarization_read_opts;
  i++;

  module_num = i;
}


agd_server::~agd_server() 
{ for(int i=0; i< module_num; i++) delete module_list[i]; 
}

void agd_server::send_version() 
{ sock.send_string(string("%.2f",VERSION)); }


void agd_server::send_functions() 
{ for(int i = 0; i<module_num; i++)
     sock.ostr() << module_list[i]->name() << endl;
  sock.send();
}


bool agd_server::receive_graph() 
{
  bool stat = true;

  string fname = tmp_fname;

  if (is_directory(agd_directory))
     fname = agd_directory + "/" + sock.get_ip();

  if (!sock.receive_file(fname)) 
  { error_msg("receive_graph: " + sock.get_error());
    return false;
   }

  if (graph_format == "gml") 
  { if (gw.read_gml(fname) != 0) 
    { error_msg("receive_graph: error in gml format.");
      stat = false;
     }
   }
  else
  if (graph_format == "gw") 
  { if (gw.read_gw(fname) != 0)
    { error_msg("receive_graph: error in gw format.");
      stat = false;
     }
   }

  if (stat)
  { graph& G = gw.get_graph();
    int n = G.number_of_nodes();
    int m = G.number_of_edges();
    log << "#nodes = " << n << endl;
    log << "#edges = " << m << endl;
  }

  return stat;
}


bool agd_server::send_graph() 
{ bool b = false;
  ostream& out = sock.ostr();
  if (graph_format == "gml") b = gw.save_gml(out);
  if (graph_format == "gw")  b = gw.save_layout(out);
  if (b) sock.send();
  return b;
}


int agd_server::get_module_index(string cmd)
{ int i = module_num-1;
  while (i >= 0)
  { LayoutModule* lm = module_list[i];
    if (cmd == lm->name()) break;
    i--;
   }
  return i;
}


int agd_server::get_algorithm()
{ string alg;
  if (!sock.receive_string(alg)) 
  { error_msg("receive_string (alg): " + sock.get_error());
    return -1;
   }
  log << "name   = " << alg << endl;
  if (alg == "echo") return module_num;
  int i = get_module_index(alg);
  if (i < 0) error_msg(string("%s: unknown layout module.",~alg));
  return i;
}




bool agd_server::call_function(int i)
{
  if (i == module_num) // echo
  { log << "running " << "echo" << endl;
    return true;
   }

  log << "running " << module_list[i]->name() << endl;

  graph& G = gw.get_graph();

  LayoutModule* F         = module_list[i];
  read_opt_func read_opts = module_read_opts[i];
  int           mod_opts  = module_opts[i];

  ifstream opt_in(tmp_fname);
  read_opts(opt_in,*F);

  if (G.empty()) 
  { error_msg("Warning: Graph is empty.");
    return false;
   }


  if (mod_opts & L_RECT)
  { node v;
    forall_nodes(v,G) gw.set_shape(v,rectangle_node);
   }

  if (mod_opts & L_GRID)
  { MaxAllMapper mapper (40,2,20,DPoint(10,10));
    ((GridLayoutModule*)F)->set_mapper(mapper);
   }


  node_array<int> n_index(G);
  edge_array<int> e_index(G);

  int count = 0;
  node v;
  forall_nodes(v,G) 
  { n_index[v] = G.index(v);
    edge e;
    forall_adj_edges(e,v) e_index[e] = count++;
   }  

  GraphWinInterface I(gw);

  AgdKey agd_key;
  if (!F->check(G,agd_key)) 
  { error_msg("Unsatisfied graph property: " + agd_key.info());
    return false;
   }

  F->call(G,I); 

  // restore node and edge ordering
  G.sort_nodes(n_index);
  G.sort_edges(e_index);

  return true;
}

void agd_server::send_infos(int i)
{ LayoutModule* F = module_list[i];
  ostream& out = sock.ostr();
  out << "Name:           " << F->long_name()    << endl;
  out << "Author:         " << F->author()       << endl;
  out << "Implementation: " << F->impl_author()  << endl;
  out << "Date:           " << F->impl_date()    << endl;
  out << "Version:        " << F->impl_version() << endl;
  sock.send();
}


void agd_server::send_options(int i)
{ (*module_write_opts[i])(sock.ostr(),*module_list[i]);
  sock.send();
}


void agd_server::handle_request() 
{
  string ip = sock.get_ip();

  log << time_string() << endl;
  log << "hostid = " << ip << endl;

  string client;
  if (!sock.receive_string(client))
  { error_msg("receive_string (client): " + sock.get_error());
    return;
   }
  log << "client = " << client << endl;

  string client_fname = agd_directory + "/CLIENTS";

  bool client_ok = true;

  if (is_file(client_fname))
  { ifstream client_file(client_fname);
    client_ok = false;
    while (client_file)
    { string str = read_line(client_file);
      int len = str.length();
      if (len > 0 && str == ip(0,len-1)) client_ok = true;
     }
   }

  if (client_ok)
    sock.send_string("ok");
  else
   { error_msg("Client (" + ip + ") not authorized.");
     return;
    }


  string cmd;
  if (!sock.receive_string(cmd))
  { error_msg("receive_string (cmd): " + sock.get_error());
    return;
   }

  log << "cmd    = " << cmd << endl;


  if (cmd == "version") 
  { send_version();
    return;
   }

  if (cmd == "list") 
  { sock.send_string("ok");
    if (!sock.receive_string(cmd)) 
    { error_msg("receive_string (cmd): " + sock.get_error());
      return;
     }
    if (cmd == "result") send_functions();
    return;
   }

  if (cmd == "info")
  { sock.send_string("ok");
    int alg_i = get_algorithm();
    if (alg_i < 0 || alg_i >= module_num) return;
    sock.send_string("ok");
    if (!sock.receive_string(cmd)) 
    { error_msg("receive_string (cmd): " + sock.get_error());
      return;
     }
    if (cmd == "result") send_infos(alg_i);
    return;
   }

  if (cmd == "options")
  { sock.send_string("ok");
    int alg_i = get_algorithm();
    if (alg_i < 0 || alg_i >= module_num) return;
    sock.send_string("ok");
    if (!sock.receive_string(cmd)) 
    { error_msg("receive_string (cmd): " + sock.get_error());
      return;
     }
    if (cmd == "result") send_options(alg_i);
    return;
   }
 

  if (cmd == "layout")
  { sock.send_string("ok");
    int alg_i = get_algorithm();
    if (alg_i < 0) return; 

    for(;;)
    { 
      sock.send_string("ok");

      if (!sock.receive_string(cmd))
      { log << "receive_string (cmd): " << sock.get_error() << endl;
        break;
       }

      if (cmd == "options")
      { sock.send_string("ok");
        sock.receive_file(tmp_fname);
        ifstream in(tmp_fname);
        while (in) 
        { string s = read_line(in);
          if (s != "") log << "option = " << s << endl;
         }
        in.close();
        in.open(tmp_fname);
        if (alg_i < module_num)
            (*module_read_opts[alg_i])(in,*module_list[alg_i]);
        continue;
       }
  
      if (cmd == "gml" || cmd == "gw")
      { log << "format = " << cmd << endl;
        graph_format = cmd;
        sock.send_string("ok");
        if (!receive_graph()) break;
        if (!call_function(alg_i)) break;
        continue;
       }
  
      if (cmd == "result")
      { log << "sending result" << endl;
        send_graph(); 
        break;
       }

      error_msg("unknown command: " + cmd);
      break;

     }

    return;
   }

  error_msg("unknown command: " + cmd);
}


static void agd_error_handler(int i, const char* s)
{ cerr << s << endl; }

//LEDA_END_NAMESPACE



#if defined(WINMAIN)

int main() {
  int    argc;
  char** argv;
  get_command_line(argc,argv);

#else

int main(int argc, char** argv) {

#endif

  int portnum = PORTNUM;
  int timeout = TIMEOUT;
  int qlength = 5;

  string agddir = "/LEDA/server/agd";

  window::do_not_open_display(1);

  for(int i=1; i<argc; i++)
  { string param = argv[i]; 
    if (isdigit(param[0]))
      portnum = atoi(param);
    else 
      agddir =  param;
   }

  agd_server S(portnum,qlength,timeout,agddir);

/*
  catch_system_errors();
*/
  set_error_handler(agd_error_handler);

  if (!S.listen()) return 1;

  for(;;)
  { if (!S.accept()) continue;
    S.handle_request();
    S.close();
  }

  return 0;
}
