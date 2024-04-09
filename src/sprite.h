#ifndef SPRITE_H
#define SPRITE_H

#include "json.hpp"
#include <fstream>
#include <iostream>
#include "image.h"
#include <vector>

class Sprite{

public:
    Image sprite;
    int width;
    int height;
    int num;

    Sprite() { sprite = Image(); };

    Sprite(const char* filename, int width, int height, int num) 
    {
        sprite.loadTGA(filename);

        this->width  = width;
        this->height = height;
        this->num    = num;
    };
};

#endif //SPRITE_H