#ifndef HHRESOURCE_SOURCES_PICKUP_H_
#define HHRESOURCE_SOURCES_PICKUP_H_

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

class Pickup
{
public:

	string DIR; // = images_dir + "player.png";
	SDL_Rect Pos;
	SDL_Texture * image;
	bool active;
	float bob;

	float pos_X, pos_Y;

	Pickup(SDL_Renderer * renderer, string dir, string file, int x, int y)
	{
		Pos.x = x; // 0
		Pos.y = y; // 320
		Pos.w = 64;
		Pos.h = 64;
		pos_X = x;
		pos_Y = y;

		bob = 120;
		active = true;

		DIR = dir + file;
		image = IMG_LoadTexture(renderer, DIR.c_str());
	}

	void update(float deltaTime)
	{
		Pos.x = (int)(pos_X + 0.5);
		Pos.y = (int)(pos_Y + 0.5);

	}

	void bobble(float deltaTime)
	{
		//for bobbing effect
		bob += 2 * deltaTime;
		if (bob > 360)
			bob = 0;
		Pos.y += (int)5 * sin(bob);
	}


	void draw(SDL_Renderer * renderer)
	{
		if (active)
		//health back
		SDL_RenderCopy(renderer, image, NULL, &Pos);
	}

	inline void move(float modifier, int direction, float deltaTime)
	{
		if (direction == 0)//up
		{
			pos_Y -= ((300 - modifier)* deltaTime);
		}
		else if (direction == 1)//down
		{
			pos_Y += ((300 - modifier)* deltaTime);
		}
		else if (direction == 2)//left
		{
			pos_X -= ((300 - modifier) * deltaTime);
		}
		else if (direction == 3)//right
		{
			pos_X += ((300 - modifier) * deltaTime);
		}
	}

};

#endif