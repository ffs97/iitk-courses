all: brute.c welzl.c time.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) brute.c -o brute -lleda -lX11 -lm
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) welzl.c -o welzl -lleda -lX11 -lm
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) time.c -o time -lleda -lX11 -lm
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) points.c -o points -lleda -lX11 -lm

brutemake: brute.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) brute.c -o brute -lleda -lX11 -lm

welzlmake: welzl.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) welzl.c -o welzl -lleda -lX11 -lm

timemake: time.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) time.c -o time -lleda -lX11 -lm

pointsmake: points.c
	g++ -I$(LEDAROOT)/incl/ -L$(LEDAROOT) points.c -o points -lleda -lX11 -lm
