SHELL=/bin/sh

cc = $(shell basename $(CURDIR))
sys = $(shell cmd/leda.sys)
instdir = /LEDA/INSTALL/$(sys)/$(cc)

default: lib

kernel:
	./kernel_config
	$(MAKE) lib xlman

all: lib tests

install: FORCE
	cp -r -L incl /LEDA/INSTALL/$(sys)
	if [ -f leda.lib   ]; then cp leda.lib $(instdir); fi
	if [ -f leda.dll   ]; then cp leda.dll $(instdir); fi
	if [ -f libleda.a  ]; then cp libleda.a $(instdir); fi
	if [ -f libleda.so ]; then cp libleda.so $(instdir); fi

FORCE: lib

#------------------------------------------------------------------------------
# libraries
#------------------------------------------------------------------------------

lib: .license
	@if [ -f .license -a -d src  ]; then $(MAKE) -C src;      fi
	@if [ -f .license -a -d src1 ]; then $(MAKE) -C src1;     fi
	@if [ -f .license -a -f closelib  ]; then ./closelib;     fi
	@if [ -f .license -a -f static.mk ]; then $(MAKE) static; fi
	@if [ -f .license -a -f shared.mk ]; then $(MAKE) shared; fi

touch: 
	@if [ -f .license -a -d src  ]; then $(MAKE) -C src   touch; fi
	@if [ -f .license -a -d src1 ]; then $(MAKE) -C src1  touch; fi
	@if [ -f .license -a -f static.mk ]; then $(MAKE) static; fi
	@if [ -f .license -a -f shared.mk ]; then $(MAKE) shared; fi

nogui: .license
	mv libleda.a libleda.a.gui
	mv libleda.so libleda.so.gui
	mv src/graphics src/graphics_
	mv src1/graphics src1/graphics_
	$(MAKE) touch
	mv src/graphics_ src/graphics
	mv src1/graphics_ src1/graphics
	mv libleda.a libleda.so no_gui
	mv libleda.a.gui libleda.a
	mv libleda.so.gui libleda.so

gui: .license
	$(MAKE) touch


lib0: .license
	@if [ -f .license -a -d src  ]; then $(MAKE) -C src; fi
	@if [ -f .license -a -f closelib  ]; then ./closelib; fi
	@if [ -f .license -a -f static.mk ]; then $(MAKE) static; fi
	@if [ -f .license -a -f shared.mk ]; then $(MAKE) shared; fi

lib1: .license
	@if [ -f .license -a -d src1 ]; then $(MAKE) -C src1; fi
	@if [ -f .license -a -f closelib  ]; then ./closelib; fi
	@if [ -f .license -a -f static.mk ]; then $(MAKE) static; fi
	@if [ -f .license -a -f shared.mk ]; then $(MAKE) shared; fi

libL1:
	cp libL.a libL1.a
	mv src/system/_leda.o src/system/_leda_orig.o
	$(MAKE) -C src/system  -i L=L1 DFLAGS=-DLEDA_CHECK_LICENSE
	mv src/system/_leda_orig.o src/system/_leda.o

agd:
	$(MAKE) -C AGD 
	@if [ -f AGD/static.mk ]; then $(MAKE) -f AGD/static.mk; fi
	@if [ -f AGD/shared.mk ]; then $(MAKE) -f AGD/shared.mk; fi


shared: .license
	@if [ -f .license ]; then \
	if [ -f shared.mk ]; then \
	echo "$(MAKE) -f shared.mk"; $(MAKE) -f shared.mk; \
	if [ -f libAGD.a ]; then \
	echo "$(MAKE) -f shared.mk agd";    $(MAKE) -f shared.mk agd; fi; \
	else echo "Not configured to build shared libs."; fi; fi

static: .license
	@if [ -f .license ]; then $(MAKE) -f static.mk; fi


.license:
	@/bin/sh confdir/util/unix/license.sh


#------------------------------------------------------------------------------
# programs
#------------------------------------------------------------------------------

xlman: .license
	@if [ -f .license ]; then \
	echo "$(MAKE) -C demo/xlman "; $(MAKE) -C demo/xlman; fi

static_xlman: .license
	@if [ -f .license ]; then \
	echo "$(MAKE) -C demo/xlman "; $(MAKE) -C demo/xlman  static_xlman; fi

demos: .license
	@if [ -f .license ]; then \
	if [ -d demo ]; then $(MAKE) -C demo; fi; fi

tests: .license
	@if [ -f .license ]; then \
	if [ -d test ]; then $(MAKE) -C test; fi; fi

#------------------------------------------------------------------------------
# manual
#------------------------------------------------------------------------------

man: .license
	@if [ -f .license ]; then \
	echo "$(MAKE) -C Manual/MANUAL ";\
	$(MAKE) -C Manual/MANUAL; fi

pdfman: .license
	@if [ -f .license ]; then \
	echo "$(MAKE) -C Manual/MANUAL  pdf";\
	$(MAKE) -C Manual/MANUAL  pdf; fi

dvi: .license
	@if [ -f .license ]; then \
	echo "$(MAKE) -C Manual/MANUAL  dvi";\
	$(MAKE) -C Manual/MANUAL  dvi; fi



#------------------------------------------------------------------------------
# cleaning up
#------------------------------------------------------------------------------

del:
	@if [ -d src   ]; then $(MAKE) -C src    clean; fi
	@if [ -d src1  ]; then $(MAKE) -C src1   clean; fi
	@if [ -d prog  ]; then $(MAKE) -C prog   del; fi
	@if [ -d test  ]; then $(MAKE) -C test   del; fi
	@if [ -d demo  ]; then $(MAKE) -C demo   del; fi
	rm -f lib*.a lib*.so lib*.sl lib*.lib leda.dll leda.lib

clean:
	@if [ -d src   ]; then $(MAKE) -C src    clean; fi
	@if [ -d src1  ]; then $(MAKE) -C src1   clean; fi
	@if [ -d prog  ]; then $(MAKE) -C prog   clean; fi
	@if [ -d test  ]; then $(MAKE) -C test   clean; fi
	@if [ -d demo  ]; then $(MAKE) -C demo   clean; fi

