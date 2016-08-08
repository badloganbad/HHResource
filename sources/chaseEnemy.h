/*
 * chaseEnemy.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Phillipbadgett
 */

#ifndef HHRESOURCE_SOURCES_CHASEENEMY_H_
#define HHRESOURCE_SOURCES_CHASEENEMY_H_

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

class ChaseEnemy {
public:
	string DIR; // = images_dir + "player.png";
	SDL_Point Center;
	SDL_Rect Pos;
	SDL_Texture * image;
	int health;
	float angle;
	bool active;
	bool flipped;
	float pos_X, pos_Y;
	float attackTime;
	const float attackRate = 1000.0f;

	ChaseEnemy(SDL_Renderer * renderer, string dir, int x, int y) {
		DIR = dir + "skull.png";
		Pos.x = x; // 0
		Pos.y = y; // 320
		Pos.w = 32;
		Pos.h = 32;
		Center.x = 16;
		Center.y = 16;
		image = IMG_LoadTexture(renderer, DIR.c_str());
		health = 5 + rand() % 5;
		active = true;
		angle = 0;
		flipped = false;
		pos_X = x;
		pos_Y = y;
		attackTime = 0;
	}

	void update(float deltaTime, SDL_Rect player) {
		if (health > 0) {
			if (250000
					> ((Pos.x - player.x) * (Pos.x - player.x)
							+ (Pos.y - player.y) * (Pos.y - player.y)))
				active = true;
			else
				active = false;
			if (active) {
				//get the angle between the player and the turret
				double x = (Pos.x + (Pos.w / 2)) - (player.x + (player.w / 2));
				double y = (Pos.y + (Pos.h / 2)) - (player.y + (player.h / 2));
				angle = atan2(y, x) * 180 / 3.14;

				if (angle > 360)
					angle = 0;

				if (angle < -360)
					angle = 0;

				if (angle > 90 && angle < 270) {
					flipped = true;
				} else if (angle < -90 && angle > -270) {
					flipped = true;
				} else
					flipped = false;

				pos_X -= 150 * cos((angle / 180 * 3.14)) * deltaTime;
				pos_Y -= 150 * sin((angle / 180 * 3.14)) * deltaTime;

				if (SDL_GetTicks() > attackTime) {
					attackTime = SDL_GetTicks() + 1 * 1000;
				}
			}

			Pos.x = (int) (pos_X + 0.5);
			Pos.y = (int) (pos_Y + 0.5);
		}
	}

	void draw(SDL_Renderer * renderer) {

		if (health > 0) {
			if (flipped)
				SDL_RenderCopyEx(renderer, image, NULL, &Pos, angle, &Center,
						SDL_FLIP_VERTICAL);
			else
				SDL_RenderCopyEx(renderer, image, NULL, &Pos, angle, &Center,
						SDL_FLIP_NONE);
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

};

#endif /* HHRESOURCE_SOURCES_CHASEENEMY_H_ */
