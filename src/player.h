#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "sprite.h"
#include "image.h"
#include "camera.h"
// #include "entity.h"

class Player {

public:
    enum spriteDir { DOWN_FACING = 0, RIGHT_FACING, LEFT_FACING, UP_FACING };
    std::vector<std::vector<Sprite>> sprites;

    Vector2 size;
    int life;
    int direction;
    bool moving;
    bool targetable;
    bool rolling;
    Vector2 position;
    float speed;
    bool rollCD;

    Player(Vector2 size, int life, int speed)
    {
        std::vector<Sprite> tmp(4);
        sprites.push_back(tmp);
        sprites.push_back(tmp);
        sprites.push_back(tmp);

        this->size = size;
        this->life = life;
        direction = DOWN_FACING;
        moving = false;
        targetable = true;
        rolling = false;
        position = Vector2(100.0f, 100.0f);
        this->speed = speed;
        rollCD = false;
    }

    void loadIdle(int dir, const char* filename, int width, int height, int num) { sprites[0][dir] = Sprite(filename, width, height, num); }

    void loadWalk(int dir, const char* filename, int width, int height, int num) { sprites[1][dir] = Sprite(filename, width, height, num); }

    void loadRoll(int dir, const char* filename, int width, int height, int num) { sprites[2][dir] = Sprite(filename, width, height, num); }

    void animate(Image& framebuffer, float time, const Camera& camera);
    void animateRoll(Image& framebuffer, int spriteNum, const Camera& camera);

    bool inHitbox(Vector2 point) { return (point.x > position.x && point.x < position.x+size.x && point.y > position.y && point.y < position.y+size.y); }
};

#endif //PLAYER_H