#include "functions.h"

void logSDLErrorAndQuit(const char* msg) {
    SDL_Quit();
    exit(1);
}