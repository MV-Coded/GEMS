#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <utility>

enum class Color { RED, YELLOW, BLUE, WHITE };
struct Cell {
    Color color;
    bool toBeDestroyed = false;
    bool isBonus = false;
};

class Board {
public:
    Board(SDL_Renderer* renderer);
    void render();
    void update();
    void handleEvent(SDL_Event& event);

private:
    SDL_Renderer* renderer;
    static const int SIZE = 10;
    static const int CELL_SIZE = 60;
    Cell grid[SIZE][SIZE];
    std::pair<int, int> selected = { -1, -1 };
    void fillRandom();
    void swap(int x1, int y1, int x2, int y2);
    bool isValidSwap(int x1, int y1, int x2, int y2);
    bool checkMatches();
    void destroyMatches();
    void applyGravity();
    void spawnBonuses(int x, int y);
    void triggerBonus(int x, int y);
    SDL_Color getSDLColor(Color color);
};
