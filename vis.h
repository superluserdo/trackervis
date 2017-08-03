#define IMAGE_PATH "image.png"
#define SPRITE_SIZEX 32
#define SPRITE_SIZEY 32
//#define num.modules 3
#define LAYERS 4
#define STRAWS 32
#define PI 3.1415926535

extern int num_modules;

struct num_struct {
	int modules;
	int layers;
	int straws;
};

extern struct num_struct num;

struct coord {
	const char *text;
	int length;
	SDL_Rect coord_rect;
};

int visualise (SDL_Window *win, SDL_Renderer *renderer, char *eventsFilename);

int geom_init(float strawgeometry[num.modules][LAYERS][STRAWS][2][3], float XZrel[num.modules][LAYERS][STRAWS][2][2], float *diam, float diagram_centre[2], float *stereodiff, char **eventsFilenamePtr);

int textupdate(SDL_Renderer *renderer, int count, float Yvalue, SDL_Surface *textsurfaces[count], SDL_Texture **texttextures, TTF_Font *dejavu, struct coord coords[count]);

void quitvis(SDL_Renderer *renderer, int count, TTF_Font *dejavu);

/* Threading */

extern pthread_mutex_t display_mutex;
extern pthread_mutex_t clock_mutex;
extern pthread_cond_t display_cond;
extern pthread_cond_t cond_end;
extern pthread_mutex_t track_mutex; 
