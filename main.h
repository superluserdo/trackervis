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

struct opts_struct {
	int onoffs[2];
	char *eventsFilename;
	struct grid_struct res;
};


int visualise (SDL_Window *win, SDL_Renderer *renderer, struct opts_struct *opts);
