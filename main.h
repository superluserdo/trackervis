#define NATIVE_RES_X 1366 
#define NATIVE_RES_Y 768
#define MAX_SOUNDS_LIST 10

/*		Structs		*/

struct grid_struct {
	int x;
	int y;
} grid;


/* Status of the Visualiser */

struct vis_struct {
	int over;
};


/* Whole Program */

struct program_struct {
	int width, height;
};


int visualise(SDL_Window *win, SDL_Renderer *renderer, char *eventsFilename);
