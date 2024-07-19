#pragma once

#include <ctime>
#include <cstdint>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <cmath>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define NUMBER_OF_MODE 3

SDL_Color WHITE{ 255,255,255,255 };
SDL_Color BLACK{ 0,0,0,255 };
int SQUARE_D = 10;

SDL_Surface* playerSurface;
SDL_Texture* playerTexture;
