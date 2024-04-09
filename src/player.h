#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "sprite.h"
#include "image.h"
#include "camera.h"

class Player {

public:
    enum spriteDir { DOWN_FACING = 0, RIGHT_FACING, LEFT_FACING, UP_FACING };
    std::vector<std::vector<Sprite>> sprites;

    int direction;
    bool moving;
    bool targetable;
    bool rolling;
    Vector2 position;
    Vector2 offsetCam;
    float speed;

    Player()
    {
        std::vector<Sprite> tmp(4);
        sprites.push_back(tmp);
        sprites.push_back(tmp);
        sprites.push_back(tmp);

        direction = DOWN_FACING;
        moving = false;
        targetable = true;
        rolling = false;
        position = Vector2(100.0f, 100.0f);
        offsetCam = Vector2(0.0f, 0.0f);
        speed = 60.0f;
    }

    void loadIdle(int dir, const char* filename, int width, int height, int num) { sprites[0][dir] = Sprite(filename, width, height, num); }

    void loadWalk(int dir, const char* filename, int width, int height, int num) { sprites[1][dir] = Sprite(filename, width, height, num); }

    void loadRoll(int dir, const char* filename, int width, int height, int num) { sprites[2][dir] = Sprite(filename, width, height, num); }

    void animate(Image& framebuffer, float time, const Camera& camera);
    void animateRoll(Image& framebuffer, int spriteNum, const Camera& camera);
};

#endif //PLAYER_H