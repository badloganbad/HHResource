#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDl_image.h"
#include <direct.h>
#define getcwd _getcwd
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "unistd.h"
#endif

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <math.h>

#include "player.h"
#include "scene.h"
#include "pickup.h"
#include "turret.h"
#include "chaseEnemy.h"
#include "wall.h"
#include "button.h"

using namespace std;

//GLOBALS
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

int mouseX = 0, mouseY = 0;

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
	window = SDL_CreateWindow("Night of the Horseman",           // window title
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

	//transition numbers
	enum slide {
		up, down, left, right
	};

	//CREATE THE PLAYER
	Player player(renderer, images_dir, 0, 320);

	//create the background images
	Scenery BKGD(renderer, images_dir, "background.png", 0, 0);
	Scenery MDGD2(renderer, images_dir, "midground2.png", 0, -728);
	Scenery MDGD1(renderer, images_dir, "midground1.png", 0, -728);
	Scenery FRGD(renderer, images_dir, "foreground.png", 0, -728);

	Scenery instruct(renderer, images_dir, "fireInstruct.png", 0, 668);
	instruct.Pos.w = 256;
	instruct.Pos.h = 100;

	//create the pickups
	Pickup * ammo[2];
	ammo[0] = new Pickup(renderer, images_dir, "pumkinB.png", 600, 400);
	ammo[1] = new Pickup(renderer, images_dir, "pumkinB.png", 1150, 400);
	ammo[0]->active = false;
	ammo[1]->active = false;
	Pickup * winBox = new Pickup(renderer, images_dir, "pumkinB.png", 2850,
			350);

	Pickup * health[2];
	health[0] = new Pickup(renderer, images_dir, "spiritOrb.png", 350, 350);
	health[1] = new Pickup(renderer, images_dir, "spiritOrb.png", 1885, 350);
	health[0]->active = false;
	health[1]->active = false;

	Pickup *bat[3];
	bat[0] = new Pickup(renderer, images_dir, "bat.png", 500, -500);
	bat[1] = new Pickup(renderer, images_dir, "bat.png", 2700, -500);
	bat[2] = new Pickup(renderer, images_dir, "bat.png", 1800, 1000);

	//Make the enemy turrets
	Turret * marksman[3];
	marksman[0] = new Turret(renderer, images_dir, 1175, 240);
	marksman[1] = new Turret(renderer, images_dir, 1875, 240);
	marksman[2] = new Turret(renderer, images_dir, 2515, 240);

	//make the enemies that chase the player
	ChaseEnemy * skull[3];
	skull[0] = new ChaseEnemy(renderer, images_dir, 400, -500);
	skull[1] = new ChaseEnemy(renderer, images_dir, 2600, -500);
	skull[2] = new ChaseEnemy(renderer, images_dir, 1700, 1000);

	//GUI objects
	Button title(renderer, images_dir, "title.png", "title.png", 256, 64);
	title.Pos.w = 512;
	title.Pos.h = 128;
	Button playGame(renderer, images_dir, "play-n.png", "play-o.png", 512 - 64,
			384);
	Button quitGame(renderer, images_dir, "quit-n.png", "quit-o.png", 512 - 64,
			384 + 128);
	SDL_Rect mousePos;
	mousePos.x = 0;
	mousePos.y = 0;
	mousePos.w = 16;
	mousePos.h = 16;

	SDL_Rect winRect;
	winRect.x = 2800;
	winRect.y = 320;
	winRect.w = 100;
	winRect.h = 100;

	BKGD.Pos.w = 1024;
	BKGD.Pos.h = 768;

	//Win screen and Lose screen
	Scenery win(renderer, images_dir, "win.png", 0, 0);
	win.Pos.w = 1024;
	win.Pos.h = 768;
	Scenery lose(renderer, images_dir, "lose.png", 0, 0);
	lose.Pos.w = 1024;
	lose.Pos.h = 768;

	//bool values to control game states
	bool quit = false;

	enum GameState {
		GAME, WIN, LOSE, TITLE
	};
	GameState state;
	state = TITLE;

	//game loop
	while (!quit) {

		switch (state) {

		case GAME: {
			/*
			 //reset everything
			 player.currentHealth = 100;
			 player.pos_X = 1;
			 player.pos_Y = 320;

			 BKGD.pos_X = 0;
			 BKGD.pos_Y = 0;
			 MDGD2.pos_X = 0;
			 MDGD2.pos_Y = -728;
			 MDGD1.pos_X = 0;
			 MDGD1.pos_Y = -728;
			 FRGD.pos_X = 0;
			 FRGD.pos_Y = -728;

			 ammo[0]->pos_X = 600;
			 ammo[0]->pos_Y = 400;
			 ammo[1]->pos_X = 1150;
			 ammo[1]->pos_Y = 400;

			 health[0]->pos_X = 350;
			 health[0]->pos_Y = 350;
			 health[1]->pos_X = 1885;
			 health[1]->pos_Y = 350;

			 bat[0]->pos_X = 500;
			 bat[0]->pos_Y = -500;
			 bat[1]->pos_X = 2700;
			 bat[1]->pos_Y = -500;
			 bat[2]->pos_X = 1800;
			 bat[2]->pos_Y = 1000;

			 marksman[0]->pos_X = 1175;
			 marksman[0]->pos_Y = 240;
			 marksman[1]->pos_X = 1875;
			 marksman[1]->pos_Y = 240;
			 marksman[2]->pos_X = 2515;
			 marksman[2]->pos_Y = 240;

			 skull[0]->pos_X = 400;
			 skull[0]->pos_Y = -500;
			 skull[1]->pos_X = 2600;
			 skull[1]->pos_Y = -500;
			 skull[2]->pos_X = 1700;
			 skull[2]->pos_Y = 1000;
			 */

			//Create deltaTime
			thisTime = SDL_GetTicks();
			deltaTime = (float) (thisTime - lastTime) / 1000;
			lastTime = thisTime;
			player.fireRate += 3 * deltaTime;

			//check for input events
			if (SDL_PollEvent(&event)) {
				//if the window is closed
				if (event.type == SDL_QUIT) {
					quit = true;
					break;
				}
			}

			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (player.fireRate > 1 && player.ammoCount > 0) {
						player.ammoCount--;
						player.fireRate = 0;
						player.throwPumpkin();
					}
				}
				break;

			case SDL_MOUSEMOTION:
				mouseX = event.button.x;
				mouseY = event.button.y;
				break;

			}

			//to quit the game
			if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
				//quitting
				SDL_Delay(200);
				quit = true;
			}

			//move up
			if (currentKeyStates[SDL_SCANCODE_W]) {

				//move things down
				if (FRGD.Pos.y < 0 && player.Pos.y < 384 - 64) {
					FRGD.move(0, down, deltaTime);
					MDGD1.move(10, down, deltaTime);
					MDGD2.move(20, down, deltaTime);

					winBox->move(0, down, deltaTime);

					//pickups
					for (int i = 0; i < 2; i++) {
						ammo[i]->move(0, down, deltaTime);
						health[i]->move(0, down, deltaTime);
					}

					for (int i = 0; i < 3; i++) {
						bat[i]->move(0, down, deltaTime);
					}
					//bullets
					for (int i = 0; i < player.bulletList.size(); i++) {
						if (player.bulletList[i]->active) {
							player.bulletList[i]->move(0, down, deltaTime);
						}
					}
					//enemies
					for (int i = 0; i < 3; i++) {
						skull[i]->move(0, down, deltaTime);
						marksman[i]->move(0, down, deltaTime);
						for (int j = 0; j < marksman[i]->bulletList.size();
								j++) {
							marksman[i]->bulletList[j]->move(0, down,
									deltaTime);
						}
					}

				} else if (player.Pos.y > 0)
					player.move(0, up, deltaTime);

				if (player.flipped) {
					if (player.angle < 45)
						player.angle += 100 * deltaTime;
				} else {
					if (player.angle > -45)
						player.angle -= 100 * deltaTime;
				}
			}

			//move down
			if (currentKeyStates[SDL_SCANCODE_S]) {

				//move the foreground
				if (FRGD.Pos.y > -1536 && player.Pos.y > 384 - 64) {
					FRGD.move(0, up, deltaTime);
					MDGD1.move(10, up, deltaTime);
					MDGD2.move(20, up, deltaTime);

					winBox->move(0, up, deltaTime);

					//pickups
					for (int i = 0; i < 2; i++) {
						ammo[i]->move(0, up, deltaTime);
						health[i]->move(0, up, deltaTime);
					}

					for (int i = 0; i < 3; i++) {
						bat[i]->move(0, up, deltaTime);
					}
					//bullets
					for (int i = 0; i < player.bulletList.size(); i++) {
						if (player.bulletList[i]->active) {
							player.bulletList[i]->move(0, up, deltaTime);
						}
					}
					//enemies
					for (int i = 0; i < 3; i++) {
						skull[i]->move(0, up, deltaTime);
						marksman[i]->move(0, up, deltaTime);
						for (int j = 0; j < marksman[i]->bulletList.size();
								j++) {
							marksman[i]->bulletList[j]->move(0, up, deltaTime);
						}
					}

				} else if (player.Pos.y < 768 - 128)
					player.move(0, down, deltaTime);

				if (player.flipped) {
					if (player.angle > -90)
						player.angle -= 100 * deltaTime;
				} else {
					if (player.angle < 90)
						player.angle += 100 * deltaTime;
				}
			}

			//move left
			if (currentKeyStates[SDL_SCANCODE_A]) {
				//move the foreground
				if (player.Pos.x < 512 - 64 && FRGD.Pos.x < 1) {
					FRGD.move(0, right, deltaTime);
					MDGD1.move(10, right, deltaTime);
					MDGD2.move(20, right, deltaTime);

					winBox->move(0, right, deltaTime);

					//pickups
					for (int i = 0; i < 2; i++) {
						ammo[i]->move(0, right, deltaTime);
						health[i]->move(0, right, deltaTime);
					}

					for (int i = 0; i < 3; i++) {
						bat[i]->move(0, right, deltaTime);
					}

					//bullets
					for (int i = 0; i < player.bulletList.size(); i++) {
						if (player.bulletList[i]->active) {
							player.bulletList[i]->move(0, right, deltaTime);
						}
					}

					//enemies
					for (int i = 0; i < 3; i++) {
						skull[i]->move(0, right, deltaTime);
						marksman[i]->move(0, right, deltaTime);
						for (int j = 0; j < marksman[i]->bulletList.size();
								j++) {
							marksman[i]->bulletList[j]->move(0, right,
									deltaTime);
						}
					}

				} else if (player.Pos.x > 0)
					player.move(0, left, deltaTime);

				if (player.flipped == false) {
					player.flipped = true;
					player.angle *= -1;
				}
			}

			//move right
			if (currentKeyStates[SDL_SCANCODE_D]) {
				//move the foreground
				if (player.Pos.x > 512 - 64 && FRGD.Pos.x > -2048) {
					FRGD.move(0, left, deltaTime);
					MDGD1.move(10, left, deltaTime);
					MDGD2.move(20, left, deltaTime);

					winBox->move(0, left, deltaTime);

					//pickups
					for (int i = 0; i < 2; i++) {
						ammo[i]->move(0, left, deltaTime);
						health[i]->move(0, left, deltaTime);
					}

					for (int i = 0; i < 3; i++) {
						bat[i]->move(0, left, deltaTime);
					}
					for (int i = 0; i < player.bulletList.size(); i++) {
						if (player.bulletList[i]->active) {
							player.bulletList[i]->move(0, left, deltaTime);
						}
					}

					//enemies
					for (int i = 0; i < 3; i++) {
						skull[i]->move(0, left, deltaTime);
						marksman[i]->move(0, left, deltaTime);
						for (int j = 0; j < marksman[i]->bulletList.size();
								j++) {
							marksman[i]->bulletList[j]->move(0, left,
									deltaTime);
						}
					}

				} else if (player.Pos.x < 1024 - 128)
					player.move(0, right, deltaTime);

				if (player.flipped == true) {
					player.flipped = false;
					player.angle *= -1;
				}
			}

			//spacebar
			if (currentKeyStates[SDL_SCANCODE_SPACE]) {

			}

			//check for collision - ammo
			for (int i = 0; i < 2; i++) {
				if (SDL_HasIntersection(&player.Pos, &ammo[i]->Pos)) {
					if (ammo[i]->active) {
						player.ammoCount = 10;
						ammo[i]->active = false;
					}
				}

				//check for collision - health +
				if (SDL_HasIntersection(&player.Pos, &health[i]->Pos)) {
					if (health[i]->active) {
						player.currentHealth += 25;
						health[i]->active = false;
					}
				}
			}

			for (int i = 0; i < 3; i++) {
				//check for collision - item 1
				if (SDL_HasIntersection(&player.Pos, &bat[i]->Pos)) {
					if (bat[i]->active) {
						player.batCount++;
						bat[i]->active = false;
					}
				}
			}

			//collision, player's bullets to enemies
			for (int i = 0; i < player.bulletList.size(); i++) {
				for (int j = 0; j < 3; j++) {
					if (player.bulletList[i]->active) {
						if (marksman[j]->health > 0) { //if it hits a marksman thats alive
							if (3000 //distance formula
									> ((player.bulletList[i]->Pos.x
											- (marksman[j]->Pos.x
													+ marksman[j]->Pos.w * 3 / 4))
											* (player.bulletList[i]->Pos.x
													- (marksman[j]->Pos.x
															+ marksman[j]->Pos.w
																	* 3 / 4))
											+ (player.bulletList[i]->Pos.y
													- (marksman[j]->Pos.y
															+ marksman[j]->Pos.h
																	/ 2))
													* (player.bulletList[i]->Pos.y
															- (marksman[j]->Pos.y
																	+ marksman[j]->Pos.h
																			/ 2)))) {
								player.bulletList[i]->reset();
								marksman[j]->health--;
								marksman[j]->active = false;
							}
						}
						if (skull[j]->health > 0) { // if it hits a skull that is alive
							if (1600 //distance formula
									> ((player.bulletList[i]->Pos.x + 16
											- (skull[j]->Pos.x
													+ skull[j]->Pos.w / 2))
											* (player.bulletList[i]->Pos.x + 16
													- (skull[j]->Pos.x
															+ skull[j]->Pos.w
																	/ 2))
											+ (player.bulletList[i]->Pos.y + 16
													- (skull[j]->Pos.y
															+ skull[j]->Pos.h
																	/ 2))
													* (player.bulletList[i]->Pos.y
															+ 16
															- (skull[j]->Pos.y
																	+ skull[j]->Pos.h
																			/ 2)))) {
								player.bulletList[i]->reset();
								skull[j]->health--;
								skull[j]->active = false;
							}

						}
					}
				}
			}

			//collision for enemies bullets to player
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < marksman[i]->bulletList.size(); j++) {
					if (marksman[i]->bulletList[j]->active) {
						if (3000
								> (((player.Pos.x + player.Pos.w / 2)
										- (marksman[i]->bulletList[j]->Pos.x
												+ marksman[i]->bulletList[j]->Pos.w
														/ 2))
										* ((player.Pos.x + player.Pos.w / 2)
												- (marksman[i]->bulletList[j]->Pos.x
														+ marksman[i]->bulletList[j]->Pos.w
																/ 2))
										+ ((player.Pos.y + player.Pos.h / 2)
												- (marksman[i]->bulletList[j]->Pos.y
														+ marksman[i]->bulletList[j]->Pos.h
																/ 2))
												* ((player.Pos.y
														+ player.Pos.h / 2)
														- (marksman[i]->bulletList[j]->Pos.y
																+ marksman[i]->bulletList[j]->Pos.h
																		/ 2)))) {
							player.currentHealth -= 10;
							marksman[i]->bulletList[j]->reset();
						}
					}
				}
			}

			//collision with chasing enemies to the player
			for (int i = 0; i < 3; i++) {
				if (skull[i]->active) {
					if (2500
							> (((player.Pos.x + player.Pos.w / 2)
									- (skull[i]->Pos.x + skull[i]->Pos.w / 2))
									* ((player.Pos.x + player.Pos.w / 2)
											- (skull[i]->Pos.x
													+ skull[i]->Pos.w / 2))
									+ ((player.Pos.y + player.Pos.h / 2)
											- (skull[i]->Pos.y
													+ skull[i]->Pos.h / 2))
											* ((player.Pos.y + player.Pos.h / 2)
													- (skull[i]->Pos.y
															+ skull[i]->Pos.h
																	/ 2)))) {
						if (SDL_GetTicks() > skull[i]->attackTime) {
							player.currentHealth -= 3;
						}
					}
				}
			}

			//Update
			player.update(deltaTime, mouseX, mouseY);
			for (int i = 0; i < 3; i++) {
				marksman[i]->update(deltaTime, player.Pos);
				skull[i]->update(deltaTime, player.Pos);
			}

			//pickups
			for (int i = 0; i < 2; i++) {
				ammo[i]->update(deltaTime);
				health[i]->update(deltaTime);
				health[i]->bobble(deltaTime);
			}

			//bats
			for (int i = 0; i < 3; i++) {
				bat[i]->update(deltaTime);
			}

			winBox->update(deltaTime);

			//provide the player with ammo or health
			if (ammo[0]->active == false && ammo[1]->active == false
					&& player.ammoCount < 1) {
				int temp = rand() % 2;
				ammo[temp]->active = true;
			}

			//provide the player with ammo or health
			if (health[0]->active == false && health[1]->active == false
					&& player.currentHealth < 70) {
				int temp = rand() % 2;
				health[temp]->active = true;
			}

			//foreground and midground
			FRGD.update();
			MDGD1.update();
			MDGD2.update();

			if (player.currentHealth < 10) {
				state = LOSE;
			}

			//check win collision
			if (SDL_HasIntersection(&player.Pos, &winBox->Pos)
					&& player.batCount == 3) {
				state = WIN;
			}

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

			//draw the ammo
			for (int i = 0; i < 2; i++) {
				ammo[i]->draw(renderer);
				health[i]->draw(renderer);
			}

			for (int i = 0; i < 3; i++) {
				bat[i]->draw(renderer);
			}

			//draw enemy
			for (int i = 0; i < 3; i++) {
				marksman[i]->draw(renderer);
				skull[i]->draw(renderer);
			}

			//Draw the Player
			player.draw(renderer);

			//present renderer
			SDL_RenderPresent(renderer);
			break;
		}		//end GAME

		case TITLE: {
			//check for input events
			if (SDL_PollEvent(&event)) {
				//if the window is closed
				if (event.type == SDL_QUIT) {
					quit = true;
					break;
				}
			}

			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					//collision
					if (SDL_HasIntersection(&mousePos, &playGame.Pos)) {
						playGame.ov = true;
						state = GAME;
					} else {
						playGame.ov = false;
					}

					//collision
					if (SDL_HasIntersection(&mousePos, &quitGame.Pos)) {
						quitGame.ov = true;
						quit = true;

					} else {
						quitGame.ov = false;
					}
				}
				break;

			case SDL_MOUSEMOTION:
				mouseX = event.button.x;
				mouseY = event.button.y;
				break;

			}

			//to quit the game
			if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
				//quitting
				SDL_Delay(200);
				quit = true;
			}

			//update
			mousePos.x = mouseX;
			mousePos.y = mouseY;

			//collision
			if (SDL_HasIntersection(&mousePos, &playGame.Pos))
				playGame.ov = true;
			else
				playGame.ov = false;

			//collision
			if (SDL_HasIntersection(&mousePos, &quitGame.Pos))
				quitGame.ov = true;
			else
				quitGame.ov = false;

			//Draw
			//clear the renderer
			SDL_RenderClear(renderer);

			BKGD.draw(renderer);
			title.draw(renderer);
			playGame.draw(renderer);
			quitGame.draw(renderer);

			//present renderer
			SDL_RenderPresent(renderer);

			break;
		}						//end Title

		case WIN: {
			//check for input events
			if (SDL_PollEvent(&event)) {
				//if the window is closed
				if (event.type == SDL_QUIT) {
					quit = true;
					break;
				}
			}

			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					//collision
					if (SDL_HasIntersection(&mousePos, &playGame.Pos)) {
						playGame.ov = true;

						//reset everything
						player.currentHealth = 100;
						player.pos_X = 1;
						player.pos_Y = 320;

						BKGD.pos_X = 0;
						BKGD.pos_Y = 0;
						MDGD2.pos_X = 0;
						MDGD2.pos_Y = -728;
						MDGD1.pos_X = 0;
						MDGD1.pos_Y = -728;
						FRGD.pos_X = 0;
						FRGD.pos_Y = -728;

						ammo[0]->pos_X = 600;
						ammo[0]->pos_Y = 400;
						ammo[1]->pos_X = 1150;
						ammo[1]->pos_Y = 400;

						health[0]->pos_X = 350;
						health[0]->pos_Y = 350;
						health[1]->pos_X = 1885;
						health[1]->pos_Y = 350;

						bat[0]->pos_X = 500;
						bat[0]->pos_Y = -500;
						bat[0]->active = true;

						bat[1]->pos_X = 2700;
						bat[1]->pos_Y = -500;
						bat[1]->active = true;

						bat[2]->pos_X = 1800;
						bat[2]->pos_Y = 1000;
						bat[2]->active = true;

						marksman[0]->pos_X = 1175;
						marksman[0]->pos_Y = 240;
						marksman[0]->active = true;

						marksman[1]->pos_X = 1875;
						marksman[1]->pos_Y = 240;
						marksman[1]->active = true;

						marksman[2]->pos_X = 2515;
						marksman[2]->pos_Y = 240;
						marksman[2]->active = true;

						skull[0]->pos_X = 400;
						skull[0]->pos_Y = -500;
						skull[0]->active = true;

						skull[1]->pos_X = 2600;
						skull[1]->pos_Y = -500;
						skull[1]->active = true;

						skull[2]->pos_X = 1700;
						skull[2]->pos_Y = 1000;
						skull[2]->active = true;

						state = GAME;
					} else {
						playGame.ov = false;
					}

					//collision
					if (SDL_HasIntersection(&mousePos, &quitGame.Pos)) {
						quitGame.ov = true;
						quit = true;

					} else {
						quitGame.ov = false;
					}
				}
				break;

			case SDL_MOUSEMOTION:
				mouseX = event.button.x;
				mouseY = event.button.y;
				break;

			}

			//to quit the game
			if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
				//quitting
				SDL_Delay(200);
				quit = true;
			}

			//update
			mousePos.x = mouseX;
			mousePos.y = mouseY;

			//collision
			if (SDL_HasIntersection(&mousePos, &playGame.Pos))
				playGame.ov = true;
			else
				playGame.ov = false;

			//collision
			if (SDL_HasIntersection(&mousePos, &quitGame.Pos))
				quitGame.ov = true;
			else
				quitGame.ov = false;

			//Draw
			//clear the renderer
			SDL_RenderClear(renderer);

			win.draw(renderer);
			playGame.draw(renderer);
			quitGame.draw(renderer);

			//present renderer
			SDL_RenderPresent(renderer);

			break;
		}						//end WIN

		case LOSE: {
			//check for input events
			if (SDL_PollEvent(&event)) {
				//if the window is closed
				if (event.type == SDL_QUIT) {
					quit = true;
					break;
				}
			}

			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					//collision
					if (SDL_HasIntersection(&mousePos, &playGame.Pos)) {
						playGame.ov = true;

						//reset everything
						player.currentHealth = 100;
						player.pos_X = 1;
						player.pos_Y = 320;

						BKGD.pos_X = 0;
						BKGD.pos_Y = 0;
						MDGD2.pos_X = 0;
						MDGD2.pos_Y = -728;
						MDGD1.pos_X = 0;
						MDGD1.pos_Y = -728;
						FRGD.pos_X = 0;
						FRGD.pos_Y = -728;

						ammo[0]->pos_X = 600;
						ammo[0]->pos_Y = 400;
						ammo[1]->pos_X = 1150;
						ammo[1]->pos_Y = 400;

						health[0]->pos_X = 350;
						health[0]->pos_Y = 350;
						health[1]->pos_X = 1885;
						health[1]->pos_Y = 350;

						bat[0]->pos_X = 500;
						bat[0]->pos_Y = -500;
						bat[0]->active = true;

						bat[1]->pos_X = 2700;
						bat[1]->pos_Y = -500;
						bat[1]->active = true;

						bat[2]->pos_X = 1800;
						bat[2]->pos_Y = 1000;
						bat[2]->active = true;

						marksman[0]->pos_X = 1175;
						marksman[0]->pos_Y = 240;
						marksman[0]->active = true;

						marksman[1]->pos_X = 1875;
						marksman[1]->pos_Y = 240;
						marksman[1]->active = true;

						marksman[2]->pos_X = 2515;
						marksman[2]->pos_Y = 240;
						marksman[2]->active = true;

						skull[0]->pos_X = 400;
						skull[0]->pos_Y = -500;
						skull[0]->active = true;

						skull[1]->pos_X = 2600;
						skull[1]->pos_Y = -500;
						skull[1]->active = true;

						skull[2]->pos_X = 1700;
						skull[2]->pos_Y = 1000;
						skull[2]->active = true;
						state = GAME;
					} else {
						playGame.ov = false;
					}

					//collision
					if (SDL_HasIntersection(&mousePos, &quitGame.Pos)) {
						quitGame.ov = true;
						quit = true;

					} else {
						quitGame.ov = false;
					}
				}
				break;

			case SDL_MOUSEMOTION:
				mouseX = event.button.x;
				mouseY = event.button.y;
				break;

			}

			//to quit the game
			if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
				//quitting
				SDL_Delay(200);
				quit = true;
			}

			//update
			mousePos.x = mouseX;
			mousePos.y = mouseY;

			//collision
			if (SDL_HasIntersection(&mousePos, &playGame.Pos))
				playGame.ov = true;
			else
				playGame.ov = false;

			//collision
			if (SDL_HasIntersection(&mousePos, &quitGame.Pos))
				quitGame.ov = true;
			else
				quitGame.ov = false;

			//Draw
			//clear the renderer
			SDL_RenderClear(renderer);

			lose.draw(renderer);
			playGame.draw(renderer);
			quitGame.draw(renderer);

			//present renderer
			SDL_RenderPresent(renderer);

			break;
		}						//End LOSE

		default:
			break;
		}						//end switch

	}

	for (int i = 0; i < 3; i++) {
		delete skull[i];
		delete bat[i];
		delete marksman[i];
	}

	for (int i = 0; i < 2; i++) {
		delete ammo[i];
		delete health[i];
	}

// Close and destroy the window
	SDL_DestroyWindow(window);

// Clean up
	SDL_Quit();
	return 0;
}
