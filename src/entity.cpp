#include "entity.h"
#include "game.h"
#include "sprite.h"
#include "player.h"
#include "camera.h"
#include "stage.h"

Sprite* Bullet::sprite = nullptr;
Sprite* PBullet::sprite = nullptr;
std::vector<Sprite*> Enemy::sprites;

Bullet::Bullet(const Player& p, Vector2 pos)
{
    this->position = pos;
    this->speed = 80.0f;

    direction = p.position - position;
    direction.normalize();
    isUsed = true;
    damage = 1;
}

void Bullet::setSprite()
{
    Bullet::sprite = new Sprite("../data/enemBullet.tga", 10, 13, 4);
}

void Bullet::drawEntity(Image& framebuffer, const Camera& camera)
{
    Image img = sprite->sprite;
    int num = sprite->num;
    int spriteNum = int(Game::instance->time*7)%num;
    framebuffer.drawImage( img, position.x - camera.position.x + camera.half.x, position.y - camera.position.y + camera.half.y, Area(spriteNum*sprite->width, 0, sprite->width, sprite->height) );
}

void Bullet::updatePos(float seconds_elapsed)
{ 
    this->position += direction*speed*seconds_elapsed; 

    if (position.x < 0 || position.x > 450 || position.y < 0 || position.y > 300) isUsed = false;
}

PBullet::PBullet(const Enemy& e, Vector2 pos)
{
    this->position = pos;
    this->speed = 80.0f;

    direction = e.position - position;
    direction.normalize();
    isUsed = true;
    damage = 1;
}

void PBullet::setSprite()
{
    sprite = new Sprite("../data/playerBullet.tga", 11, 9, 4);
}

Enemy::Enemy(const Player& p)
{
    int directions[] = {-1, 1};
    int num1 = directions[rand()%2];
    int num2 = directions[rand()%2];

    position = p.position + Vector2((rand()%30+50)*num1, (rand()%30+50)*num2);
    isUsed = (position.x > 0 && position.x < 450) && (position.y > 0 && position.y < 300);
    life = 1 + int(Game::instance->time)/60;
    speed = 30.0f + 5*int(Game::instance->time)/60;
    startFire = 0;
}

void Enemy::setSprite()
{
    sprites = std::vector<Sprite*>(3);

    sprites[0] = new Sprite("../data/leftEnemy.tga" , 22, 25, 4);
    sprites[1] = new Sprite("../data/rightEnemy.tga", 22, 25, 4);
    sprites[2] = new Sprite("../data/enemyDeath1.tga", 22, 21, 5);
}

void Enemy::updatePos(const Player& p, float seconds_elapsed)
{
    if (life == 0) return;
    float distance = this->position.distance(p.position);
    moving = (distance >= 30);

    Vector2 direction = p.position - this->position;
    direction.normalize();

    if (!fireCD)
    {
        *PlayingStage::instance->findFreeBullet() = Bullet(p, this->position);
        fireCD = true;
        startFire = Game::instance->time;
    }
    fireCD = int(Game::instance->time-startFire) != 5;

    if(moving) this->position += direction*this->speed*seconds_elapsed;
}

void Enemy::drawEntity(Image& framebuffer, const Camera& camera){
    Sprite* sprite = (dead) ? sprites[2] : sprites[direction.x > 0];
    Image img = sprite->sprite;
    int num = sprite->num;
    int spriteNum = (dead) ? (int(Game::instance->time-startDeath)*7)%num : int(Game::instance->time*7)%num;
    framebuffer.drawImage( img, position.x - camera.position.x + camera.half.x, position.y - camera.position.y + camera.half.y, Area(spriteNum*sprite->width, 0, sprite->width, sprite->height) );

    isUsed = !((spriteNum == num-1) && dead);
}

bool Enemy::compare(Vector2 a, Vector2 b)
{
    return (a.x > b.x && a.x < b.x+sprites[0]->width-1 && a.y > b.y && a.y < b.y+sprites[0]->height-1);
}

bool Enemy::inHitbox(PBullet* b)
{
    Vector2 point = b->position;
    Vector2 bSize  = Vector2(b->sprite->height, b->sprite->height);
    return compare(point, position) || compare(point+bSize, position);
}

void PBullet::drawEntity(Image& framebuffer, const Camera& camera)
{
    Image img = sprite->sprite;
    int num = sprite->num;
    int spriteNum = int(Game::instance->time*7)%num;
    framebuffer.drawImage( img, position.x - camera.position.x + camera.half.x, position.y - camera.position.y + camera.half.y, Area(spriteNum*sprite->width, 0, sprite->width, sprite->height) );
}
