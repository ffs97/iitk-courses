
LFLAGS = $(WFLAG)

DFLAGS = -DWINMAIN

LROOT   = ../..
UNIXLIB = -lleda $(XLIB) -lm
DOSLIB  = ..\\..\\leda.lib $(XLIB)

WINDOW_APP = 1

include ../Make.pro

