output: main.o vis.o
	gcc main.o vis.o -pthread -lm -L'./lib' -Wl,-rpath='$$ORIGIN/lib' -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lconfig -o ./trackervis -Wall -std=c99


main.o: main.c
	gcc -c main.c -std=c99

vis.o: vis.c main.h vis.h 
	gcc -c vis.c -std=c99

clean:
	rm -f *.o
