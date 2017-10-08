/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  agd_client.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


//-------------------------------------------------------------------
//
//                       agd_client.C
//
//    author: Gunter Koch
//    date:   April 1998
//-------------------------------------------------------------------

#include <LEDA/graphics/graphwin.h>   
#include <LEDA/agd_serv_base.h>
#include <fstream.h>
#include <stdio.h>

using namespace leda;

class read_agd_serv : agd_serv_base {


private:

  int agd_serv_return;
  bool with_optfile;

  int gw_save (string ifile);

  int gw_read (string ofile);

  void error_out(string & ofile,string & ifile);


  int write_optfile(string optfile );

  int make_menu_list();

public :

   read_agd_serv () {
        with_optfile = true;
        agd_serv_return = make_menu_list();
      	file_type_gml = true;
   };

   read_agd_serv (GraphWin& G) {
        gw = &G;
        with_optfile = true;
        agd_serv_return = make_menu_list();
      	file_type_gml = true;
   };


   read_agd_serv (GraphWin& G, bool _with_optfile) {
      	gw = &G;
        with_optfile = _with_optfile;
        agd_serv_return = make_menu_list();
        file_type_gml = true;
   };

   read_agd_serv (GraphWin& G, bool _with_optfile, bool file_gml) {
      	gw = &G;
        with_optfile = _with_optfile;
        agd_serv_return = make_menu_list();
        file_type_gml = file_gml;
   };



   ~read_agd_serv() { };

    
   string name () const {
      return string ("read_agd_server");
   };

   string long_name () const {
      return string ("read_agd_server");
   };

   string author () const {
      return (" ");
   };

   string impl_author () const {
      return string ("Gunter Koch");
   };

   string impl_version () const {
      return string ("1.0");
   };

   string impl_date () const {
      return string ("April 1998");
   };
   
   void init(GraphWin & G) {gw = &G;};

   void init(GraphWin & G,bool& _with_optfile) {
      	gw = &G;
        with_optfile = _with_optfile;
   };

   void init(GraphWin & G, bool& _with_optfile, bool& file_gml) {
      	gw = &G;
        with_optfile = _with_optfile;
        file_type_gml = file_gml;
   };

   bool gml_file_typ () {return file_type_gml;};

   void gml_file_typ(bool file_gml) {file_type_gml = file_gml;};
   
   bool check () { if (agd_serv_return == 0) return true; else return false;};
   
   bool check_optfile () {return(with_optfile);};

   void fktopt (int i);

   void run_agd(int i);

   bool make_function_list( list <string>& L);

   bool make_option_list( list <string>& L);

};


int read_agd_serv::gw_save(string ifile) {
   if (file_type_gml) return (gw->save_gml(ifile));
   return (gw->save(ifile));
};

int read_agd_serv::gw_read (string ofile) {
   if (file_type_gml) return (gw->read_gml(ofile));
   return (gw->read(ofile));
};


   	//Fehlerausgabe und löschen der tmp-files 


void read_agd_serv::error_out(string & ofile,string & ifile) {
	ifstream in (ofile);
        if ( ! in ) gw->acknowledge 
           	("can't read or write  "+ifile+" or "+ ofile  );
        else {
		string msg;
		msg.read_file(in); 
		gw->acknowledge (msg);
                system("rm -f " + ifile);
                system("rm -f " + ofile);
	};
};




