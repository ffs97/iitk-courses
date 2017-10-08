.SUFFIXES:
.SUFFIXES: .asm .s .c .cpp .o .a
VERS = 

CC      = g++$(VERS) -Wall --warn-error
PLAIN_C = gcc$(VERS) -Wall
CCLIB   = g++$(VERS) -shared
LD      = g++$(VERS)

PIC     = -fpic
CFLAGS  = -O2 -fno-strict-aliasing

so  = .so
a   = .a
e   = 
o   = .o
s   = .asm
 
AR  = ar $(ARFLAG) cr
ASM = yasm -m amd64 -f elf
XLIB    = -lX11

.SUFFIXES: .cpp

VPATH = ".\"

IPATH1 = $(LROOT)/incl

include Make.lst

.c$(s):
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS)  -I$(IPATH1) -S $*.c

.cpp$(o):
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS)  -I$(IPATH1) -c $*.cpp

$(o)$(e):
	$(LD) $(LFLAGS) -o $* $*$(o) $(OBJECTS) $(XLIB_PATH) -L$(LROOT) $(UNIXLIB) $(SOCKLIB)

.cpp$(e):
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS)  -I$(IPATH1) -c $*.cpp
	$(LD) $(LFLAGS) -o $* $*$(o) $(OBJECTS) $(XLIB_PATH) -L$(LROOT) $(UNIXLIB) $(SOCKLIB)


all:  $(SUBDIRS) $(OBJECTS) $(PROGS)


clean:
	rm -f *$(o) */*$(o) core

del:
	rm -f *$(o) */*$(o) $(PROGS) core


static_xlman:
	$(LD) -o xlman$(e) xlman$(o) $(LROOT)/libleda.a $(XLIB_PATH) $(XLIB) $(SOCKLIB)
	strip xlman$(e)

