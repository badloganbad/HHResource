/*
 * bullet.h
 *
 *  Created on: Aug 3, 2016
 *      Author: Phillipbadgett
 */

#ifndef HHRESOURCE_SOURCES_BULLET_H_
#define HHRESOURCE_SOURCES_BULLET_H_

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

class Bullet {
public:
	string DIR;
	SDL_Rect Pos;
	SDL_Texture * IMAGE;
	SDL_Point Center;

	float rotation;
	float angle;
	bool active;
	bool spinning;
	float move_x, move_y;
	float pos_X, pos_Y; // for precision loss

	Bullet(SDL_Renderer * renderer, string dir, string file, int x, int y) {
		DIR = dir + file; // last thing you did was this
		Pos.x = x;
		Pos.y = y; // -728
		Pos.w = 32;
		Pos.h = 32;
		IMAGE = IMG_LoadTexture(renderer, DIR.c_str());
		pos_X = x;
		pos_Y = y;
		move_x = 0;
		move_y = 0;
		active = false;
		spinning = false;
		angle = 0;
		Center.x = 16;
		Center.y = 16;
		rotation = 0;
	}

	void update(float deltaTime) {

		move_x = 500 * cos(((180 + angle) / 180 * 3.14));
		move_y = 500 * sin(((180 + angle) / 180 * 3.14));
		pos_X += move_x * deltaTime;
		pos_Y += move_y * deltaTime;
		Pos.x = (int) (pos_X + 0.5f);
		Pos.y = (int) (pos_Y + 0.5f);

		rotation += 300 * deltaTime;

		//if the pumpkin goes off screen
		if (Pos.x > 1024 || Pos.y > 768 || Pos.x < 0 - Pos.w
				|| Pos.y < 0 - Pos.h)
			reset();
	}

	void draw(SDL_Renderer * renderer) {
		if (active) {

			if (spinning) {
				SDL_RenderCopyEx(renderer, IMAGE, NULL, &Pos, rotation, &Center,
						SDL_FLIP_NONE);
			} else {
				SDL_RenderCopy(renderer, IMAGE, NULL, &Pos);
			}

		}
	}

	inline void move(float modifier, int direction, float deltaTime) {
		if (direction == 0) //up
				{
			pos_Y -= ((300 - modifier) * deltaTime);
		} else if (direction == 1) //down
				{
			pos_Y += ((300 - modifier) * deltaTime);
		} else if (direction == 2) //left
				{
			pos_X -= ((300 - modifier) * deltaTime);
		} else if (direction == 3) //right
				{
			pos_X += ((300 - modifier) * deltaTime);
		}
	}

	void fire(float radians, SDL_Rect origin) {
		pos_X = origin.x + 58;
		pos_Y = origin.y;
		angle = radians;
		active = true;
	}

	void reset() {
		active = false;
		Pos.x = 0;
		Pos.y = 0;
		rotation = 0;
	}

};

#endif /* HHRESOURCE_SOURCES_BULLET_H_ */
