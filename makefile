output: main.o vis.o
	gcc -g3 main.o vis.o -pthread -lm -o ./trackervis -Wall -std=c99 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lconfig

main.o: main.c
	gcc -g3 -c main.c -std=c99

vis.o: vis.c main.h vis.h 
	gcc -g3 -c vis.c -std=c99

clean:
	rm -f *.o
