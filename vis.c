#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <libconfig.h>
#include "libconfig.h"
#include <math.h>
#include "main.h"
#include "vis.h"
#include "SDL2_gfxPrimitives.h"

int num_modules = 30;

struct num_struct num = {
	.modules = 3,
	.layers = 4,
	.straws = 32,
};

int generate_tally(int tally_array[num.modules][num.layers][num.straws], int colour_array[num.modules][num.layers][num.straws], config_setting_t *TE_setting, int Ntrackevents);

/*	Global Variables	*/

int change = 0;
int alt = 0;
int tally_mode = 0;
int mousedown = 0;
float topbotslider = 0;
float stereodiff;
float Yvalue = 0;
int alter_Y = 1;


extern struct program_struct program;

int visualise (SDL_Window *win, SDL_Renderer *renderer, char *eventsFilename) {

	/*	Some Variables	*/

	int screen_centre[2] = {program.width/2, program.height/2};
	float diagram_zoom = 20.0;	
	float diam;
	float strawgeometry[num.modules][num.layers][num.straws][2][3];
							/*					 ^  ^			*/
							/* bottom/top		_|  |_ x/y/z	*/
	float XZrel[num.modules][num.layers][num.straws][2][2];
	float diagram_centre[2];
	float diagram_relative[2];
	geom_init(strawgeometry, XZrel, &diam, diagram_centre, &stereodiff, &eventsFilename);
	int canvas_offset[2] = {0};
	int tally_array[num.modules][num.layers][num.straws];
	int colour_array[num.modules][num.layers][num.straws];

	for (int module = 0; module < num.modules; module++) {
		for (int layer = 0; layer < num.layers; layer++) {
			for (int straw = 0; straw < num.straws; straw++) {
				tally_array[module][layer][straw] = 0;
			}
		}
	}

/* Status of the Visualiser */

struct vis_struct vis = {
	.over = 0,
};




	/* 	Initialisation	*/

	/* Setup Config File */
	config_t cfg;
	config_setting_t *setting;
	const char *str;

	config_init(&cfg);
	/* Read the file. If there is an error, report it and exit. */
	if(! config_read_file(&cfg, eventsFilename))
	{
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
		config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return(EXIT_FAILURE);
	}

	config_setting_t *TE_setting = config_lookup(&cfg, "trackevents");
	int eventindex = 0;
	int Ntrackevents = config_setting_length(TE_setting);
	printf("%d track events examined\n", Ntrackevents);

	/*	Generate Tallied-Up Straw View in a Different Thread	*/

	generate_tally(tally_array, colour_array, TE_setting, Ntrackevents);


	/*	Print Instructions	*/

	printf(
		"		CONTROLS:\n"
		"	------------------------------------\n"
		"	ESC		Quit program.\n"
		"	UP/DN Arrows	Zoom in/out.\n"
		"	Scroll wheel	Zoom in/out.\n"
		"	ALT + scroll	Adjust Y value.\n"
		"	Click +	drag	Move display around.\n"
		"	L/R Arrows	Change event number.\n"
		"	T		Toggle heat tally.\n"
		"	------------------------------------\n"
			);


	/*	Text Information	*/


	int count = 2;

	struct coord coords[count];
	
	if ( TTF_Init( ) == -1 ) {

		fprintf(stderr, "Init TTF  failed\n");
		//return -1;
	} 
	
	char *fontpath = "DejaVuSans.ttf";
	TTF_Font *dejavu = TTF_OpenFont(fontpath,300);
	if (dejavu == NULL) {
		printf("Could not find required font, %s.\n", fontpath);
		quitvis(renderer, count, dejavu);
		return 1;
	}		

	SDL_SetRenderTarget(renderer, NULL);

	SDL_Surface *textsurfaces[count];
	SDL_Texture *texttextures[count];

	textupdate(renderer, count, Yvalue, textsurfaces, texttextures, dejavu, coords);


	/* Create a Master Render Target */

	SDL_Texture *texTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, program.width, program.height);



	/* Frame Loop */

	while (1) {


		if (vis.over) {
			quitvis(renderer, count, dejavu);
			return 0;
		}


		/* Handle Events */

		SDL_Event e;
		while ( SDL_PollEvent(&e) ) {

			change = 1;
			if (e.type == SDL_QUIT) {
				vis.over = 1;
				quitvis(renderer, count, dejavu);
				return 102; /* Quit to desktop" code */
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				//vis.over = 1;
				quitvis(renderer, count, dejavu);
				return 0; /* Quit up one level */
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP) {
				diagram_zoom *= 1.1;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN) {
				diagram_zoom /= 1.1;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LEFT) {
				alter_Y = 1;
				eventindex--;
				if (eventindex <= 0)
					eventindex = 0;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RIGHT) {
				alter_Y = 1;
				eventindex++;
				if (eventindex >= Ntrackevents)
					eventindex = Ntrackevents - 1;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LALT) {
				alt = 1;
			}
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_LALT) {
				alt = 0;
			}
			else if (e.type == SDL_MOUSEWHEEL) {
				if (alt) {
					if (e.wheel.y > 0) {	
						Yvalue += 0.65;
						if (Yvalue >= 4.55) {
							Yvalue = 4.55;
						}
					}
					else if (e.wheel.y < 0) {	
						Yvalue -= 0.65;
						if (Yvalue <= -4.55) {
							Yvalue = -4.55;
						}
					}
					textupdate(renderer, count, Yvalue, textsurfaces, texttextures, dejavu, coords);
				}
				else {
					if (e.wheel.y > 0) {	
						diagram_zoom *= 1.1;
					}
					else if (e.wheel.y < 0) {
						diagram_zoom /= 1.1;
					}
				}
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_t) {
				tally_mode ^= 1;
				printf("Tally mode: %s\n", tally_mode == 1? "on" : "off");
			}	
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				mousedown = 1;
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
				mousedown = 0;
			}

			if (e.type == SDL_MOUSEMOTION && mousedown) {
				canvas_offset[0] += e.motion.xrel;
				canvas_offset[1] += e.motion.yrel;
			}
		}

		/* The real business */

		if (change) {
	
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
			if (SDL_RenderClear(renderer) < 0) {
				printf("SDL_RenderClear failed. Error message:	%s\n", SDL_GetError());
			}
	
			config_setting_t *event_setting = config_setting_get_elem(TE_setting, eventindex);
			double tmp;
			if (alter_Y) {
				config_setting_lookup_float(event_setting, "Ybest", &tmp);
				Yvalue = (float)tmp;
				textupdate(renderer, count, Yvalue, textsurfaces, texttextures, dejavu, coords);
			}
	
			if (!tally_mode) {

				/*	Draw Straw Geometry	*/
				
				for (int imodule = 0; imodule < num.modules; imodule ++) {
					for (int ilayer = 0; ilayer < num.layers; ilayer ++) {
						for (int istraw = 0; istraw < num.straws; istraw++) {
							for (int topbot = 0; topbot < 1; topbot++) {
		
								float x = XZrel[imodule][ilayer][istraw][topbot][1] * diagram_zoom + program.width/2 + canvas_offset[0];
								float y;
								if (ilayer == 0 || ilayer == 1)
									y = - (XZrel[imodule][ilayer][istraw][topbot][0] + stereodiff * (1 + Yvalue/(4.55)) * 0.5) * diagram_zoom + program.height/2 + canvas_offset[1];
								else if (ilayer == 2 || ilayer == 3)
									y = - (XZrel[imodule][ilayer][istraw][topbot][0] - stereodiff * (1 + Yvalue/(4.55)) * 0.5) * diagram_zoom + program.height/2 + canvas_offset[1];
								float radius = diam * diagram_zoom / 2.0;
								Sint16 xint = (int)x;
								Sint16 yint = (int)y;
								Sint16 radiusint = (int)radius;
									aacircleColor(renderer, xint, yint, radiusint, 0xFFFFFFFF);
		
							}
						}
					}
				}
		

				for (int i = 0; i < num.layers; i++) {
	
						/*	Highlight Hit Straws	*/
	
						float Xrel, Zrel, x, y;
						Sint16 xint, yint; 
		
						config_setting_t *hits_setting = config_setting_lookup(event_setting, "hits");
						if (!hits_setting) {
							printf("No data entry in '%s' for event #%d. Quitting.\n", eventsFilename, i + 1);
							return 1;
						}
						config_setting_t *hit_setting = config_setting_get_elem(hits_setting, i);
						if (!hit_setting) {
							printf("No data entry in '%s' for layer %d in event #%d. Quitting.\n", eventsFilename, i + 1, eventindex + 1);
							return 1;
						}
						int module, layer, straw;
						config_setting_lookup_int(event_setting, "module", &module); 
						config_setting_lookup_int(hit_setting, "layer", &layer);
						config_setting_lookup_int(hit_setting, "straw", &straw);
						module--;
						layer--;
						straw--;
						x = XZrel[module][layer][straw][0][1] * diagram_zoom + program.width/2 + canvas_offset[0];
						if (layer == 0 || layer == 1)
							y = - (XZrel[module][layer][straw][0][0] + stereodiff * (1 + Yvalue/(4.55)) * 0.5) * diagram_zoom + program.height/2 + canvas_offset[1];
						else if (layer == 2 || layer == 3)
							y = - (XZrel[module][layer][straw][0][0] - stereodiff * (1 + Yvalue/(4.55)) * 0.5) * diagram_zoom + program.height/2 + canvas_offset[1];
						float radius = diam * diagram_zoom / 2.0;
						xint = (int)x;
						yint = (int)y;
						Sint16 radiusint = (int)radius;
		
						filledCircleColor(renderer, xint, yint, radiusint, 0xFFBBFFFF);
		
						/*	Draw Actual Hit Points	*/
	
						config_setting_lookup_float(hit_setting, "X", &tmp);
						Xrel = (float)tmp - diagram_centre[1];
						config_setting_lookup_float(hit_setting, "Z", &tmp);
						Zrel = (float)tmp - diagram_centre[0];
						x = Zrel * diagram_zoom + program.width/2 + canvas_offset[0];
						y = - Xrel * diagram_zoom + program.height/2 + canvas_offset[1];
						xint = (int)x;
						yint = (int)y;
						filledCircleColor(renderer, xint, yint, 2, 0xFF0000FF);
		
				}
	
				/*	Draw Best Fit Line For Track Event	*/
	
				float Z1, X1, Z2, X2, X1rel, X2rel, Z1rel, Z2rel, x1, x2, y1, y2;
				int x1int, x2int, y1int, y2int;
		
				config_setting_t *line_setting = config_setting_lookup(event_setting, "line");
				config_setting_lookup_float(line_setting, "Z1", &tmp);
				Z1 = (float)tmp;
				config_setting_lookup_float(line_setting, "X1", &tmp);
				X1 = (float)tmp;
				config_setting_lookup_float(line_setting, "Z2", &tmp);
				Z2 = (float)tmp;
				config_setting_lookup_float(line_setting, "X2", &tmp);
				X2 = (float)tmp;
		
				Z1rel = Z1 - diagram_centre[0];
				Z2rel = Z2 - diagram_centre[0];
				X1rel = X1- diagram_centre[1];
				X2rel = X2 - diagram_centre[1];
				x1 = Z1rel * diagram_zoom + program.width/2 + canvas_offset[0];
				x2 = Z2rel * diagram_zoom + program.width/2 + canvas_offset[0];
				y1 = - X1rel * diagram_zoom + program.height/2 + canvas_offset[1];
				y2 = - X2rel * diagram_zoom + program.height/2 + canvas_offset[1];
				x1int = (int)x1;
				y1int = (int)y1;
				x2int = (int)x2;
				y2int = (int)y2;
		
				thickLineRGBA(renderer, x1, y1, x2, y2, 3, 0, 255, 0, 255);
				circleColor(renderer, program.width/2, program.height/2, 3, 0xFF00FFFF);
				for (int index = 0; index < count; index++) 
					SDL_RenderCopy(renderer, texttextures[index], NULL, &coords[index].coord_rect);
			}	

			else if (tally_mode) {
				for (int imodule = 0; imodule < num.modules; imodule ++) {
					for (int ilayer = 0; ilayer < num.layers; ilayer ++) {
						for (int istraw = 0; istraw < num.straws; istraw++) {
							for (int topbot = 0; topbot < 1; topbot++) {
		
								float x = XZrel[imodule][ilayer][istraw][topbot][1] * diagram_zoom + program.width/2 + canvas_offset[0];
								float y;
								if (ilayer == 0 || ilayer == 1)
									y = - (XZrel[imodule][ilayer][istraw][topbot][0] + stereodiff * (1 + Yvalue/(4.55)) * 0.5) * diagram_zoom + program.height/2 + canvas_offset[1];
								else if (ilayer == 2 || ilayer == 3)
									y = - (XZrel[imodule][ilayer][istraw][topbot][0] - stereodiff * (1 + Yvalue/(4.55)) * 0.5) * diagram_zoom + program.height/2 + canvas_offset[1];
								float radius = diam * diagram_zoom / 2.0;
								Sint16 xint = (int)x;
								Sint16 yint = (int)y;
								Sint16 radiusint = (int)radius;
								int colour = colour_array[imodule][ilayer][istraw];
								filledCircleRGBA(renderer, xint, yint, radiusint, 255, colour, colour, 255);
		
							}
						}
					}
				}
			}

		}
		SDL_RenderPresent(renderer);

