#ifndef HHRESOURCE_SOURCES_WALL_H_
#define HHRESOURCE_SOURCES_WALL_H

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

#include "player.h"
#include <stdio.h>

using namespace std;

class Wall
{
public:

	string DIR; // = images_dir + "player.png";
	SDL_Rect Pos;
	SDL_Texture * image;
	bool active;
	float bob;

	float pos_X, pos_Y;

	Wall(SDL_Renderer * renderer, string dir, string file, int x, int y)
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

	inline void checkCollision(Player * player)
	{
		if (((player->Pos.x + player->Pos.w > Pos.x) && (player->Pos.x < Pos.x + Pos.w)) && ((player->Pos.y + player->Pos.h > Pos.y) && (player->Pos.y < Pos.y + Pos.h)))
		{
			//y's
			if (player->Pos.x + player->Pos.w > Pos.x && player->Pos.x < Pos.x + Pos.w)
			{
				if (player->Pos.y + player->Pos.h > Pos.y && player->Pos.y < Pos.y)
					player->pos_Y -= 1; //Pos.y - player->Pos.h;
				else if (player->Pos.y < Pos.y + Pos.h && player->Pos.y < Pos.y)
					player->pos_Y += 1;// Pos.y + Pos.h;
			}
			//x's
			else if (player->Pos.y + player->Pos.h > Pos.y && player->Pos.y < Pos.y + Pos.h)
			{
				if (player->Pos.x + player->Pos.w > Pos.x && player->Pos.x < Pos.x)
					player->pos_X -= 1;// Pos.x - player->Pos.w;
				else if (player->Pos.x < Pos.x + Pos.w && player->Pos.x > Pos.x)
					player->pos_X += 1;// Pos.x + Pos.w;

			}
			
		}





		////x's
		//if (player->Pos.x + player->Pos.w > Pos.x)
		//	player->pos_X = Pos.x - player->Pos.w;
		//if (player->Pos.x < Pos.x + Pos.w)
		//	player->pos_X = Pos.x + Pos.w;

		////y's
		//if (player->Pos.y + player->Pos.h > Pos.y)
		//	player->pos_Y = Pos.y - player->Pos.h;
		//if (player->Pos.y < Pos.y + Pos.h)
		//	player->pos_Y = Pos.y + Pos.h;
	}

};


#endif