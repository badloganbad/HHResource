/*
 * player.h
 *
 *  Created on: Jul 27, 2016
 *      Author: Phillipbadgett
 */

#ifndef HHRESOURCE_SOURCES_PLAYER_H_
#define HHRESOURCE_SOURCES_PLAYER_H_

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

class Player {
public:

	string DIR; // = images_dir + "player.png";
	SDL_Point Center;
	SDL_Rect Pos;
	SDL_Texture * PlayerIMAGE;
	bool flipped; // = false;
	double angle; // = 0.0;
	float bob; // = 0;
	//update floats for precision
	float pos_X;	// = 0.0f;
	float pos_Y;	// = 320.0f;
	Player(SDL_Renderer * renderer, string dir, int x, int y) {
		//PLAYER STUFF
		//player setup
		DIR = dir + "player.png";
		Pos.x = x; // 0
		Pos.y = y; // 320
		Pos.w = 128;
		Pos.h = 128;
		Center.x = 64;
		Center.y = 64;
		PlayerIMAGE = IMG_LoadTexture(renderer, DIR.c_str());

		flipped = false;
		angle = 0;
		bob = 0;
		pos_X = x; // 0
		pos_Y = y; //320

		//END PLAYER STUFF
	}

	void update(float deltaTime) {
		//Update player position code to account for precision loss
		Pos.x = (int) (pos_X + 0.5f);
		Pos.y = (int) (pos_Y + 0.5f);
		//for bobbing effect
		bob += 3 * deltaTime;
		if (bob > 360)
			bob = 0;
		Pos.y += (int) 5 * sin(bob);
		angle *= .99;

	}

	void draw(SDL_Renderer * renderer) {
		if (flipped)
			SDL_RenderCopyEx(renderer, PlayerIMAGE, NULL, &Pos, angle,
					&Center, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopyEx(renderer, PlayerIMAGE, NULL, &Pos, angle,
					&Center, SDL_FLIP_NONE);

	}

};

#endif /* HHRESOURCE_SOURCES_PLAYER_H_ */
