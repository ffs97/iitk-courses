LEDAROOT="$(HOME)/Documents/iitk-course-content/sem5/cs345/assignments/special-assign/leda"
LD_LD_LIBRARY_PATH="$(LEDAROOT)"

all: brutemake welzlmake timemake pointsmake
	
brutemake: brute.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) brute.c -o brute -lleda -lX11 -lm

welzlmake: welzl.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) welzl.c -o welzl -lleda -lX11 -lm

timemake: time.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) time.c -o time -lleda -lX11 -lm

pointsmake: points.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) points.c -o points -lleda -lX11 -lm
