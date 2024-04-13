#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "image.h"

class Player;
class Camera;
class Sprite;
class PBullet;

class Entity
{
public:
    Vector2 position;
    Vector2 direction;
    float speed;

    Entity() {}

    virtual void drawEntity(Image& framebuffer, const Camera& camera) {}
};

class Bullet : public Entity
{
public:
    static Sprite* sprite;
    int damage;
    bool isUsed;

    Bullet() { isUsed = false; }

    Bullet(const Player& p, Vector2 pos);

    void updatePos(float seconds_elapsed);

    static void setSprite();

    void drawEntity(Image& framebuffer, const Camera& camera);
};

class Enemy : public Entity
{
public:
    static std::vector<Sprite*> sprites;
    
    bool isUsed;
    bool fireCD;
    bool moving;
    bool dead;
    
    float startFire;
    float startDeath;
    int life;
    
    Enemy() { isUsed = false; }
    Enemy(const Player& p);

    static void setSprite();

    void updatePos(const Player& p, float seconds_elapsed);

    void drawEntity(Image& framebuffer, const Camera& camera);
    void drawDeath(Image& framebuffer, int spriteNum, const Camera& camera);

    bool inHitbox(PBullet* b);
    bool compare(Vector2 a, Vector2 b);
};

class PBullet : public Bullet
{
public:
    static Sprite* sprite;

    PBullet() { isUsed = false; }
    PBullet(const Enemy& e, Vector2 pos); 

    void drawEntity(Image& framebuffer, const Camera& camera);
    static void drawEntity(Image& framebuffer, Vector2 v);

    static void setSprite();
};

#endif //ENTITY_H