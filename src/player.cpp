#include "player.h"
#include "utils.h"
#include <unistd.h>

// void Player::animate(Image& framebuffer, float time, const Camera& camera)
// {
//     Sprite sprite = sprites[moving][direction];
//     Image img = sprite.sprite;
//     int num = sprite.num;
//     int spriteNum = int(time*7)%num;
//     framebuffer.drawImage( img, framebuffer.width/2.0 - 16, framebuffer.height/2.0 - 21, Area(spriteNum*sprite.width, 0, sprite.width, sprite.height) );
// }

void Player::animate(Image& framebuffer, float time, const Camera& camera)
{
    Sprite sprite = sprites[moving][direction];
    Image img = sprite.sprite;
    int num = sprite.num;
    int spriteNum = int(time*7)%num;
    framebuffer.drawImage( img, position.x - camera.position.x + framebuffer.width/2, position.y - camera.position.y + framebuffer.height/2, Area(spriteNum*sprite.width, 0, sprite.width, sprite.height) );
}


void Player::animateRoll(Image& framebuffer, int spriteNum, const Camera& camera)
{
    Sprite sprite = sprites[2][direction];
    Image img = sprite.sprite;

    framebuffer.drawImage( img, position.x - camera.position.x + framebuffer.width/2, position.y - camera.position.y + framebuffer.height/2, Area(spriteNum*sprite.width, 0, sprite.width, sprite.height) );

    rolling = (spriteNum == sprite.num-1) ? false : true;
}