/*
 * button.h
 *
 *  Created on: Aug 16, 2016
 *      Author: Phillipbadgett
 */

#ifndef HHRESOURCE_SOURCES_BUTTON_H_
#define HHRESOURCE_SOURCES_BUTTON_H_

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

class Button
{
public:
	SDL_Rect Pos;
	SDL_Texture * normal, * over;
	string DIR;
	bool ov;

	Button(SDL_Renderer * renderer, string dir, string Fnormal, string Fover, int x, int y)
	{
		Pos.x = x;
		Pos.y = y;
		Pos.w = 128;
		Pos.h = 64;

		DIR = dir + Fnormal;

		normal = IMG_LoadTexture(renderer, DIR.c_str());

		DIR = dir + Fover;

		over = IMG_LoadTexture(renderer, DIR.c_str());

		ov = false;

	}


	void draw(SDL_Renderer * renderer)
	{
		if (ov)
			SDL_RenderCopy(renderer, over, NULL, &Pos);
		else
			SDL_RenderCopy(renderer, normal, NULL, &Pos);
	}

};



#endif /* HHRESOURCE_SOURCES_BUTTON_H_ */
