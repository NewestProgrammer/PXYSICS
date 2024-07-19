#pragma once
#include <cmath>
#include <SDL.h>
#include <SDL_ttf.h>
#include "functions.h"
#include "main.hpp"
#include "image.h"

//#define SQUARE_D 10;
#define XGAP 300
#define YGAP 100
#define NUMBER_OF_BAR 5
#define NUMBER_OF_POLONIUM 10
#define SIZE_OF_POLONIUM 50

typedef struct {
    double x;
    double y;
} S;
typedef struct {
    double vx;
    double vy;
} V;
typedef struct {
    double ax;
    double ay;
} A;


class PxysicsMainClass {

public:
    double x, y;
    double vx, vy;
    double ax, ay;
    virtual S MOV(V v) = 0;
    virtual V ACL(A a) = 0;
    virtual ~PxysicsMainClass() = default;
    int variable = 0;
    int radius = 500;;
    void INIT() {
        x = 0, y = 0;
        vx = 0, vy = 0;
        ax = 0, ay = 0;
    }
    void VAIN() {
        vx = 0;
        vy = 0;
        ax = 0;
        ay = 0;
    }
};
class PxysicsClass1 : public PxysicsMainClass {
public:
    S MOV(V v) override {
        x += v.vx;
        y += v.vy;
        S s = { x,y };
        return s;
    }
    V ACL(A a) override {
        vx += a.ax;
        vy += a.ay;
        V v = { vx,vy };
        return v;
    }

};
class Objects {
public:
    int barVariable[NUMBER_OF_BAR];
    int poloniumVariableX[NUMBER_OF_POLONIUM];
    int poloniumVariableY[NUMBER_OF_POLONIUM];
    SDL_Rect bru[NUMBER_OF_BAR];
    SDL_Rect brd[NUMBER_OF_BAR];
    SDL_Rect pr[NUMBER_OF_POLONIUM];
    int poloniumAteVariable[NUMBER_OF_POLONIUM] = { 0, };
    int polonium[NUMBER_OF_POLONIUM];
public:

    void generateBar() {
        for (int i = 0; i < NUMBER_OF_BAR; i++) {
            barVariable[i] = (rand() % (1000 - 2 * YGAP)) + YGAP;
            bru[i] = { 1100 + XGAP * i, 0, 100, barVariable[i] };
            brd[i] = { 1100 + XGAP * i, barVariable[i] + YGAP, 100, 1000 - (barVariable[i] + YGAP) };
        }
    }
    void generatePolonium() {
        for (int i = 0; i < NUMBER_OF_POLONIUM; i++) {
            poloniumAteVariable[i] = 1;
            poloniumVariableY[i] = (rand() % 1000);
            poloniumVariableX[i] = (rand() % 1000);

            pr[i] = { 1001 + poloniumVariableX[i], poloniumVariableY[i], SIZE_OF_POLONIUM, SIZE_OF_POLONIUM };
        }

    }
    void movAndShowPolonium(SDL_Renderer* pRenderer) {
        for (int i = 0; i < NUMBER_OF_POLONIUM; i++) {
            pr[i].x -= 1;
            if (poloniumAteVariable[i] == 1) {
                RenderPNG(pRenderer, "./polonium.png", pr[i].x, pr[i].y);
            }
        }
    }
    void movAndShowBar(SDL_Renderer* pRenderer) {
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255); // 검은 배경색으로 초기화

        for (int i = 0; i < NUMBER_OF_BAR; i++) {
            bru[i].x -= 1;
            brd[i].x -= 1;
            SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
            SDL_RenderFillRect(pRenderer, &bru[i]);
            SDL_RenderFillRect(pRenderer, &brd[i]);
        }
    }

    int ifPlayerDied(int player_x, int player_y) {
        for (int i = 0; i < NUMBER_OF_BAR; i++) {
            if (player_x + SQUARE_D > bru[i].x && player_x < bru[i].x + bru[i].w) {
                if ((player_y < bru[i].h) || (player_y + SQUARE_D > brd[i].y)) {
                    return 1; // 플레이어가 충돌했음을 나타냄
                }
            }
        }
        return 0; // 충돌하지 않음
    }
    int ifPlayerAtePolonium(int player_x, int player_y) {
        int score = 0;
        for (int i = 0; i < NUMBER_OF_POLONIUM; i++) {
            if (player_x + SQUARE_D > pr[i].x && player_x < pr[i].x + SIZE_OF_POLONIUM &&
                player_y + SQUARE_D > pr[i].y && player_y < pr[i].y + SIZE_OF_POLONIUM) {
                if (poloniumAteVariable[i] == 1) {
                    score += 100;
                    poloniumAteVariable[i] = 0;
                }
            }
        }
        return score;
    }
};
class RenderText {
private:
    void renderText(SDL_Renderer* renderer, const char* message, TTF_Font* font, SDL_Color color, int x, int y) {
        SDL_Surface* surf = TTF_RenderText_Blended(font, message, color);
        if (surf == NULL) {
            SDL_FreeSurface(surf);
            logSDLErrorAndQuit("TTF_RenderText_Blended");
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        if (texture == NULL) {
            SDL_DestroyTexture(texture);
            logSDLErrorAndQuit("SDL_CreateTextureFromSurface");
        }

        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        TTF_SizeText(font, message, &dst.w, &dst.h);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
    }
public:
    void renderScore(SDL_Renderer* renderer, TTF_Font* font, int score, int x, int y) {
        char text[256];
        snprintf(text, sizeof(text), "SCORE: %d", score);
        SDL_Color color = { 255, 255, 255, 255 }; // 흰색
        renderText(renderer, text, font, color, x, y);
    }
};