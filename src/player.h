#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "image.h"
#include "sprite.h"

class Bullet;

enum sDir { DOWN_FACING = 0, RIGHT_FACING, LEFT_FACING, UP_FACING };
enum sState { IDLE, WALKING, ROLLING, DEAD };
class Camera;

class Player
{
public:
    std::vector<std::vector<Sprite*>> sprites;
    Sprite* deathAnim;
    Sprite* reviveAnim;

    Vector2 size;
    int life;
    int direction;
    int killCount;

    bool moving;
    bool targetable;
    bool rolling;
    bool dead;
    bool isHit;
    bool revive;

    Vector2 position;
    float speed;
    bool rollCD;
    bool fireCD;

    float startFire;
    float startRoll;
    float startDeath;
    float startHit;
    float startRevive;

    Player(Vector2 size, int speed);

    void restart();

    void loadTGA(const char* filename, int width, int height, int num, sDir dir, sState state);

    void animate(Image& framebuffer, const Camera& camera);
    void animate(Image& framebuffer, int spriteNum, const Camera& camera, int mode);
    void animateRoll(Image& framebuffer, int spriteNum, const Camera& camera);
    

    bool inHitbox(Bullet* b);
    bool compare(Vector2 a, Vector2 b);
};

#endif //PLAYER_H