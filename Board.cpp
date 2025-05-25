#include "Board.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

Board::Board(SDL_Renderer* r) : renderer(r) {
    std::srand(std::time(nullptr));
    fillRandom();
}

void Board::fillRandom() {
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            grid[y][x].color = static_cast<Color>(std::rand() % 4);
}

SDL_Color Board::getSDLColor(Color color) {
    switch (color) {
    case Color::RED: return { 255, 0, 0, 255 };
    case Color::YELLOW: return { 255, 255, 0, 255 };
    case Color::BLUE: return { 0, 0, 255, 255 };
    case Color::WHITE: return { 255, 255, 255, 255 };
    }
    return { 0, 0, 0, 255 };
}

void Board::render() {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            SDL_Rect rect = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE - 2, CELL_SIZE - 2 };
            SDL_Color c = getSDLColor(grid[y][x].color);
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Board::handleEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x / CELL_SIZE;
        int y = event.button.y / CELL_SIZE;
        if (selected.first == -1)
            selected = { x, y };
        else {
            if (isValidSwap(selected.first, selected.second, x, y)) {
                swap(selected.first, selected.second, x, y);
                if (!checkMatches()) {
                    swap(selected.first, selected.second, x, y); // откат
                }
            }
            selected = { -1, -1 };
        }
    }
}

bool Board::isValidSwap(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) + abs(y1 - y2)) == 1;
}

void Board::swap(int x1, int y1, int x2, int y2) {
    std::swap(grid[y1][x1], grid[y2][x2]);
}

bool Board::checkMatches() {
    bool found = false;

    // Clear previous marks
    for (auto& row : grid)
        for (auto& cell : row)
            cell.toBeDestroyed = false;

    // Check rows
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE - 2; ++x) {
            if (grid[y][x].color == grid[y][x + 1].color && grid[y][x].color == grid[y][x + 2].color) {
                grid[y][x].toBeDestroyed = grid[y][x + 1].toBeDestroyed = grid[y][x + 2].toBeDestroyed = true;
                found = true;
            }
        }
    }

    // Check columns
    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE - 2; ++y) {
            if (grid[y][x].color == grid[y + 1][x].color && grid[y][x].color == grid[y + 2][x].color) {
                grid[y][x].toBeDestroyed = grid[y + 1][x].toBeDestroyed = grid[y + 2][x].toBeDestroyed = true;
                found = true;
            }
        }
    }

    return found;
}

void Board::destroyMatches() {
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            if (grid[y][x].toBeDestroyed) {
                if (std::rand() % 5 == 0) spawnBonuses(x, y);
                grid[y][x].color = static_cast<Color>(std::rand() % 4);
                grid[y][x].toBeDestroyed = false;
            }
}

void Board::applyGravity() {
    for (int x = 0; x < SIZE; ++x) {
        int write = SIZE - 1;
        for (int y = SIZE - 1; y >= 0; --y) {
            if (!grid[y][x].toBeDestroyed) {
                grid[write][x] = grid[y][x];
                --write;
            }
        }
        while (write >= 0) {
            grid[write][x].color = static_cast<Color>(std::rand() % 4);
            grid[write][x].toBeDestroyed = false;
            --write;
        }
    }
}

void Board::update() {
    if (checkMatches()) {
        destroyMatches();
        applyGravity();
    }
}

void Board::spawnBonuses(int x, int y) {
    int bonusX = x + (std::rand() % 7) - 3;
    int bonusY = y + (std::rand() % 7) - 3;

    if (bonusX < 0 || bonusX >= SIZE || bonusY < 0 || bonusY >= SIZE)
        return;

    triggerBonus(bonusX, bonusY);
}

void Board::triggerBonus(int x, int y) {
    if (std::rand() % 2 == 0) { // Перекраска
        Color c = grid[y][x].color;
        int changed = 0;
        for (int dy = -3; dy <= 3; ++dy) {
            for (int dx = -3; dx <= 3; ++dx) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE) {
                    if (std::abs(nx - x) + std::abs(ny - y) > 1 && changed < 2) {
                        grid[ny][nx].color = c;
                        changed++;
                    }
                }
            }
        }
    }
    else { // Бомба
        for (int i = 0; i < 5; ++i) {
            int bx = std::rand() % SIZE;
            int by = std::rand() % SIZE;
            grid[by][bx].toBeDestroyed = true;
        }
    }
}