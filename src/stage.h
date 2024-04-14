#ifndef STAGE_H
#define STAGE_H

#define MAX_BULLETS 100
#define MAX_ENEMIES 20

#include <vector>
#include "image.h"

class Player;
class Entity;
class Camera;
class GameMap;
class Bullet;
class Enemy;
class PBullet;
class Sprite;

enum eStages { INTRO_STAGE, PLAYING_STAGE, ENDING_STAGE };
enum eEntity { eBULLET, eENEMY, ePBULLET };

class Stage
{
public:
    Stage() {};
    virtual void render(Image& framebuffer, const Image& minifont) {}
    virtual void update(float seconds_elapsed) {}

    Stage* currentStage;

    virtual void switchStage() {}
};

class IntroStage : public Stage
{
public:
    static IntroStage* instance;
    std::vector<Image*> bgSprites;
    float startTime;
    bool transition;

    IntroStage();
    void render(Image& framebuffer, const Image& minifont);
    void update(float seconds_elapsed);
    void switchStage();
};

class PlayingStage : public Stage
{
public:
    static PlayingStage* instance;
    Image* life;
    Sprite* rollInd;
    Player* player;

    std::vector<PBullet*> pBullets;
    std::vector<Bullet* > bullets;
    std::vector<Enemy*  > enemies;

    float startTime;
    float currentTick;
    int   spriteNum;
    
    Vector2  target;
    Camera*  camera;
    Image*   tileset;
    GameMap* map;

    PlayingStage();
    void restart();
    void render(Image& framebuffer, const Image& minifont);
    void update(float seconds_elapsed);

    void switchStage();

    void generateEnemies();

    PBullet* findFreePBullet();
    Bullet*  findFreeBullet();
    Enemy*   findFreeEnemy();
    Enemy*   findClosestEnemy();

    void drawBullets( Image& framebuffer);
    void drawEnemies( Image& framebuffer);
    void drawPBullets(Image& framebuffer);
};

class EndingStage : public Stage
{
public:
    static EndingStage* instance;
    float finalTime;
    int score;
    Image* endScene;

    EndingStage();

    void render(Image& framebuffer, const Image& minifont);
    void update(float seconds_elapsed);

    void switchStage();
};

#endif //STAGE_H
