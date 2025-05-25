#include "Game.hpp"
#include <iostream>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

Game::Game() : window(nullptr), renderer(nullptr), running(false), board(nullptr) {}

Game::~Game() {}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    window = SDL_CreateWindow("GEMS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    board = new Board(renderer);
    running = true;
    return true;
}

void Game::run() {
    while (running) {
        handleEvents();
        board->update();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        board->render();
        SDL_RenderPresent(renderer);
        SDL_Delay(60);
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;
        else
            board->handleEvent(event);
    }
}

void Game::cleanup() {
    delete board;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
