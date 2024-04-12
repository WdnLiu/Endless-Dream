#include "stage.h"
#include "input.h"
#include "player.h"
#include "entity.h"
#include "gameMap.h"
#include "game.h"

IntroStage::IntroStage()
{
    Image* bg1 = new Image();
    Image* bg2 = new Image();
	Image* bg3 = new Image();

    bg1->loadTGA("../data/startImg1.tga");
    bg2->loadTGA("../data/startImg2.tga");
	bg3->loadTGA("../data/transitionBG.tga");

    bgSprites.push_back(bg1);
    bgSprites.push_back(bg2);
	bgSprites.push_back(bg3);

	currentStage = this;
	transition =  false;
}

void IntroStage::render(Image& framebuffer, const Image& minifont)
{
    if (!transition) framebuffer.drawImage(*bgSprites[int(Game::instance->time*10)%2], 0, 0);

	else {
		framebuffer.drawImage(*bgSprites[2], 0, 0);
		framebuffer.drawText("Your alarm is about to wake you up!", 1, 10, minifont, 4, 6);
		framebuffer.drawText("Kill enemies to snooze!", 1, 20, minifont, 4, 6);
		if (int(Game::instance->time*10)%2) framebuffer.drawText("Press Z to continue!", 1, 30, minifont, 4, 6);

		framebuffer.drawText("Controls:", 1, framebuffer.height - 50, minifont, 4, 6);
		framebuffer.drawText("Use your arrow keys to move", 1, framebuffer.height - 40, minifont, 4, 6);
		framebuffer.drawText("Press x to roll and dodge enemy bullets!", 1, framebuffer.height - 30, minifont, 4, 6);
		framebuffer.drawText("Press z to cast a fireball towards", 1, framebuffer.height - 20, minifont, 4, 6);
		framebuffer.drawText("closest enemy! (WIP)", 1, framebuffer.height - 10, minifont, 4, 6);

	}
}


void IntroStage::update(float seconds_elapsed)
{
    if (Input::wasKeyPressed(SDL_SCANCODE_Z))
	{		
		if (transition) switchStage();
		transition = true;
	}
}

void IntroStage::switchStage()
{
	currentStage = PlayingStage::instance;
}

PlayingStage::PlayingStage()
{
    player = new Player(Vector2(15, 20), 60.0f);
    camera = new Camera(player->position.x, player->position.y);
	map = new GameMap();
    map->loadGameMap("../data/testMap.tmj");
    tileset = new Image();
	tileset->loadTGA("../data/Tiles.tga");
	startTime = Game::instance->time;

	bullets = std::vector<Bullet*> (MAX_BULLETS);
	enemies = std::vector<Enemy* > (MAX_ENEMIES);

	for (int i = 0; i < MAX_BULLETS; ++i) bullets[i] = new Bullet();
	for (int i = 0; i < MAX_ENEMIES; ++i) enemies[i] =  new Enemy();

	player->loadTGA("../data/idleDown.tga", 15, 20, 4, DOWN_FACING, IDLE);
	player->loadTGA("../data/idleUp.tga", 15, 20, 4, UP_FACING, IDLE);
	player->loadTGA("../data/idleRight.tga", 16, 20, 4, RIGHT_FACING, IDLE);
	player->loadTGA("../data/idleLeft.tga", 16, 20, 4, LEFT_FACING, IDLE);

	player->loadTGA("../data/walkRight.tga", 16, 21, 6, RIGHT_FACING, WALKING);
	player->loadTGA("../data/walkLeft.tga", 16, 21, 6, LEFT_FACING, WALKING);
	player->loadTGA("../data/walkDown.tga", 16, 21, 6, DOWN_FACING, WALKING);
	player->loadTGA("../data/walkUp.tga", 16, 21, 6, UP_FACING, WALKING);

	player->loadTGA("../data/rollDown.tga", 17, 25, 9, DOWN_FACING, ROLLING);
	player->loadTGA("../data/rollUp.tga", 17, 24, 9, UP_FACING, ROLLING);
	player->loadTGA("../data/rollRight.tga", 19, 22, 9, RIGHT_FACING, ROLLING);
	player->loadTGA("../data/rollLeft.tga", 19, 22, 9, LEFT_FACING, ROLLING);

	player->loadDeathAnim("../data/deathAnimation.tga", 19, 25, 12);

	currentStage = this;
}

void PlayingStage::restart()
{
	delete camera;
	for (Bullet* b : bullets) delete b; 

	player->restart();
    camera = new Camera(player->position.x, player->position.y);
	startTime = Game::instance->time;
	startDeath = 0;

	for (int i = 0; i < MAX_BULLETS; ++i) bullets[i] = new Bullet();
	spriteNum = 0;

	currentStage = this;
}

Bullet* PlayingStage::findFreeBullet()
{
	for (Bullet* b : bullets)
	{
		if (b->isUsed)
			continue;
		
		b->isUsed = true;
		return b;
	}

	std::cout << "No more bullets" << std::endl;
	return bullets[0];
}

