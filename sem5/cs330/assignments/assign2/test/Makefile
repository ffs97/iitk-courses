# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as
#GCCDIR = ~/gnu/local/decstation-ultrix/bin/
GCCDIR = ../../mips-i386-xgcc/bin/
LDFLAGS = -T script -N
#ASFLAGS = -mips
ASFLAGS =
CPPFLAGS = $(INCDIR)


# if you aren't cross-compiling:
#GCCDIR =
#LDFLAGS = -N -T 0
#ASFLAGS =
#CPPFLAGS = -P $(INCDIR)


CC = $(GCCDIR)gcc
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

#CPP = /lib/cpp
CPP = /usr/bin/cpp
#CPP = $(GCCDIR)cpp
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort printtest vectorsum testregPA forkjoin testexec testyield forkjoin_hard testloop1 testloop2 testloop3 testloop4 testloop5 testloop testlooplong longtest1 longtest2

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.s > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm -f strt.s

halt.o: halt.c
	$(CC) $(INCDIR) -S halt.c -o halt.s
	$(AS) $(CFLAGS) halt.s -o halt.o
#	$(CC) $(CFLAGS) -c halt.c
	rm -f halt.s
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(INCDIR) -S shell.c -o shell.s
	$(AS) $(CFLAGS) shell.s -o shell.o
#	$(CC) $(CFLAGS) -c shell.c
	rm -f shell.s
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(INCDIR) -S sort.c -o sort.s
	$(AS) $(CFLAGS) sort.s -o sort.o
#	$(CC) $(CFLAGS) -c sort.c
	rm -f sort.s
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(INCDIR) -S matmult.c -o matmult.s
	$(AS) $(CFLAGS) matmult.s -o matmult.o
#	$(CC) $(CFLAGS) -c matmult.c
	rm -f matmult.s
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

printtest.o: printtest.c
	$(CC) $(INCDIR) -S printtest.c -o printtest.s
	$(AS) $(CFLAGS) printtest.s -o printtest.o
	rm -f printtest.s
printtest: printtest.o start.o
	$(LD) $(LDFLAGS) start.o printtest.o -o printtest.coff
	../bin/coff2noff printtest.coff printtest

vectorsum.o: vectorsum.c
	$(CC) $(INCDIR) -S vectorsum.c -o vectorsum.s
	$(AS) $(CFLAGS) vectorsum.s -o vectorsum.o
	rm -f vectorsum.s
vectorsum: vectorsum.o start.o
	$(LD) $(LDFLAGS) start.o vectorsum.o -o vectorsum.coff
	../bin/coff2noff vectorsum.coff vectorsum

testregPA.o: testregPA.c
	$(CC) $(INCDIR) -S testregPA.c -o testregPA.s
	$(AS) $(CFLAGS) testregPA.s -o testregPA.o
	rm -f testregPA.s
testregPA: testregPA.o start.o
	$(LD) $(LDFLAGS) start.o testregPA.o -o testregPA.coff
	../bin/coff2noff testregPA.coff testregPA

forkjoin.o: forkjoin.c
	$(CC) $(INCDIR) -S forkjoin.c -o forkjoin.s
	$(AS) $(CFLAGS) forkjoin.s -o forkjoin.o
	rm -f forkjoin.s
forkjoin: forkjoin.o start.o
	$(LD) $(LDFLAGS) start.o forkjoin.o -o forkjoin.coff
	../bin/coff2noff forkjoin.coff forkjoin

testexec.o: testexec.c
	$(CC) $(INCDIR) -S testexec.c -o testexec.s
	$(AS) $(CFLAGS) testexec.s -o testexec.o
	rm -f testexec.s
testexec: testexec.o start.o
	$(LD) $(LDFLAGS) start.o testexec.o -o testexec.coff
	../bin/coff2noff testexec.coff testexec

testyield.o: testyield.c
	$(CC) $(INCDIR) -S testyield.c -o testyield.s
	$(AS) $(CFLAGS) testyield.s -o testyield.o
	rm -f testyield.s
testyield: testyield.o start.o
	$(LD) $(LDFLAGS) start.o testyield.o -o testyield.coff
	../bin/coff2noff testyield.coff testyield

forkjoin_hard.o: forkjoin_hard.c
	$(CC) $(INCDIR) -S forkjoin_hard.c -o forkjoin_hard.s
	$(AS) $(CFLAGS) forkjoin_hard.s -o forkjoin_hard.o
	rm -f forkjoin_hard.s
