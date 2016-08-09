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
#include <vector>

#include "bullet.h"

using namespace std;

class Player {
public:

	string DIR; // = images_dir + "player.png";
	SDL_Point Center;
	SDL_Rect Pos;
	SDL_Texture * PlayerIMAGE;

	SDL_Point armCenter;
	SDL_Rect armPos;
	SDL_Texture * armImage;
	int ammoCount;
	int batCount;
	int currentHealth; // , maxHealth;

	//health stuff
	SDL_Rect HBpos;
	SDL_Texture * hback;

	SDL_Rect HMpos;
	SDL_Texture * hmiddle;
	SDL_Texture * hfront;

	//ammo stuff
	SDL_Rect Apos;
	SDL_Texture * ammoImage[11];

	//inv stuff
	SDL_Rect Ipos;
	SDL_Texture * batImage[3];
	SDL_Texture * invback;

	//bullet list
	vector<Bullet*> bulletList;

	bool flipped; // = false;
	double angle; // = 0.0;
	double armAngle;
	float bob; // = 0;
	//update floats for precision
	float pos_X;	// = 0.0f;
	float pos_Y;	// = 320.0f;
	float fireRate;
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

		DIR = dir + "arm.png";
		armPos.x = x; // 0
		armPos.y = y; // 320
		armPos.w = 128;
		armPos.h = 128;
		armCenter.x = 58;
		armCenter.y = 16;
		armImage = IMG_LoadTexture(renderer, DIR.c_str());
		armAngle = 0;

		ammoCount = 10;
		batCount = 0;
		flipped = false;
		angle = 0;
		bob = 0;
		pos_X = x; // 0
		pos_Y = y; //320
		currentHealth = 100;
		//maxHealth = 100;
		fireRate = 0;

		//health HUD
		DIR = dir + "hback.png";
		hback = IMG_LoadTexture(renderer, DIR.c_str());
		HBpos.x = 350;
		HBpos.y = 0;
		HBpos.w = 268;
		HBpos.h = 69;

		DIR = dir + "hmiddle.png";
		hmiddle = IMG_LoadTexture(renderer, DIR.c_str());
		HMpos.x = 350;
		HMpos.y = 0;
		HMpos.w = 268;
		HMpos.h = 69;

		DIR = dir + "hfront.png";
		hfront = IMG_LoadTexture(renderer, DIR.c_str());

		//ammo stuff
		Apos.x = 0;
		Apos.y = 0;
		Apos.w = 269;
		Apos.h = 94;
		DIR = dir + "pumpkin0.png";
		ammoImage[0] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin1.png";
		ammoImage[1] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin2.png";
		ammoImage[2] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin3.png";
		ammoImage[3] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin4.png";
		ammoImage[4] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin5.png";
		ammoImage[5] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin6.png";
		ammoImage[6] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin7.png";
		ammoImage[7] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin8.png";
		ammoImage[8] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin9.png";
		ammoImage[9] = IMG_LoadTexture(renderer, DIR.c_str());
		DIR = dir + "pumpkin10.png";
		ammoImage[10] = IMG_LoadTexture(renderer, DIR.c_str());

		//ammo stuff
		DIR = dir + "iback.png";
		invback = IMG_LoadTexture(renderer, DIR.c_str());
		Ipos.x = 757;
		Ipos.y = 0;
		Ipos.w = 267;
		Ipos.h = 53;

		DIR = dir + "bat1.png";
		batImage[0] = IMG_LoadTexture(renderer, DIR.c_str());

		DIR = dir + "bat2.png";
		batImage[1] = IMG_LoadTexture(renderer, DIR.c_str());

		DIR = dir + "bat3.png";
		batImage[2] = IMG_LoadTexture(renderer, DIR.c_str());

		//END PLAYER STUFF

		//bullet setup
		for (int i = 0; i < 10; i++) {
			bulletList.push_back(
					new Bullet(renderer, dir, "pumkinB.png", 0, 0));
			bulletList[i]->spinning = true;
		}
	}

	/*void health(SDL_Renderer * renderer, string dir, int x, int y)
	 {

	 }*/

	void update(float deltaTime, int mx, int my) {
		//Update player position code to account for precision loss
		Pos.x = (int) (pos_X + 0.5f);
		Pos.y = (int) (pos_Y + 0.5f);

		//for bobbing effect
		bob += 3 * deltaTime;
		if (bob > 360)
			bob = 0;
		Pos.y += (int) 5 * sin(bob);

		double ax = Pos.x + (50 * cos((angle - 90) / 180 * 3.14));
		double ay = Pos.y + (50 * sin((angle - 90) / 180 * 3.14));

		armPos.x = (int) (ax);
		armPos.y = (int) (ay) + 50;

		double x = (armPos.x + (60)) - (mx);
		double y = (armPos.y + (17)) - (my);
		armAngle = atan2(y, x) * 180 / 3.14;

		angle *= .995;// * deltaTime;

		if (ammoCount < 0)
			ammoCount = 0;

		if (currentHealth > 100) {
			currentHealth = 100;
		}

		if (currentHealth < 0) {
			currentHealth = 0;
		}

		//healthbar status
		HMpos.w = 268 * currentHealth / 100;
		//HMpos.w = HMpos.w / 100;

		for (int i = 0; i < bulletList.size(); i++) {
			if (bulletList[i]->active) {
				bulletList[i]->update(deltaTime);
			}
		}

	}

	void draw(SDL_Renderer * renderer) {

		//draw bullets
		for (int i = 0; i < bulletList.size(); i++) {
			bulletList[i]->draw(renderer);
		}

		if (flipped) {
			SDL_RenderCopyEx(renderer, PlayerIMAGE, NULL, &Pos, angle, &Center,
					SDL_FLIP_HORIZONTAL);
			armCenter.x = 70;
			SDL_RenderCopyEx(renderer, armImage, NULL, &armPos, armAngle,
					&armCenter, SDL_FLIP_HORIZONTAL);
		} else {
			SDL_RenderCopyEx(renderer, PlayerIMAGE, NULL, &Pos, angle, &Center,
					SDL_FLIP_NONE);
			armCenter.x = 58;
			SDL_RenderCopyEx(renderer, armImage, NULL, &armPos, 180 + armAngle,
					&armCenter, SDL_FLIP_NONE);
		}

		//health back
		SDL_RenderCopy(renderer, hback, NULL, &HBpos);
		//health middle
		SDL_RenderCopy(renderer, hmiddle, NULL, &HMpos);
		//health front
		SDL_RenderCopy(renderer, hfront, NULL, &HBpos);

		//ammo 
		SDL_RenderCopy(renderer, ammoImage[ammoCount], NULL, &Apos);

		//inventory
		SDL_RenderCopy(renderer, invback, NULL, &Ipos);

		for (int i = 0; i < batCount; i++)
			SDL_RenderCopy(renderer, batImage[i], NULL, &Ipos);

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

	void throwPumpkin() {
		for (int i = 0; i < bulletList.size(); i++) {
			if (bulletList[i]->active == false) {
				bulletList[i]->fire(armAngle, Pos, 58, 0);
				break;
			}
		}
	}

	~Player()
	{
		for (int i = 0; i < bulletList.size(); i++)
		{
			delete bulletList[i];
		}
		bulletList.clear();
	}

};

#endif /* HHRESOURCE_SOURCES_PLAYER_H_ */
