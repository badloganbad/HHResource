/*
 * scene.h
 *
 *  Created on: Jul 27, 2016
 *      Author: Phillipbadgett
 */

#ifndef HHRESOURCE_SOURCES_SCENE_H_
#define HHRESOURCE_SOURCES_SCENE_H_

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDl_image.h"
#include <direct.h>
#define getcwd _getcwd
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#include <stdio.h>

using namespace std;

class Scenery {
public:
	//MIDGROUND2 STUFF
	//mdgd things
	string DIR;		// = images_dir + "midground2.png";
	SDL_Rect Pos;

	SDL_Texture * IMAGE;

	float pos_X, pos_Y;// = 0, m2_Y = -728;
	//END MIDGROUND2 STUFF

	Scenery(SDL_Renderer * renderer, string dir, string file, int x, int y) {

		DIR = dir + file; // last thing you did was this
		Pos.x = x;
		Pos.y = y; // -728
		Pos.w = 3072;
		Pos.h = 2304;
		IMAGE = IMG_LoadTexture(renderer, DIR.c_str());
		pos_X = x;
		pos_Y = y;

	}

	void update() {
		Pos.x = (int)(pos_X + 0.5f);
		Pos.y = (int)(pos_Y + 0.5f);
	}

	void draw(SDL_Renderer * renderer) {
		SDL_RenderCopy(renderer, IMAGE, NULL, &Pos);
	}
};

#endif /* HHRESOURCE_SOURCES_SCENE_H_ */
