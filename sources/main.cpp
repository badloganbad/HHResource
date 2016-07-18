

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#endif


#if defined(__APPLE__)
#include "SDL2/SDL.h"
#endif


#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {

#if defined(_WIN32) || (_WIN64)
cout << "Running on Windows..." << endl;
#endif


#if defined(__APPLE__)
cout << "Running on Apple..."  << endl;
#endif

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    
    //The Surface contained by the window
    SDL_Surface * screenSurface = NULL;
    
    //Get window surface
    screenSurface = SDL_GetWindowSurface(window);
    
    //fill the surface with blue
    SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 42, 254));
    
    //update the surface
    SDL_UpdateWindowSurface(window);
   	

    // The window is open: could enter program loop here (see SDL_PollEvent())

    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
