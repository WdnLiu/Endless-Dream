#include "player.h"
#include "game.h"
#include "camera.h"
#include "entity.h"

#define START_LIFE 5

Player::Player(Vector2 size, int speed)
{
    std::vector<Sprite*> tmp(4);
    sprites.push_back(tmp);
    sprites.push_back(tmp);
    sprites.push_back(tmp);

    this->size = size;
    this->life = START_LIFE;
    direction = DOWN_FACING;
    killCount = 0;

    moving = false;
    targetable = true;
    rolling = false;
    dead = false;
    revive = false;
    isHit = false;

    position = Vector2(100.0f, 100.0f);
    this->speed = speed;
    rollCD = 0;
}

void Player::restart()
{
    direction = DOWN_FACING;

    moving = false;
    targetable = true;
    rolling = false;
    dead = false;
    isHit = false;
    revive = true;

    position = Vector2(100.0f, 100.0f);
    rollCD = 0;

    startRevive = Game::instance->time;

    life = START_LIFE;
    killCount = 0;
}

void Player::loadTGA(const char* filename, int width, int height, int num, sDir dir, sState state) 
{
    sprites[state][dir] = new Sprite(filename, width, height, num); 
}

void Player::animate(Image& framebuffer, const Camera& camera)
{
    Sprite sprite = *sprites[moving][direction];
    Image img = sprite.sprite;
    int num = sprite.num;
    int spriteNum = int(Game::instance->time*7)%num;
    framebuffer.drawImage( img, position.x - camera.position.x + camera.half.x, position.y - camera.position.y + camera.half.y, Area(spriteNum*sprite.width, 0, sprite.width, sprite.height) );
}


void Player::animateRoll(Image& framebuffer, int spriteNum, const Camera& camera)
{
    Sprite sprite = *sprites[2][direction];
    Image img = sprite.sprite;

    framebuffer.drawImage( img, position.x - camera.position.x + camera.half.x, position.y - camera.position.y + camera.half.y, Area(spriteNum*sprite.width, 0, sprite.width, sprite.height) );
}

void Player::animate(Image& framebuffer, int spriteNum, const Camera& camera, int mode)
{
    Sprite* sprite;
    if (mode == 0){
        sprite = deathAnim;
    }
    else{
        sprite = reviveAnim;
    }
    framebuffer.drawImage( sprite->sprite, camera.half.x, camera.half.y, Area(spriteNum*sprite->width, 0, sprite->width, sprite->height));    
}

bool Player::compare(Vector2 a, Vector2 b)
{
    return (a.x > b.x-2 && a.x < b.x+size.x && a.y > b.y && a.y < b.y+size.y-5);
}

bool Player::inHitbox(Bullet* b)
{
    Vector2 point = b->position;
    Vector2 bSize  = Vector2(b->sprite->height, b->sprite->height);
    return compare(point, position) || compare(point+bSize, position);
}