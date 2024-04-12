#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "image.h"

class Player;
class Camera;
class Sprite;

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

    Bullet() : Entity() { isUsed = false; }

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
    float startFire;
    int life;
    
    Enemy() : Entity() { isUsed = false; }
    Enemy(const Player& p);

    static void setSprite();

    void updatePos(const Player& p, float seconds_elapsed);

    void drawEntity(Image& framebuffer, const Camera& camera);
};

#endif //ENTITY_H