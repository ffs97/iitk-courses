# CS345 Special Assignment

## LEDA

Download the LEDA library from (here)[http://www.algorithmic-solutions.info/free/index.php]

Downlaod and extract the LEDA library to a custom path, say <LEDA>. After this,
add the LEDA path to an environment variable LEDAROOT

    export LEDAROOT="<LEDA>"
    export LD_LIBRARY_PATH=$LEDAROOT # For shared libraries

In order to compile with LEDA libraries, use

    g++ -I$LEDAROOT/incl/ -L$LEDAROOT prog.c -o prog -lleda -lX11 -lm

## Running the code

I have created a make file in the code directory to compile all the programs,
however the LEDAROOT and the LD_LIBRARY_PATH environment variables must be set
before invoking make

    cd code
    make

Running the compiled codes

    ./brute         // The brute force algorithm
    ./welzl         // The randomized algorithm
    ./time          // To generate time data
    ./points        // To generate the points
