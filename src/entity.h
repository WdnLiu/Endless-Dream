#ifndef ENTITY_H
#define ENTITY_H

#include "player.h"

class Entity
{
public:
    Vector2 position;
    Vector2 direction;
    float speed;
    int damage;
    bool isUsed;

    Entity() { isUsed = false; }

    Entity(const Player& p)
    {
        this->position = p.position + Vector2(rand()%30+50, rand()%30+50);
        this->speed = rand()%100;
        this->damage = 1;

        direction = p.position - position;
        direction.normalize();
        isUsed = true;
    }

    Entity(const Player& p, Vector2 position, float speed, int damage)
    {
        this->position = position;
        this->speed = speed;
        this->damage = damage;

        direction = p.position - position;
        direction.normalize();
        isUsed = true;
    }

    void updatePos(float time_elapsed) 
    { 
        this->position += direction*speed*time_elapsed; 

        if (position.x < 0 || position.x > 450 || position.y < 0 || position.y > 300) isUsed = false;
    }

    void drawEntity(Image& framebuffer, const Camera& camera);

};

#endif //ENTITY_H