Enemy* PlayingStage::findFreeEnemy()
{
	for (Enemy* e : enemies)
	{
		if (e->isUsed)
			continue;

		e->isUsed = true;
		return e;
	}
}

void PlayingStage::drawBullets(Image& framebuffer)
{
	for (Bullet* b : bullets)
	{
		if (b->isUsed) {
			b->updatePos(Game::instance->elapsed_time);
			b->drawEntity(framebuffer, *camera);
		}
	}
}

void PlayingStage::drawEnemies(Image& framebuffer)
{
	for (Enemy* e : enemies)
	{
		if (e->isUsed) {
			e->updatePos(*player, Game::instance->elapsed_time);
			e->drawEntity(framebuffer, *camera);
		}
	}
}

void PlayingStage::render(Image& framebuffer, const Image& minifont)
{

    map->drawMap(framebuffer, *tileset, player->position, *camera);

	framebuffer.drawText( toString(camera->position.x) + ':' + toString(camera->position.y), 1, 10, minifont, 4, 6 );
	framebuffer.drawText( toString(player->position.x) + ':' + toString(player->position.y), 1, 17, minifont, 4, 6 );
	framebuffer.drawText( toString(player->life), 1, 25, minifont, 4, 6 );

	if (player->rolling)
	{
		player->animateRoll(framebuffer, spriteNum, *camera);
		spriteNum = int ((Game::instance->time-startRoll)*20) %9;
	}
	else if (player->dead)
	{
		player->animateDeath(framebuffer, spriteNum, *camera);
		spriteNum = int((Game::instance->time-startDeath)*10)%player->deathAnim->num;
		return;
	}
	else player->animate(framebuffer, *camera);

	if (player->rollCD > 0) player->rollCD = (int(Game::instance->time-startRoll) == 5) ? false : true;

	drawBullets(framebuffer);
	drawEnemies(framebuffer);
}

void PlayingStage::update(float seconds_elapsed)
{
	if (player->dead) {
		if (spriteNum == player->deathAnim->num-1) switchStage();
		return;	
	}
    Vector2 playerSpeed = Vector2(0.0f, 0.0f);
	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		playerSpeed.y -= 1;
		player->direction = UP_FACING;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		playerSpeed.y += 1;
		player->direction = DOWN_FACING;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT))
	{
		playerSpeed.x += 1;
		player->direction = RIGHT_FACING;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT))
	{
		playerSpeed.x -= 1;
		player->direction = LEFT_FACING;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_X) && !player->rollCD)
	{
		player->rolling = true;
		startRoll = Game::instance->time;
		spriteNum = 0;
		player->rollCD = true;
		player->targetable = false;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) {
		*findFreeEnemy() = Enemy(*player);
	}

	target = player->position+playerSpeed;

	if (playerSpeed.x != 0 && playerSpeed.y != 0) {
		playerSpeed.normalize();
	}
	if (playerSpeed.x != 0 || playerSpeed.y != 0) {
		player->moving = true;
	}
	else player->moving = false;

	if (player->moving) {
		player->position.x = clamp(player->position.x + playerSpeed.x*player->speed*seconds_elapsed, 15, 450);
		player->position.y = clamp(player->position.y + playerSpeed.y*player->speed*seconds_elapsed, 0, 285);
	}
	camera->position.x = clamp(player->position.x, 15, 500);
	camera->position.y = clamp(player->position.y, 0, 335);

	for (Bullet* b : bullets)
	{
		if (!player->targetable) break;
		if (!b->isUsed) continue;

		if (player->inHitbox(b))
		{
			player->life -= b->damage;
			b->isUsed = false;
		}
	}

	if (player->life  == 0 && !player->dead) 
	{
		startDeath = Game::instance->time;
		player->dead = true;
	}

	if(!player->targetable) player->targetable = (Game::instance->time - player->startRoll == 1);
}

void PlayingStage::switchStage()
{
	currentStage = EndingStage::instance;
	EndingStage::instance->currentStage = EndingStage::instance;
	EndingStage::instance->finalTime = (startDeath-startTime);
}

EndingStage::EndingStage()
{
	endScene = new Image();
	endScene->loadTGA("../data/endScene.tga");

	currentStage = this;
}

void EndingStage::render(Image& framebuffer, const Image& minifont)
{
	framebuffer.drawImage(*endScene, 0, 0);

	framebuffer.drawText( "Minutes late to class: " + toString(finalTime), 1, framebuffer.height-30, minifont, 4, 6);
	if (int(Game::instance->time*10)%2) framebuffer.drawText("Press R to Restart!", 45, framebuffer.height-10, minifont, 4, 6);

}

void EndingStage::update(float seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_R))
	{
		switchStage();
	}
}

void EndingStage::switchStage()
{
	PlayingStage::instance->restart();
	currentStage = PlayingStage::instance;
}