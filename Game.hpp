#pragma once
#include <SDL2/SDL.h>
#include "Board.hpp"

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Board* board;
    void handleEvents();
};
