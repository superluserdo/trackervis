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

int visualise(SDL_Window *win, SDL_Renderer *renderer);
int startscreen(SDL_Window *win, SDL_Renderer *renderer);
void quitstart(SDL_Texture *startimgbg, SDL_Texture *startimgtext1, SDL_Texture *startimgtext2, SDL_Renderer *renderer);

int main() {

	printf(
		"--------------------------------------------------------\n"
		"|	G-2 Track Events Visualiser (Unfinished)	|\n"
		"--------------------------------------------------------\n"
		"tomh422826@gmail.com\n"
		"\n"
		"Libraries:		SDL2		dynamic\n"
		"			SDL2_gfx	static\n"
		"			libconfig	static\n"
		"--------------------------------------------------------\n");
	SDL_Window *win = NULL;
        SDL_Renderer *renderer = NULL;
	// Initialize SDL.
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                        return 1;

	// start SDL with audio support
	if(SDL_Init(SDL_INIT_AUDIO)==-1) {
	    printf("SDL_Init: %s\n", SDL_GetError());
	    exit(1);
	}

        win = SDL_CreateWindow("Tom's G-2 Track Visualiser", 100, 100, program.width, program.height, 0);
	//renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED);
	renderer = SDL_CreateRenderer(win, -1, 0);
	if (renderer == NULL) {
		printf("SDL_CreateRenderer failed. Error message: '%s\n'", SDL_GetError());
	
		SDL_RendererInfo info;
		int num = SDL_GetNumRenderDrivers();
		printf("There are %d usable render drivers\n", num);	
		printf("Driver	SDL_RendererFlags\n");
		for (int i = 0; i < num; i++) {
			if (SDL_GetRenderDriverInfo(i,&info) == 0)
				printf("%s	%d\n", info.name, info.flags);
		}
	}

	int returncode = visualise(win, renderer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	printf("Quitting...\n");
	return 0;
}
