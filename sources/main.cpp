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
#include <iostream>
#include <time.h>
#include <math.h>

#include "player.h"
#include "scene.h"
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

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

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

	//CREATE THE PLAYER
	Player player(renderer, images_dir, 0, 320);

	//BACKGROUND STUFF
	//bkgd things
	string BKGDdir = images_dir + "background.png";
	SDL_Rect BKGDpos;
	BKGDpos.x = 0;
	BKGDpos.y = 0;
	BKGDpos.w = 1024;
	BKGDpos.h = 768;
	SDL_Texture * BKGD;
	BKGD = IMG_LoadTexture(renderer, BKGDdir.c_str());
	//END BACKGROUND STUFF

	//MIDGROUND2 STUFF
	//mdgd things
	string MDGD2dir = images_dir + "midground2.png";
	SDL_Rect MDGD2pos;
	MDGD2pos.x = 0;
	MDGD2pos.y = -728;
	MDGD2pos.w = 3072;
	MDGD2pos.h = 2304;
	SDL_Texture * MDGD2;
	MDGD2 = IMG_LoadTexture(renderer, MDGD2dir.c_str());
	float m2_X = 0, m2_Y = -728;
	//END MIDGROUND2 STUFF

	//MIDGROUND1 STUFF
	//mdgd things
	string MDGD1dir = images_dir + "midground1.png";
	SDL_Rect MDGD1pos;
	MDGD1pos.x = 0;
	MDGD1pos.y = -728;
	MDGD1pos.w = 3072;
	MDGD1pos.h = 2304;
	SDL_Texture * MDGD1;
	MDGD1 = IMG_LoadTexture(renderer, MDGD1dir.c_str());
	float m1_X = 0, m1_Y = -728;
	//END MIDGROUND1 STUFF

	//FOREGROUND STUFF
	//frgd things
	string FRGDdir = images_dir + "foreground.png";
	SDL_Rect FRGDpos;
	FRGDpos.x = 0;
	FRGDpos.y = -728;
	FRGDpos.w = 3072;
	FRGDpos.h = 2304;
	SDL_Texture * FRGD;
	FRGD = IMG_LoadTexture(renderer, FRGDdir.c_str());
	float fr_X = 0, fr_Y = -728;
	//END FOREGROUND STUFF



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

			//move the foreground
			if (FRGDpos.y < 0 && player.Pos.y < 384 - 64) {
				fr_Y += (300 * deltaTime);
				m1_Y += (290 * deltaTime);
				m2_Y += (280 * deltaTime);
			} else if (player.Pos.y > 0)
				player.pos_Y -= 300 * deltaTime;

			if (player.flipped) {
				player.angle += 150 * deltaTime;
			} else {
				player.angle -= 150 * deltaTime;
			}
		}

		//move down
		if (currentKeyStates[SDL_SCANCODE_S]) {

			//move the foreground
			if (FRGDpos.y > -1536 && player.Pos.y > 384 - 64) {
				fr_Y -= (300 * deltaTime);
				m1_Y -= (290 * deltaTime);
				m2_Y -= (280 * deltaTime);
			} else if (player.Pos.y < 768 - 128)
				player.pos_Y += 300 * deltaTime;

			if (player.flipped) {
				player.angle -= 150 * deltaTime;
			} else {
				player.angle += 150 * deltaTime;
			}
		}

		//move left
		if (currentKeyStates[SDL_SCANCODE_A]) {
			//move the foreground
			if (player.Pos.x < 512 - 64 && FRGDpos.x < 1) {
				fr_X += (300 * deltaTime);
				m1_X += (280 * deltaTime);
				m2_X += (260 * deltaTime);
			} else if (player.Pos.x > 0)
				player.pos_X -= 300 * deltaTime;

			if (player.flipped == false) {
				player.flipped = true;
				player.angle *= -1;
			}
		}

		//move right
		if (currentKeyStates[SDL_SCANCODE_D]) {
			//move the foreground
			if (player.Pos.x > 512 - 64 && FRGDpos.x > -2048) {
				fr_X -= (300 * deltaTime);
				m1_X -= (280 * deltaTime);
				m2_X -= (260 * deltaTime);
			} else if (player.Pos.x < 1024 - 128)
				player.pos_X += 300 * deltaTime;

			if (player.flipped == true) {
				player.flipped = false;
				player.angle *= -1;
			}
		}

		//Update
		player.update(deltaTime);

		//foreground and midground
		FRGDpos.x = (int) (fr_X + 0.5f);
		FRGDpos.y = (int) (fr_Y + 0.5f);

		MDGD1pos.x = (int) (m1_X + 0.5f);
		MDGD1pos.y = (int) (m1_Y + 0.5f);

		MDGD2pos.x = (int) (m2_X + 0.5f);
		MDGD2pos.y = (int) (m2_Y + 0.5f);

		//Draw
		//clear the renderer
		SDL_RenderClear(renderer);

		//Draw the BKGD
		SDL_RenderCopy(renderer, BKGD, NULL, &BKGDpos);

		//Draw the MDGD2
		SDL_RenderCopy(renderer, MDGD2, NULL, &MDGD2pos);

		//Draw the MDGD1
		SDL_RenderCopy(renderer, MDGD1, NULL, &MDGD1pos);

		//Draw the FRGD
		SDL_RenderCopy(renderer, FRGD, NULL, &FRGDpos);

		//Draw the Player
		player.draw(renderer);

		//present renderer
		SDL_RenderPresent(renderer);

	}

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
