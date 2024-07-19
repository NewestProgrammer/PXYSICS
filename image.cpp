#include "image.h"
#include "functions.h"

void RenderPNG(SDL_Renderer* pRenderer, const char* file, int PNG_X, int PNG_Y) {
    SDL_Surface* imageSurface = IMG_Load(file);
    if (imageSurface == NULL) {
        SDL_FreeSurface(imageSurface);
        logSDLErrorAndQuit("TTF_RenderText_Blended");
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, imageSurface);
    if (texture == NULL) {
        SDL_DestroyTexture(texture);
        logSDLErrorAndQuit("SDL_CreateTextureFromSurface");
    }
    SDL_Rect destRect = { PNG_X, PNG_Y, imageSurface->w, imageSurface->h };
    SDL_RenderCopy(pRenderer, texture, NULL, &destRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(imageSurface);
}