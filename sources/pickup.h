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

	Pickup(SDL_Renderer * renderer, string dir, string file, int x, int y)
	{
		Pos.x = x; // 0
		Pos.y = y; // 320
		Pos.w = 64;
		Pos.h = 64;

		active = true;

		DIR = dir + file;
		image = IMG_LoadTexture(renderer, DIR.c_str());
	}

	void update(float deltaTime)
	{

	}


	void draw(SDL_Renderer * renderer)
	{
		if (active)
		//health back
		SDL_RenderCopy(renderer, image, NULL, &Pos);
	}



};

#endif