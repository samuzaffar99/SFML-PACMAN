#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Map
{
private:
    static const int ROWS = 51;
    static const int COLS = 46;
    static const int TILE_SIZE = 15;
    const int grid[ROWS][COLS];

public:
    Map();
    void draw(RenderWindow &window) const;
    bool isWall(int row, int col) const;
    bool isFood(int row, int col) const;

    // Accessors
    static int getRows() { return ROWS; }
    static int getCols() { return COLS; }
    static int getTileSize() { return TILE_SIZE; }

    friend class FoodManager;
};