int read_agd_serv::write_optfile(string optfile) {
              	                        	//Erzeugen der Ausgabedatei

   agd_serv_intern_struct_def agd_intern_struct;
   agd_serv_struct_def agd_serv_struct;
   ofstream opt (optfile, ios::out | ios::app);
   if (!opt) {
      gw->acknowledge("can't write file \""+ optfile+"\"");
      return(1);
   }; 
      
   forall (agd_serv_struct,fkt_opt_list) {
	opt << "fkt" << '\n';
	opt << agd_serv_struct.fkt << '\n';

	forall (agd_intern_struct,agd_serv_struct.optenum_list) {
		opt << "enum" << "\n";
		opt << agd_intern_struct.string_name << '\n';
                opt << agd_intern_struct.wert.int_wert;
		string s;
		forall (s,*(agd_intern_struct.enum_list)) 
                   {opt << s << '\n';};
		opt << "enum_end" << "\n";
	};

	forall (agd_intern_struct,agd_serv_struct.optbool_list) {
		opt << "bool" << "\n"; 
                opt << agd_intern_struct.string_name << '\n';
                opt << agd_intern_struct.wert.bool_wert << "bool_end" << "\n";
	};

	forall (agd_intern_struct,agd_serv_struct.optint_list) {
		opt <<"int" << "\n";
                opt << agd_intern_struct.string_name << '\n'; 
                opt <<  agd_intern_struct.wert.int_wert << "int_end" << "\n"; 
	};

	forall (agd_intern_struct,agd_serv_struct.optdouble_list) {
		opt << "double" << "\n";
                opt << agd_intern_struct.string_name << '\n'; 
                opt << agd_intern_struct.wert.double_wert << "double_end" <<"\n";
	};

	forall (agd_intern_struct,agd_serv_struct.optfloat_list) {
		opt << "float" << "\n";
                opt << agd_intern_struct.string_name << '\n'; 
                opt << agd_intern_struct.wert.float_wert << "float_end" <<"\n";
	};
	forall (agd_intern_struct,agd_serv_struct.optstring_list) {
		opt << "string" << "\n";
                opt << agd_intern_struct.string_name << '\n'; 
                opt << *(agd_intern_struct.wert.string_wert) << "string_end" <<"\n";
	};
	opt << "fkt_end" << "\n";
   };
   opt.close();
   return(0);
};


                	// Eingabe für optionale Parameter

