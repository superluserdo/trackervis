output: main.o vis.o
	gcc main.o vis.o -pthread -lm -L/user/tomh/straws/trackervis/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lconfig -o ./trackervis -Wall -std=c99 -rpath=/user/tomh/straws/trackervis/lib

main.o: main.c
	gcc -c main.c -std=c99

vis.o: vis.c main.h vis.h 
	gcc -c vis.c -std=c99

clean:
	rm -f *.o
