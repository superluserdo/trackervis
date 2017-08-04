#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "main.h"
//#include <libconfig.h>

//extern struct time_struct timing;
struct program_struct program = {
	.width = NATIVE_RES_X,
	.height = NATIVE_RES_Y
};

SDL_Renderer *renderer;
SDL_Window *win;


int main(int argc, char **argv) {

	printf(
		"--------------------------------------------------------\n"
		"|	G-2 Track Events Visualiser (Unfinished)	|\n"
		"--------------------------------------------------------\n"
		"tomh422826@gmail.com\n"
		"\n"
		"Libraries:		SDL2		dynamic\n"
		"			SDL2_gfx	static\n"
		"			libconfig	static\n"
		"--------------------------------------------------------\n\n");

	/* Specify trackevents file and check if it exists */

	FILE *tevents;
	char *eventsFilename;
	if (argc > 1) {
		eventsFilename = argv[1];
	}
	else {
		eventsFilename = "trackevents.txt";
	}

	tevents = fopen(eventsFilename, "r");
	if (tevents == NULL) {
		printf( "Could not open track event file '%s'.\n"
				"You need the file 'trackevents.txt' in the current folder,\n"
				"or specify a different file to read as an argument.\n\n",
				eventsFilename);
		return 1;
	}
	fclose(tevents);

	/* Initialise SDL, create window, renderer */

	SDL_Window *win = NULL;
        SDL_Renderer *renderer = NULL;
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                        return 1;

        win = SDL_CreateWindow("G-2 Track Visualiser", 100, 100, program.width, program.height, 0);
	renderer = SDL_CreateRenderer(win, -1, 0); /*Final argument is bitmask -- 0 means don't reject any renderer */
	if (renderer == NULL) {
		printf("SDL_CreateRenderer failed. Error message: '%s\n'", SDL_GetError());
	
		/* Print driver info if renderer creation fails */
		SDL_RendererInfo info;
		int num = SDL_GetNumRenderDrivers();
		printf("There are %d usable render drivers\n", num);	
		printf("Driver	SDL_RendererFlags\n");
		for (int i = 0; i < num; i++) {
			if (SDL_GetRenderDriverInfo(i,&info) == 0)
				printf("%s	%d\n", info.name, info.flags);
		}
	}

	/* Start the actual visualiser */
	int returncode = visualise(win, renderer, eventsFilename);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	printf("Quitting...\n");
	return returncode;
}