void read_agd_serv::fktopt (int i) {

   int r;
   string s;
   list<string> lst; 
   list_item it = fkt_opt_list.get_item(i);
   agd_serv_struct_def agd_serv_struct;
   agd_serv_struct = fkt_opt_list[it];
   agd_serv_intern_struct_def agd_intern_struct;
   	
                	// Panel für Eingabe der optionalen Paramaeter

        
   panel P ("Options for " + agd_serv_struct.fkt);
   r = 0;

   int enum_array [agd_serv_struct.optenum_list.size()];
   forall (agd_intern_struct,agd_serv_struct.optenum_list) {
      s = agd_intern_struct.string_name;
      enum_array[r] = agd_intern_struct.wert.int_wert;
      list <string> enumlist =  *(agd_intern_struct.enum_list);
      P.choice_item (s, enum_array[r++],enumlist);
      };
   r = 0;
   bool bool_array [agd_serv_struct.optbool_list.size()];
   forall (agd_intern_struct,agd_serv_struct.optbool_list) {
      s = agd_intern_struct.string_name; 
      bool_array[r] = agd_intern_struct.wert.bool_wert;
      P.bool_item (s, bool_array[r++] );
   };
   r = 0;
   int int_array [agd_serv_struct.optint_list.size()];
   forall (agd_intern_struct,agd_serv_struct.optint_list) {
      s = agd_intern_struct.string_name; 
      int_array[r] = agd_intern_struct.wert.int_wert;
      P.int_item (s, int_array[r++] );
   };
   r = 0;
   double double_array [agd_serv_struct.optdouble_list.size()];
   forall (agd_intern_struct,agd_serv_struct.optdouble_list) {
      s = agd_intern_struct.string_name; 
      double_array[r] = agd_intern_struct.wert.double_wert;
      P.double_item (s, double_array[r++] );
   };
   r = 0;
   double float_array [agd_serv_struct.optfloat_list.size()];
   forall (agd_intern_struct,agd_serv_struct.optfloat_list) {
      s = agd_intern_struct.string_name; 
      float_array[r] = (double) agd_intern_struct.wert.float_wert;
      P.double_item (s,float_array[r++]);
   };
   r = 0;
   string string_array [agd_serv_struct.optstring_list.size()];
   forall (agd_intern_struct,agd_serv_struct.optstring_list) {
      s = agd_intern_struct.string_name; 
      string_array[r] = *(agd_intern_struct.wert.string_wert);
      P.string_item (s,string_array[r++]);
   };


   const int
    OK     = P.button( "Ok" ),
    INFO   = P.button( "Info" ),
    CANCEL = P.button ("Cancel");
   int wahl;
   P.display(gw->get_window(),window::center,window::center);


        	// Anzeige der Informationen über den Algorithmus

   while (( wahl = P.read()) == INFO ){
        
        panel PI(*(agd_serv_struct.optinfo_list.head().wert.string_wert));
        forall (agd_intern_struct,agd_serv_struct.optinfo_list){
           PI.text_item (agd_intern_struct.string_name );
           PI.text_item (*(agd_intern_struct.wert.string_wert));
           PI.text_item("");
        };
       PI.button( "OK" );
       PI.open(gw->get_window());
   };


   if ( wahl == OK ) {
	
        list_item it1;
        r = 0;
	forall_items (it1,fkt_opt_list[it].optenum_list) {
        	fkt_opt_list[it].optenum_list[it1].
                   	wert.int_wert = enum_array[r++]; 
	};
	r = 0;
	forall_items (it1,fkt_opt_list[it].optbool_list) {
                fkt_opt_list[it].optbool_list[it1].
                   	wert.bool_wert = bool_array[r++];
	};
        r = 0;
	forall_items (it1,fkt_opt_list[it].optint_list) {
        	fkt_opt_list[it].optint_list[it1].
                   	wert.int_wert = int_array[r++]; 
	};
        r = 0;
	forall_items (it1,fkt_opt_list[it].optdouble_list) {
		fkt_opt_list[it].optdouble_list[it1].
                   	wert.double_wert = double_array[r++];
	};
        r = 0;

	forall_items (it1,fkt_opt_list[it].optfloat_list) {
		fkt_opt_list[it].optfloat_list[it1].
                   	wert.float_wert = float_array[r++];
	};
        r = 0;
	forall_items (it1,fkt_opt_list[it].optstring_list) {
		*(fkt_opt_list[it].optstring_list[it1].
                   	wert.string_wert) = string_array[r++];
	};
   };
}; 






                        	// Aufruf des AGD_Servers mit einzelnen 
                                //Funktionen als Parameter

