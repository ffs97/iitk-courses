 
LROOT   = ../..
UNIXLIB = -lleda $(XLIB) -lm
DOSLIB  = ..\\..\\libleda.lib

include ../Make.pro


leda_vs_cln.o: leda_vs_cln.cpp
	$(CC) $(CFLAGS) $(DFLAGS) -I$(LROOT)/incl -c leda_vs_cln.cpp

leda_vs_cln: leda_vs_cln.o
	$(LD) -o leda_vs_cln leda_vs_cln.o -L../.. -lcln $(UNIXLIB)
	

leda_vs_gmp.o: leda_vs_gmp.cpp
	$(CC) $(CFLAGS) $(DFLAGS) -I$(LROOT)/incl -c leda_vs_gmp.cpp

leda_vs_gmp: leda_vs_gmp.o
	$(LD) -o leda_vs_gmp leda_vs_gmp.o -L../.. -lgmp $(UNIXLIB)
	
