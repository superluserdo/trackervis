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

extern struct opts_struct opts;

struct coord {
	const char *text;
	int length;
	SDL_Rect coord_rect;
};

struct textupdate_struct {
	int eventindex;
	float Yvalue;
	int onoffs[2];
};

int visualise (SDL_Window *win, SDL_Renderer *renderer, struct opts_struct *opts);

int geom_init(float strawgeometry[num.modules][LAYERS][STRAWS][2][3], float XZrel[num.modules][LAYERS][STRAWS][2][2], float *diam, float diagram_centre[2], float *stereodiff);

int generate_tally(int tally_array[num.modules][num.layers][num.straws], int colour_array[num.modules][num.layers][num.straws], config_setting_t *TE_setting, int Ntrackevents);

int textupdate(SDL_Renderer *renderer, int count, struct textupdate_struct text_info, SDL_Surface *textsurfaces[count], SDL_Texture **texttextures, TTF_Font *dejavu, struct coord coords[count]);

void quitvis(SDL_Renderer *renderer, int count, TTF_Font *dejavu);

