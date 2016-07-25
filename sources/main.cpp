#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDl_image.h"
#define getcwd _getcwd
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

//GLOBALS
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

int main(int argc, char* argv[]) {

#if defined(_WIN32) || (_WIN64)
	cout << "Running on Windows..." << endl;
//get the current working directory
	string s_cwd(getcwd(NULL, 0));

// create a string linking to the images folder
	string images_dir = s_cwd + "\\HHResource\\images\\";

// create a string linking to the audio folder
	string audio_dir = s_cwd + "\\HHResource\\audio\\";
#endif

#if defined(__APPLE__)
	cout << "Running on Apple..." << endl;
//get the current working directory
	string s_cwd(getcwd(NULL, 0));

// create a sting linking to the mac's images folder
	string images_dir = s_cwd + "/HHResource/images/";

//create a string link to the audio folder on __APPLE__
	string audio_dir = s_cwd + "/HHResource/audio/";
#endif

	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow("An SDL2 window",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1024,                               // width, in pixels
			768,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// keyboard input
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	//SDL Event to handle event input
	SDL_Event event;

	//Renderer
	SDL_Renderer * renderer = NULL;
	//create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//BACKGROUND STUFF
	//bkgd things
	string BKGDdir = images_dir + "test.png";
	SDL_Rect BKGDpos;
	BKGDpos.x = 0;
	BKGDpos.y = 0;
	BKGDpos.w = 1024;
	BKGDpos.h = 768;
	SDL_Texture * BKGD;
	BKGD = IMG_LoadTexture(renderer, BKGDdir.c_str());
	//END BACKGROUND STUFF


	//PLAYER STUFF
	//player setup
	string playerDIR = images_dir + "player.png";
	SDL_Point playerCenter;
	SDL_Rect playerPos;
	playerPos.x = 0;
	playerPos.y = 0;
	playerPos.w = 128;
	playerPos.h = 128;
	playerCenter.x = 64;
	playerCenter.y = 64;
	SDL_Texture * Player;
	Player = IMG_LoadTexture(renderer, playerDIR.c_str());
	bool flipped = false;
	double angle = 0.0;
	float bob = 0;
	//update floats for precision
	float pos_X = 0.0f;
	float pos_Y = 0.0f;
	//END PLAYER STUFF


	//bool values to control game states
	bool quit = false;

	//game loop
	while (!quit) {

		//Create deltaTime
		thisTime = SDL_GetTicks();
		deltaTime = (float) (thisTime - lastTime) / 1000;
		lastTime = thisTime;

		//check for input events
		if (SDL_PollEvent(&event)) {
			//if the window is closed
			if (event.type == SDL_QUIT) {
				quit = true;
				break;
			}
		}

		//to quit the game
		if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
			//quitting
			SDL_Delay(200);
			quit = true;
		}

		//move up
		if (currentKeyStates[SDL_SCANCODE_W]) {
			pos_Y -= 300 * deltaTime;
			if (flipped) {
				angle += 150 * deltaTime;
			} else {
				angle -= 150 * deltaTime;
			}
		}

		//move down
		if (currentKeyStates[SDL_SCANCODE_S]) {
			pos_Y += 300 * deltaTime;
			if (flipped) {
				angle -= 150 * deltaTime;
			} else {
				angle += 150 * deltaTime;
			}
		}

		//move left
		if (currentKeyStates[SDL_SCANCODE_A]) {
			pos_X -= 300 * deltaTime;
			if (flipped == false) {
				flipped = true;
				angle *= -1;
			}
		}

		//move right
		if (currentKeyStates[SDL_SCANCODE_D]) {
			pos_X += 300 * deltaTime;
			if (flipped == true) {
				flipped = false;
				angle *= -1;
			}
		}

		//move right
		if (currentKeyStates[SDL_SCANCODE_X]) {
			angle -= 150 * deltaTime;
		}

		//move right
		if (currentKeyStates[SDL_SCANCODE_Z]) {
			angle += 150 * deltaTime;
		}

		//Update
		//Update player position code to account for precision loss
		playerPos.x = (int) (pos_X + 0.5f);
		playerPos.y = (int) (pos_Y + 0.5f);
		//for bobbing effect
		bob+=3 * deltaTime;
		if(bob >360)
			bob=0;
		playerPos.y += (int) 5*sin(bob);
		angle *= .995;

		//Draw
		//clear the renderer
		SDL_RenderClear(renderer);

		//Draw the BKGD
		SDL_RenderCopy(renderer, BKGD, NULL, &BKGDpos);

		//Draw the Player
		if (flipped)
			SDL_RenderCopyEx(renderer, Player, NULL, &playerPos, angle,
					&playerCenter, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopyEx(renderer, Player, NULL, &playerPos, angle,
					&playerCenter, SDL_FLIP_NONE);

		//present renderer
		SDL_RenderPresent(renderer);

	}

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
