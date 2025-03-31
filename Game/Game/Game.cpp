#include "Game.h"
#include <iostream>

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    isRunning = false;
    playerX = SCREEN_WIDTH / 2;
    playerY = SCREEN_HEIGHT - PLAYER_SIZE;
    playerVelX = 0;
    playerVelY = 0;
    isJumping = false;
    jumpKeyHeld = false;
    jumpCharge = MIN_JUMP;
    platforms.push_back(Platform(300, 450, 200, 20));
    platforms.push_back(Platform(100, 350, 150, 20));
    platforms.push_back(Platform(500, 250, 180, 20));
}

Game::~Game() {
    close();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize!" << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Jump Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    isRunning = true;
    return true;
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                playerVelX = -PLAYER_SPEED;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                playerVelX = PLAYER_SPEED;
                break;
            case SDLK_SPACE:
                if (!isJumping && !jumpKeyHeld) {
                    jumpKeyHeld = true;
                    jumpCharge = MIN_JUMP;
                    jumpStartTime = SDL_GetTicks();  // Lưu thời gian bắt đầu giữ phím
                }
                break;
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                if (jumpKeyHeld && !isJumping) {
                    Uint32 holdTime = SDL_GetTicks() - jumpStartTime;  // Tính thời gian giữ
                    jumpCharge = MIN_JUMP + (holdTime / 50);  // Tăng lực nhảy dựa trên thời gian giữ
                    if (jumpCharge > MAX_JUMP) jumpCharge = MAX_JUMP; // Giới hạn lực nhảy
                    playerVelY = -jumpCharge; // Áp dụng lực nhảy
                    isJumping = true;
                }
                jumpKeyHeld = false;
            }
            else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a ||
                e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
                playerVelX = 0; // Dừng di chuyển ngang khi thả phím
            }
        }
    }
}
void Game::checkCollisions() {
    for (Platform& platform : platforms) {
        if (playerX + PLAYER_SIZE > platform.x && playerX < platform.x + platform.w &&
            playerY + PLAYER_SIZE >= platform.y && playerY + PLAYER_SIZE - playerVelY < platform.y && playerVelY > 0) {
            playerY = platform.y - PLAYER_SIZE; // Đặt nhân vật đứng trên platform
            playerVelY = 0;
            isJumping = false;
        }
    }
}

void Game::update() {
    const int MAX_FALL_SPEED = 20;  // Giới hạn tốc độ rơi

    playerX += playerVelX;
    playerY += playerVelY;

    // Áp dụng trọng lực
    playerVelY += GRAVITY;
    if (playerVelY > MAX_FALL_SPEED) playerVelY = MAX_FALL_SPEED;
	checkCollisions();
    // Nếu rơi xuống đất, reset trạng thái nhảy
    if (playerY >= SCREEN_HEIGHT - PLAYER_SIZE) {
        playerY = SCREEN_HEIGHT - PLAYER_SIZE;
        playerVelY = 0;
        isJumping = false;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
	//Vẽ platform
    for (Platform& platform : platforms) {
        platform.render(renderer);
    }
    // Vẽ nhân vật
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect playerRect = { playerX, playerY, PLAYER_SIZE, PLAYER_SIZE };
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderPresent(renderer);

}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);  // Giới hạn tốc độ khung hình (~60 FPS)
    }
}

void Game::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
