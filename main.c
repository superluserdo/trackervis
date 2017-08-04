#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "main.h"
#include "libconfig.h"
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

	/* Check for program conf file, use defaults if it doesn't exist */

	struct opts_struct opts;

	config_t conf;
	config_init(&conf);
	int defaults = 0;

	if (config_read_file(&conf, "trackervis.conf")) {

		config_setting_t *onoffs = config_lookup(&conf, "onoffs");
		if (onoffs) {
			config_setting_lookup_int(onoffs, "Yvalue", &opts.onoffs[0]);
			if (opts.onoffs[0] != 0 && opts.onoffs[0] != 1) {
				defaults = 2;
			}
			config_setting_lookup_int(onoffs, "eventindex", &opts.onoffs[1]);
			if (opts.onoffs[1] != 0 && opts.onoffs[1] != 1) {
				defaults = 2;
			}
		}
		else {
			defaults = 2;
		}
	}
	else {
		defaults = 1;
	}
	if (defaults > 0) {
		if (defaults == 1) {
			printf("Couldn't find program config file 'trackervis.conf'.\n"
					"Resorting to defaults.\n\n");
		}
		else if (defaults = 2) {
			printf("Something isn't right about the config file trackervis.conf.\n"
					"Resorting to defaults.\n\n");
		}
		opts.onoffs[0] = 1;
		opts.onoffs[1] = 1;
	}
	/* Specify trackevents file and check if it exists */

	FILE *tevents;
	if (argc > 1) {
		opts.eventsFilename = argv[1];
	}
	else {
		opts.eventsFilename = "trackevents.txt";
	}

	tevents = fopen(opts.eventsFilename, "r");
	if (tevents == NULL) {
		printf( "Could not open track event file '%s'.\n"
				"You need the file 'trackevents.txt' in the current folder,\n"
				"or specify a different file to read as an argument.\n\n",
				opts.eventsFilename);
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
	int returncode = visualise(win, renderer, &opts);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	printf("Quitting...\n");
	return returncode;
}
