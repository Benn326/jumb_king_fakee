#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "platform.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SIZE = 50;
const int PLAYER_SPEED = 4;
const int GRAVITY = 1;
const int MAX_JUMP = 18;
const int MIN_JUMP = 7;
const int MAP_WIDTH = 800;
const int MAP_HEIGHT = 2400;
const int FRAME_COUNT = 15;
const int FRAME_DURATION = 100; // ms, điều chỉnh cho phù hợp


class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void close();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect camera;
    bool isRunning;
    int playerX, playerY;
    int playerVelX, playerVelY;
    bool isJumping;
    bool jumpKeyHeld;
    int jumpCharge;
    std::vector<Platform> platforms;
    Uint32 jumpStartTime;
    int facing;
    //Nhân vật 
    SDL_Texture* playerTexture; // Texture của spritesheet nhân vật
    int currentFrame;           // Frame hiện tại (0 - 14)
    Uint32 lastFrameTime;       // Thời gian cập nhật frame cuối cùng

    void handleEvents();
    void update();
    void render();
    void checkCollisions();
};

#endif  