//		SDL_SetRenderTarget(renderer, texTarget);
		alter_Y = 0;
		SDL_Delay(16);
		change = 0;
	}

		quitvis(renderer, count, dejavu);



	return 0;
}


/*Functions*/


void quitvis(SDL_Renderer *renderer, int count, TTF_Font *dejavu) {

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 50);
	if (dejavu) {
		TTF_CloseFont(dejavu);
	}
	SDL_RenderClear(renderer);
}


int geom_init(float strawgeometry[num.modules][num.layers][num.straws][2][3], float XZrel[num.modules][num.layers][num.straws][2][2], float *diam, float diagram_centre[2], float *stereodiff, char **eventsFilenamePtr) {
		
	*diam = 0.5;
	float theta = 7.5*PI/180;
	float wdist = 0.6/(cos(theta));
	float Xoff1 = 0.0; /* Allow for the module to be offset in X */
	float Ybot = -4.55;
	float Ytop = 4.55;
	float Zpos = 10.0;
	float Zmoddiff = 13.0;
	float layeroffsets_Z[4] = {3.931, 4.408, 6.091, 6.568};
	float layeroffsets_X[4] = {4.7984, 4.7984 + wdist/2, 6.378, 6.378 - wdist/2};
	*stereodiff = 9.1 * tan(theta);

	float Xmid = Xoff1 + layeroffsets_X[1] + 16 * wdist;
	float Zmid = Zpos + Zmoddiff * 1 + layeroffsets_Z[1];
	diagram_centre[0] = Zmid;
	diagram_centre[1] = Xmid;
	/* NOTE: in GM2 coordinates, "Z" is along beam line,
	 * which is drawn HORIZONTALLY here!
	 * Likewise, "X" is orthogonal, and is drawn vertically
	 * on the screen!
	 *
	 * I am using capital notation (X, Y, Z) for beamline
	 * coordinates and lowercase (x, y) for program
	 * coordinates.
	 */

	for (int imodule = 0; imodule < num.modules; imodule ++) {
		for (int ilayer = 0; ilayer < num.layers; ilayer ++) {
			for (int istraw = 0; istraw < num.straws; istraw++) {

					/*	X	*/

					strawgeometry[imodule][ilayer][istraw][0][0] = Xoff1  + layeroffsets_X[ilayer] + istraw * wdist;
					if (ilayer == 0 || ilayer == 1)
						strawgeometry[imodule][ilayer][istraw][1][0] = Xoff1  + layeroffsets_X[ilayer] + istraw * wdist + *stereodiff;
					else
						strawgeometry[imodule][ilayer][istraw][1][0] = Xoff1  + layeroffsets_X[ilayer] + istraw * wdist - *stereodiff;

					XZrel[imodule][ilayer][istraw][0][0] = strawgeometry[imodule][ilayer][istraw][0][0] - Xmid;
					XZrel[imodule][ilayer][istraw][1][0] = strawgeometry[imodule][ilayer][istraw][1][0] - Xmid;
					/*	Y	*/
					
					strawgeometry[imodule][ilayer][istraw][0][1] = Ybot;
					strawgeometry[imodule][ilayer][istraw][1][1] = Ytop;
					
					/*	Z	*/
					
					strawgeometry[imodule][ilayer][istraw][0][2] = Zpos + Zmoddiff * imodule + layeroffsets_Z[ilayer];
					strawgeometry[imodule][ilayer][istraw][1][2] = Zpos + Zmoddiff * imodule + layeroffsets_Z[ilayer];

					XZrel[imodule][ilayer][istraw][0][1] = strawgeometry[imodule][ilayer][istraw][0][2] - Zmid;
					XZrel[imodule][ilayer][istraw][1][1] = strawgeometry[imodule][ilayer][istraw][1][2] - Zmid;
			}
		}
	}
}			

