#define ZOOM_MULT 2
//#define NATIVE_RES_X 640
#define NATIVE_RES_X 1366 
#define NATIVE_RES_Y 768
#define MAX_SOUNDS_LIST 10

/*		Structs		*/

struct grid_struct {
	int x;
	int y;
} grid;

/* Status Struct */

struct status_struct {
	struct vis_struct *vis;
	struct player_struct *player;
	struct audio_struct *audio;
	struct time_struct *timing;
};

/* Status of the Level */

struct vis_struct {
	int gameover;
	int over;
	int pauselevel;
	int currentlevel;
	struct grid_struct grid;
	int maxscreens;
	int totalnativedist;
	int partymode;
	float speedmult;
	float speedmultmon;
	int currentscreen;
	struct lane_struct *lanes;
};

struct lane_struct {
	int total;
	int currentlane;
	int lanewidth;
	int *laneheight;
};

/* Status of the Player */

struct player_struct {

	int HP, power;
	int max_HP;
	int max_PP;
	int invincibility;
	int invinciblecounter[2];
	int sword;
	int direction;
	int flydir;
};

/* Whole Program */

struct program_struct {
	int width, height;
	int score;
};

/* Audio */

struct audio_struct {

	int track;
	int newtrack;
	int noise;
	int soundchecklist[MAX_SOUNDS_LIST];
};

/* Timing */

struct time_struct {

	int countbeats;
	float bps;
	float startbeat;
	float currentbeat;
	float pxperbeat;
	int framecount;
	int fpsanim;
	int fpsglobal;
	int ticks;
	int zerotime;
	int pausetime;
	int *pauselevel;
	float intervalanim;
	float intervalglobal;
};

void *frametimer(void *);
int pausefunc(SDL_Renderer *renderer, SDL_Texture *levelcapture, int currentlevel);

int visualise(SDL_Window *win, SDL_Renderer *renderer);
int startscreen(SDL_Window *win, SDL_Renderer *renderer);

void quitstart(SDL_Texture *startimgbg, SDL_Texture *startimgtext1, SDL_Texture *startimgtext2, SDL_Renderer *renderer);

/* Return Code definitions
	0:	Return normally
	1-100:	Return normally and again x many times
	101:	Quit to startscreen from wherever
	102:	Quit to desktop from wherever
*/
