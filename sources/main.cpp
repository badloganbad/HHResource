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

	//create the background images
	Scenery BKGD(renderer, images_dir, "background.png", 0, 0);
	Scenery MDGD2(renderer, images_dir, "midground2.png", 0, -728);
	Scenery MDGD1(renderer, images_dir, "midground1.png", 0, -728);
	Scenery FRGD(renderer, images_dir, "foreground.png", 0, -728);

	BKGD.Pos.w = 1024;
	BKGD.Pos.h = 768;

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
			if (FRGD.Pos.y < 0 && player.Pos.y < 384 - 64) {
				FRGD.pos_Y += (300 * deltaTime);
				MDGD1.pos_Y += (290 * deltaTime);
				MDGD2.pos_Y += (280 * deltaTime);
			} else if (player.Pos.y > 0)
				player.pos_Y -= 300 * deltaTime;

			if (player.flipped) {
				player.angle += 100 * deltaTime;
			} else {
				player.angle -= 100 * deltaTime;
			}
		}

		//move down
		if (currentKeyStates[SDL_SCANCODE_S]) {

			//move the foreground
			if (FRGD.Pos.y > -1536 && player.Pos.y > 384 - 64) {
				FRGD.pos_Y -= (300 * deltaTime);
				MDGD1.pos_Y -= (290 * deltaTime);
				MDGD2.pos_Y -= (280 * deltaTime);
			} else if (player.Pos.y < 768 - 128)
				player.pos_Y += 300 * deltaTime;

			if (player.flipped) {
				player.angle -= 100 * deltaTime;
			} else {
				player.angle += 100 * deltaTime;
			}
		}

		//move left
		if (currentKeyStates[SDL_SCANCODE_A]) {
			//move the foreground
			if (player.Pos.x < 512 - 64 && FRGD.Pos.x < 1) {
				FRGD.pos_X += (300 * deltaTime);
				MDGD1.pos_X += (280 * deltaTime);
				MDGD2.pos_X += (260 * deltaTime);
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
			if (player.Pos.x > 512 - 64 && FRGD.Pos.x > -2048) {
				FRGD.pos_X -= (300 * deltaTime);
				MDGD1.pos_X -= (280 * deltaTime);
				MDGD2.pos_X -= (260 * deltaTime);
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
		FRGD.update();
		MDGD1.update();
		MDGD2.update();

		//Draw
		//clear the renderer
		SDL_RenderClear(renderer);

		//Draw the BKGD
		BKGD.draw(renderer);
		//Draw the MDGD2
		MDGD2.draw(renderer);
		//Draw the MDGD1
		MDGD1.draw(renderer);
		//Draw the FRGD
		FRGD.draw(renderer);

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