int textupdate(SDL_Renderer *renderer, int count, float Yvalue, SDL_Surface *textsurfaces[count], SDL_Texture *texttextures[count], TTF_Font *dejavu, struct coord coords[count]) {

	char buf[256];

	for (int i = 0; i < count; i++) {

		snprintf(buf, 256, "Y = %.2f cm", Yvalue);
		coords[i].text = buf;
		coords[i].length = strlen(buf);
		coords[i].coord_rect.w = 30 * coords[0].length;
		coords[i].coord_rect.h= 50;
	}
		coords[0].coord_rect.x = program.width - coords[0].coord_rect.w;
		coords[0].coord_rect.y = program.height - coords[0].coord_rect.h;

		coords[1].coord_rect.x = 0;//program.width - coords[1].coord_rect.w;
		coords[1].coord_rect.y = program.height - coords[1].coord_rect.h;

		SDL_Color fg;
		fg.r = 100;
		fg.g = 100;
		fg.b = 100;

	for (int i = 0; i < count; i++) {
		textsurfaces[i] = TTF_RenderText_Solid(dejavu, coords[i].text, fg);
		if (textsurfaces[i] == NULL) {
			printf("Could not render text to surface\n");
		}

		texttextures[i] = SDL_CreateTextureFromSurface(renderer, textsurfaces[i]);

	}

	return 0;
}