forkjoin_hard: forkjoin_hard.o start.o
	$(LD) $(LDFLAGS) start.o forkjoin_hard.o -o forkjoin_hard.coff
	../bin/coff2noff forkjoin_hard.coff forkjoin_hard

testloop1.o: testloop1.c
	$(CC) $(INCDIR) -S testloop1.c -o testloop1.s
	$(AS) $(CFLAGS) testloop1.s -o testloop1.o
	rm -f testloop1.s
testloop1: testloop1.o start.o
	$(LD) $(LDFLAGS) start.o testloop1.o -o testloop1.coff
	../bin/coff2noff testloop1.coff testloop1

testloop2.o: testloop2.c
	$(CC) $(INCDIR) -S testloop2.c -o testloop2.s
	$(AS) $(CFLAGS) testloop2.s -o testloop2.o
	rm -f testloop2.s
testloop2: testloop2.o start.o
	$(LD) $(LDFLAGS) start.o testloop2.o -o testloop2.coff
	../bin/coff2noff testloop2.coff testloop2

testloop3.o: testloop3.c
	$(CC) $(INCDIR) -S testloop3.c -o testloop3.s
	$(AS) $(CFLAGS) testloop3.s -o testloop3.o
	rm -f testloop3.s
testloop3: testloop3.o start.o
	$(LD) $(LDFLAGS) start.o testloop3.o -o testloop3.coff
	../bin/coff2noff testloop3.coff testloop3

testloop4.o: testloop4.c
	$(CC) $(INCDIR) -S testloop4.c -o testloop4.s
	$(AS) $(CFLAGS) testloop4.s -o testloop4.o
	rm -f testloop4.s
testloop4: testloop4.o start.o
	$(LD) $(LDFLAGS) start.o testloop4.o -o testloop4.coff
	../bin/coff2noff testloop4.coff testloop4

testloop5.o: testloop5.c
	$(CC) $(INCDIR) -S testloop5.c -o testloop5.s
	$(AS) $(CFLAGS) testloop5.s -o testloop5.o
	rm -f testloop5.s
testloop5: testloop5.o start.o
	$(LD) $(LDFLAGS) start.o testloop5.o -o testloop5.coff
	../bin/coff2noff testloop5.coff testloop5

testloop.o: testloop.c
	$(CC) $(INCDIR) -S testloop.c -o testloop.s
	$(AS) $(CFLAGS) testloop.s -o testloop.o
	rm -f testloop.s
testloop: testloop.o start.o
	$(LD) $(LDFLAGS) start.o testloop.o -o testloop.coff
	../bin/coff2noff testloop.coff testloop

testlooplong.o: testlooplong.c
	$(CC) $(INCDIR) -S testlooplong.c -o testlooplong.s
	$(AS) $(CFLAGS) testlooplong.s -o testlooplong.o
	rm -f testlooplong.s
testlooplong: testlooplong.o start.o
	$(LD) $(LDFLAGS) start.o testlooplong.o -o testlooplong.coff
	../bin/coff2noff testlooplong.coff testlooplong

longtest1.o: longtest1.c
	$(CC) $(INCDIR) -S longtest1.c -o longtest1.s
	$(AS) $(CFLAGS) longtest1.s -o longtest1.o
	rm -f longtest1.s
longtest1: longtest1.o start.o
	$(LD) $(LDFLAGS) start.o longtest1.o -o longtest1.coff
	../bin/coff2noff longtest1.coff longtest1

longtest2.o: longtest2.c
	$(CC) $(INCDIR) -S longtest2.c -o longtest2.s
	$(AS) $(CFLAGS) longtest2.s -o longtest2.o
	rm -f longtest2.s
longtest2: longtest2.o start.o
	$(LD) $(LDFLAGS) start.o longtest2.o -o longtest2.coff
	../bin/coff2noff longtest2.coff longtest2

clean:
	rm -f start.o halt.o halt shell.o shell sort.o sort matmult.o matmult halt.coff shell.coff sort.coff matmult.coff printtest.o printtest printtest.coff vectorsum.o vectorsum.coff vectorsum testregPA.o testregPA.coff testregPA forkjoin.o forkjoin.coff forkjoin testexec.o testexec.coff testexec testyield.o testyield.coff testyield forkjoin_hard forkjoin_hard.o forkjoin_hard.coff testloop1 testloop1.o testloop1.coff testloop2 testloop2.o testloop2.coff testloop3 testloop3.o testloop3.coff testloop4 testloop4.o testloop4.coff testloop5 testloop5.o testloop5.coff testlooplong testlooplong.o testlooplong.coff testloop testloop.o testloop.coff longtest1.o longtest1.coff longtest2.o longtest2.coff
