#include "entity.h"

void Entity::drawEntity(Image& framebuffer, const Camera& camera)
{
    framebuffer.drawLine( position.x- camera.position.x + framebuffer.width/2, position.y - camera.position.y + framebuffer.height/2, position.x -camera.position.x + framebuffer.width/2 - direction.x*10, position.y -camera.position.y +framebuffer.height/2 - direction.y*10, Color::WHITE );
}