int generate_tally(int tally_array[num.modules][num.layers][num.straws], int colour_array[num.modules][num.layers][num.straws], config_setting_t *TE_setting, int Ntrackevents) {

	int total = 0;
	for (int i = 0; i < Ntrackevents; i++) {
		config_setting_t *event_setting = config_setting_get_elem(TE_setting, i);
		config_setting_t *hits_setting = config_setting_lookup(event_setting, "hits");
		for (int j = 0; j < config_setting_length(hits_setting); j++) {
			config_setting_t *hit_setting = config_setting_get_elem(hits_setting, j);
			int module, layer, straw;
			config_setting_lookup_int(event_setting, "module", &module); 
			config_setting_lookup_int(hit_setting, "layer", &layer);
			config_setting_lookup_int(hit_setting, "straw", &straw);
			module--;
			layer--;
			straw--;
			tally_array[module][layer][straw]++;
			total++;
		}
	}
	int maxstraw;
	for (int module = 0; module < num.modules; module++) {
		for (int layer = 0; layer < num.layers; layer++) {
			for (int straw = 0; straw < num.straws; straw++) {
				if (tally_array[module][layer][straw] > maxstraw)
					maxstraw = tally_array[module][layer][straw];
			}
		}
	}
	float ratio = 255.0/maxstraw;
	for (int module = 0; module < num.modules; module++) {
		for (int layer = 0; layer < num.layers; layer++) {
			for (int straw = 0; straw < num.straws; straw++) {
				colour_array[module][layer][straw] = 255 - (int) (tally_array[module][layer][straw] * ratio);
			}
		}
	}
	printf("%d total hits belonging to track events\n\n", total);
}