void read_agd_serv::run_agd(int i) 
{ 
  list_item it = agd_function_list.get_item(i);
  string algorithmus = agd_function_list[it]; 
  string ifile = (tmpnam(NULL)); // Erzeugen der Tmp-Filenamen
  string ofile = (tmpnam(NULL)); 
  string ansfile = (tmpnam(NULL));
  
  string s; 
  int q;

  
  ofstream ans (ansfile, ios::trunc | ios::app);	
  if (! ans ) gw->acknowledge("can't write file \""+ ansfile +"\"");
  
  else {
  		// falls der Algorythmus Angaben zu seiner 
     ans << "NULL";	// arbeitsweise ausgibt, werden diese in dieser
     ans.close();	// Datei gespeichert
  
   	if (!(gw_save(ifile))){ 
           gw->acknowledge("can't file write\""+ifile+"\"");
           
                	// Speichern des Graphen im iflile
                        // Aufruf des Servers mit den entsprechenden
                        // Parametern     
        
       } else {
         if (with_optfile == true ){
           if ( write_optfile(optfile) == 0) {
              if ((q=system("agd_server \"" + algorithmus + "\" " + 
                 	 + ifile + " " + ofile + " " + 
                         optfile + " " + ansfile)) == 0) {
  
                system("rm -f " + ifile);
                        	// Einlesen des Ausgabegraphen des Servers
                                // in das GraphWin-Fenster                                
                if ((gw_read(ofile))) {
                   gw->acknowledge("can't file read\""+ofile+"\"");
                   system("rm -f " + ofile);
                } else {
              	  system("rm -f " + ofile); gw->fill_window();
                  ifstream ans1 (ansfile);	
                     		// Einlesen des Files, das die Antwort über die 
                        	// arbeitsweise des Algorythmus speichert
                  if (!ans1) gw->acknowledge("can't file read\""+ansfile+"\"");
                  else {
                     s.read_file (ans1);
                     if ( (s != "NULL") && (s.length()>2)) { 
                        	// Ausgabe der Angaben über die Arbeitsweise des 
                                // Algorythmus in einen Panel
			gw->acknowledge(s);
                       ans1.close();
                     };
                     system("rm -f " + ansfile);                   
                  };
                }; 
              } else {error_out(ofile,ifile); }; 
              system("rm -f " + optfile);
           }; 
         } else {
                if ((q=system("agd_server \"" + 
                   	algorithmus + "\" " + ifile + " " + ofile)) == 0) {                     
                   if ((gw_read(ofile))) {
                      gw->acknowledge("can't file read\""+ofile+"\"");
                   } else gw->fill_window(); 
                } else {
                   error_out(ofile,ifile);
                };
                system("rm -f " + ifile);
                system("rm -f " + ofile); 
                system("rm -f " + ansfile);
         };   
        };
  };
};




int read_agd_serv::make_menu_list () {
  
  
  string s;
  int test;
  string fktfile = tmpnam(NULL);
  optfile = string (tmpnam(NULL));
  agd_serv_struct_def agd_serv_struct;
  

                        	// erwartet Rückkehrwert 0 von Server
                                // list dann Files ein 
  if (with_optfile == true) {
     test = system("agd_server check "+ fktfile + " " + optfile); 
  } else {
     test = system("agd_server check "+ fktfile);
  };
  if (test == 0 ) {
	ifstream in_fkt_file (fktfile);
        s.read_line(in_fkt_file);
        if (! in_fkt_file.fail()) {
           do {
              agd_function_list.append(s);
              s.read_line(in_fkt_file);   
           } while (! in_fkt_file.fail());
        };
	in_fkt_file.close();	
        system("rm -f " + fktfile);
                
        if ( with_optfile == true) {        
           if (read_optfile(optfile)){
              system("rm -f " + optfile);
              forall(agd_serv_struct,fkt_opt_list) { 
                 agd_option_list.append(agd_serv_struct.fkt);
              };
           } else with_optfile = false;
           
        };
        return (0);
  }else return (1);
};

 
bool read_agd_serv::make_function_list( list <string>& L) {
        string s;
        if (!check()) return (false);
        forall(s,agd_function_list) {
           L.append(s);
        };
        return (true);
};

bool read_agd_serv::make_option_list( list <string>& L){
        string s;
        if (!with_optfile) return (false);
        forall(s,agd_option_list) {
           L.append(s);
        };
        return (true);
};






           

read_agd_serv test;

void agd_fkt (int i) {
   test.run_agd(i);
};

void opt_fkt (int i) {
   test.fktopt(i);
}



void main(void) {



menu M,M1;
int i,b;
list <string> menu_list;
list <string> opt_menu_list;
string s;
GraphWin gw;
bool file_typ = false;
   
   test.init(gw);

   test.gml_file_typ(file_typ);


   if (test.check()) {
     if (test.make_function_list (menu_list)) { 
   	i=0;
        forall(s,menu_list) M.button(s, i++,agd_fkt);	
        gw.get_window().menu_button ("AGD",M);
        if (test.make_option_list(opt_menu_list)) {  
           b=0;
           forall(s,opt_menu_list) M1.button(s, b++,opt_fkt); 
           gw.get_window().menu_button ("AGD_Options",M1);
        };
     };
   };

   gw.display();
   gw.edit();

};

