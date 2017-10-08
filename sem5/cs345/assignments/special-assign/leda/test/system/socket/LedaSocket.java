import java.net.*;
import java.io.*;
/** Leda Socket fuer kommunikation mit leda server
*/




class LedaSocket extends Socket{

	public LedaSocket (String adress, int port) throws UnknownHostException,
		IOException
	{
		super(adress, port);
	}
  
 
	public LedaOutputStream getLedaOutputStream()throws IOException {
		return new	LedaOutputStream 
			(new BufferedOutputStream (getOutputStream()));
	}
	public LedaInputStream getLedaInputStream() throws IOException{
		return new LedaInputStream
			(new BufferedInputStream (getInputStream()));
	}
}


/*{\Manpage {LedaSocket} {} {Java LEDA Socket}}*/

/*{\Mdefinition
bla bal
}*/

/*{\Mcreation Socket}*/

/*{\Moptions nextwarning=no}*/
/*
 LedaSocket (String adress, int port);
*/
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            to the empty receiver list. }*/
            


/*{\Moptions nextwarning=no}*/
/*
 LedaSocket (String adress, int port);
*/
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it
            to the empty receiver list. }*/



/*{\Moperations 1.3 2.7 }*/



/*{\Moptions nextwarning=no}*/
/*
LedaOutputStream getLedaOutputStream();
*/

/*{\Mop calls all attached functions with |p1|,|p2|,\ldots as
        parameters. You can also use the parenthesis operator.}*/




/*{\Moptions nextwarning=no}*/
/*
LedaOutputStream getLedaOutputStream();
*/

/*{\Mop calls all attached functions with |p1|,|p2|,\ldots as
        parameters. You can also use the parenthesis operator.}*/



/*{\Mexample  
  \begin{verbatim}
  #include <event.h>

  #ifdef LEDA_NAMESPACE
  using namespace leda;
  #endif

  #ifdef LEDA_STD_IO_HEADERS
  using std::cout;
  #endif

  void by_reference(int& val)
  { cout << "Call by reference:  " << val <<  " Change value! \n";
    val = 3;
  }

  void by_value(int val)
  { cout << "Call by value:      " << val << "\n"; }

  int main()
  {
    event e;
  
    attach(e,by_value);
    attach(e,by_reference);
  
    int i = 10;
  
    occur(e,i);
    occur<int&>(e,i);  
    occur(e,i);
  
    return 0;
  }
  \end{verbatim}
}*/
