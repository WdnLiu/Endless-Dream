#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "json.hpp"
#include <fstream>
#include <iostream>
#include "image.h"
#include "camera.h"

class GameMap { 

public:
    enum eCellType : int {
    EMPTY, START, WALL = 37, DOOR, CHEST
    };

    enum eItemType: int {
        NOTHING, SWORD, POTION
    };

    struct sCell {
        eCellType type;
        eItemType item;
    };

    struct sLayer {
        sCell* data;
    };

    int width;
    int height;
    sCell* data;
    sLayer* layers;
    int nLayers;

    GameMap() {
        width = height = 0;
        data = nullptr;
    }

    GameMap(int w, int h) {
        width = w;
        height = h;
        data = new sCell[w * h];
    }

    sCell& getCell(int x, int y) {
        return data[x + y * width];
    }

    sCell& getCell(int x, int y, int l) {
        int cellIndex = x + y * this->width; 
        return this->layers[l].data[cellIndex];
    }

    bool loadGameMap(const char* filename);
    void drawCell(int x, int y, const sCell& cell);
    void drawMap(Image& framebuffer, const Image& tileset, const Vector2& playerPos, const Camera& camera);
};

#endif