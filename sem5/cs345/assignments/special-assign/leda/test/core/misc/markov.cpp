/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  markov.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graph/markov_chain.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int
main(void){

        graph   mG;
        int     simple_in;

	// Before graph construction
        cout << " Statistics before graph construction " << endl;
        print_statistics();

        grid_graph(mG, 10);  // grid graph with 100 nodes
        mG.make_bidirected();

        cout << "Give a number and press Enter : " ;
        cin >> simple_in;	// wait for key+enter
        cout << endl;

	// After graph construction
        cout << " Statistics after graph construction " << endl;
        print_statistics();

        cout << "Give a number and press Enter : " ;
	cin >> simple_in;	
        cout << endl;

        edge_array<int> weight(mG,1);

        for(int cnt=0;cnt<5;cnt++) 
        {
                cout << "Iteration : " << cnt << endl;
                cout << "Allocating memory ... " << endl;

                markov_chain    p(mG, weight);

		// notice that the memory allocated increases
		// instead of remaining constant
                print_statistics();

                //cout << "Give a number and press Enter : " ;
                //cin >> simple_in;

                cout << "End of scope (iteration " << cnt 
                     << ") ==> should deallocate memory ... " << endl;

        }

        return 0;
}
