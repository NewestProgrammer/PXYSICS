#include "structAndClass.hpp"
#include "functions.h"
#include "image.h"
#include "main.hpp"

int SDL_main(int argc, char* argv[]) {
    double speed = 500;
    int score = 0;
    int mode = 0;
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);


    if (TTF_Init() == -1) {
        TTF_Quit();
        SDL_Quit();
        logSDLErrorAndQuit("SDL_ttf Init Error");
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        logSDLErrorAndQuit("SDL_image Init Error");
    }
    if (Mix_Init(MIX_INIT_MP3) == 0) {
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        logSDLErrorAndQuit("SDL_mix Init Error");
    }
    TTF_Font* pFont;
    SDL_Window* pWindow = NULL;
    SDL_Renderer* pRenderer = NULL;
    playerSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, SQUARE_D, SQUARE_D, 32, 0, 0, 0, 0);
    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &pWindow, &pRenderer) < 0) {
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        logSDLErrorAndQuit("SDL_CreateWindowAndRenderer Error");
    }

    SDL_SetWindowTitle(pWindow, "XANS");
    pFont = TTF_OpenFont("HBIOS-SYS.ttf", 24);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        Mix_CloseAudio();
        TTF_CloseFont(pFont);
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
        logSDLErrorAndQuit("Failed to initialize SDL_mixer");
    }

    Mix_Music* music = Mix_LoadMUS("bgm.mp3");
    if (music == NULL) {
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        TTF_CloseFont(pFont);
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
        logSDLErrorAndQuit("Failed to load MP3 file");
    }


    bool running = true;

    RenderText* RDT = new RenderText();
    PxysicsMainClass* PC1 = new PxysicsClass1();
    Objects* OBJ = new Objects();

    OBJ->generateBar(); OBJ->movAndShowBar(pRenderer);

    PC1->INIT();
    PC1->x = 400 - SQUARE_D / 2;
    PC1->y = 500 - SQUARE_D / 2;
    PC1->variable = 0;
    int lastTick = 0;

    Mix_PlayMusic(music, -1);
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
            else if (event.type == SDL_QUIT) {
                running = false;
            }
            if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)||(event.type==SDL_MOUSEBUTTONDOWN&&event.button.button==SDL_BUTTON_LEFT)) {
                PC1->variable = 1;
            }
            else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)|| (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)) {

                PC1->variable = 0;

            }
        }


        //start_of_mode
        if (mode == 2 && lastTick == 1) {
            PC1->variable = 0;
        }
        if (OBJ->bru[NUMBER_OF_BAR - 1].x + OBJ->bru[NUMBER_OF_BAR - 1].w == 0) {
            OBJ->generateBar();
            if (mode + 1 < NUMBER_OF_MODE) {
                mode++;
            }
            else {
                mode = 0;
            }
        }
        if (OBJ->bru[NUMBER_OF_BAR - 4].x + OBJ->bru[NUMBER_OF_BAR - 4].w == 0) {
            OBJ->generatePolonium();

        }

        //////////////////////////////
        if (PC1->variable == 0 && mode == 0) {
            PC1->ay = 0.1;
            PC1->MOV({ PC1->vx,PC1->vy });
            PC1->ACL({ PC1->ax,PC1->ay });
        }
        if (PC1->variable == 1 && mode == 0) {
            PC1->ay = -0.1;
            PC1->MOV({ PC1->vx,PC1->vy });
            PC1->ACL({ PC1->ax,PC1->ay });
        }
        //////////////////////////////
        if (PC1->variable == 0 && mode == 1) {
            PC1->vy = 5;
            PC1->ay = 0;
            PC1->MOV({ PC1->vx,PC1->vy });
        }
        if (PC1->variable == 1 && mode == 1) {
            PC1->vy = -5;
            PC1->ay = 0;
            PC1->MOV({ PC1->vx,PC1->vy });
        }
        //////////////////////////////
        if (PC1->variable == 0 && mode == 2) {
            PC1->ay = 0.5;
            PC1->MOV({ PC1->vx,PC1->vy });
            PC1->ACL({ PC1->ax,PC1->ay });
        }
        if (PC1->variable == 1 && mode == 2) {
            PC1->ay = 0.5;
            PC1->vy = -6;
            PC1->MOV({ PC1->vx,PC1->vy });
            PC1->ACL({ PC1->ax,PC1->ay });

        }
        //end_of_mode

        if (PC1->y < 0) {
            PC1->y = 0;
            PC1->VAIN();
        }
        if (PC1->y > 1000 - SQUARE_D) {
            PC1->y = 1000 - SQUARE_D;
            PC1->VAIN();
        }
        SDL_Rect r = { (int)PC1->x,(int)PC1->y,SQUARE_D,SQUARE_D };
        SDL_RenderClear(pRenderer);
        RenderPNG(pRenderer, "./bg.png", 0, 0);
        OBJ->movAndShowBar(pRenderer);

        //start_of_mode
        if (mode == 0) {
            SDL_SetRenderDrawColor(pRenderer, 255, 0, 255, 255);
        }
        //////////////////////////////
        else if (mode == 1) {
            SDL_SetRenderDrawColor(pRenderer, 100, 100, 255, 255);

        }
        //////////////////////////////
        else if (mode == 2) {
            SDL_SetRenderDrawColor(pRenderer, 255, 100, 0, 255);

        }
        //end_of_mode

        SDL_RenderFillRect(pRenderer, &r);
        OBJ->movAndShowPolonium(pRenderer);
        score += OBJ->ifPlayerAtePolonium(PC1->x, PC1->y);
        RDT->renderScore(pRenderer, pFont, score, 0, 0);
        SDL_RenderPresent(pRenderer);
        if (OBJ->ifPlayerDied(PC1->x, PC1->y) == 1) {

            Mix_FreeMusic(music);
            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
            SDL_RenderClear(pRenderer);
            RDT->renderScore(pRenderer, pFont, score, 400, 500);
            SDL_RenderPresent(pRenderer);
            SDL_Delay(2500);
            Mix_CloseAudio();
            TTF_CloseFont(pFont);
            SDL_DestroyRenderer(pRenderer);
            SDL_DestroyWindow(pWindow);
            TTF_Quit();
            Mix_Quit();
            IMG_Quit();
            SDL_Quit();
            delete OBJ;
            delete RDT;
            delete PC1;
            return 0;
        }

        SDL_Delay((int)(1000 * 1 / speed));
        score++;
        if (score % 100 == 0) {
            speed += 0.5f;
        }
        lastTick = PC1->variable;
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    TTF_CloseFont(pFont);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    delete OBJ;
    delete RDT;
    delete PC1;
    return 0;
}