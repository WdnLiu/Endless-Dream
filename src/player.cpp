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

    moving = false;
    targetable = true;
    rolling = false;
    dead = false;

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

    position = Vector2(100.0f, 100.0f);
    rollCD = 0;
    life = START_LIFE;
}

void Player::loadTGA(const char* filename, int width, int height, int num, sDir dir, sState state) 
{
    sprites[state][dir] = new Sprite(filename, width, height, num); 
}

void Player::loadDeathAnim(const char* filename, int width, int height,  int num)
{
    deathAnim = new Sprite(filename, width, height, num);
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

    rolling = (spriteNum == sprite.num-1) ? false : true;
}

void Player::animateDeath(Image& framebuffer, int spriteNum, const Camera& camera)
{
    Image img = deathAnim->sprite;

    framebuffer.drawImage( img, camera.half.x, camera.half.y, Area(spriteNum*deathAnim->width, 0, deathAnim->width, deathAnim->height));
}

bool Player::compare(Vector2 a, Vector2 b)
{
    return (a.x > b.x && a.x < b.x+size.x-1 && a.y > b.y && a.y < b.y+size.y-1);
}

bool Player::inHitbox(Bullet* b)
{
    Vector2 point = b->position;
    Vector2 bSize  = Vector2(b->sprite->height, b->sprite->height);
    return compare(point, position) || compare(point+bSize, position);
}