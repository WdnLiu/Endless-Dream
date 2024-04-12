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
		framebuffer.drawText("Every 10 enemies you kill will be", 1, 20, minifont, 4, 6);
		framebuffer.drawText("5 extra minutes of snooze time!", 1, 30, minifont, 4, 6);
		if (int(Game::instance->time*10)%2) framebuffer.drawText("Press Z to continue!", 1, 40, minifont, 4, 6);

		framebuffer.drawText("Controls:", 1, framebuffer.height - 50, minifont, 4, 6);
		framebuffer.drawText("Use your arrow keys to move", 1, framebuffer.height - 40, minifont, 4, 6);
		framebuffer.drawText("Press z to roll and dodge enemy bullets!", 1, framebuffer.height - 30, minifont, 4, 6);
		framebuffer.drawText("Press x to cast a fireball towards", 1, framebuffer.height - 20, minifont, 4, 6);
		framebuffer.drawText("closest enemy!", 1, framebuffer.height - 10, minifont, 4, 6);
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
	PlayingStage::instance->startTime = Game::instance->time;
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

	pBullets = std::vector<PBullet*> (MAX_BULLETS);
	bullets  = std::vector<Bullet* > (MAX_BULLETS);
	enemies  = std::vector<Enemy*  > (MAX_ENEMIES);

	for (int i = 0; i < MAX_BULLETS; ++i) pBullets[i] = new PBullet();
	for (int i = 0; i < MAX_BULLETS; ++i)  bullets[i] = new Bullet();
	for (int i = 0; i < MAX_ENEMIES; ++i)  enemies[i] = new Enemy();

	player->loadTGA("../data/idleDown.tga" , 15, 20, 4, DOWN_FACING,  IDLE);
	player->loadTGA("../data/idleUp.tga"   , 15, 20, 4, UP_FACING,    IDLE);
	player->loadTGA("../data/idleRight.tga", 16, 20, 4, RIGHT_FACING, IDLE);
	player->loadTGA("../data/idleLeft.tga" , 16, 20, 4, LEFT_FACING,  IDLE);

	player->loadTGA("../data/walkRight.tga", 16, 21, 6, RIGHT_FACING, WALKING);
	player->loadTGA("../data/walkLeft.tga" , 16, 21, 6, LEFT_FACING,  WALKING);
	player->loadTGA("../data/walkDown.tga" , 16, 21, 6, DOWN_FACING,  WALKING);
	player->loadTGA("../data/walkUp.tga"   , 16, 21, 6, UP_FACING,    WALKING);

	player->loadTGA("../data/rollDown.tga" , 17, 25, 9, DOWN_FACING,  ROLLING);
	player->loadTGA("../data/rollUp.tga"   , 17, 24, 9, UP_FACING,    ROLLING);
	player->loadTGA("../data/rollRight.tga", 19, 22, 9, RIGHT_FACING, ROLLING);
	player->loadTGA("../data/rollLeft.tga" , 19, 22, 9, LEFT_FACING,  ROLLING);

	player->deathAnim = new Sprite("../data/deathAnimation.tga", 19, 25, 12);
	player->reviveAnim = new Sprite("../data/reviveAnimation.tga", 25, 32, 14);

	currentStage = this;

	generateEnemies();
}

void PlayingStage::restart()
{
	delete camera;
	for (int i = 0; i < MAX_ENEMIES; ++i) {
		delete bullets[i*2];
		delete bullets[i*2+1];
		
		delete pBullets[i*2];
		delete pBullets[i*2+1];

		delete enemies[i];
	}
	player->restart();
    camera = new Camera(player->position.x, player->position.y);
	startTime = Game::instance->time;

	for (int i = 0; i < MAX_ENEMIES; ++i) {
		bullets[i*2] = new Bullet();
		bullets[i*2+1] = new Bullet();
		
		pBullets[i*2] = new PBullet();
		pBullets[i*2+1] = new PBullet();

		enemies[i] = new Enemy();
	}

	spriteNum = 0;

	currentStage = this;
}

PBullet* PlayingStage::findFreePBullet()
{
	for (PBullet* b : pBullets)
	{
		if (b->isUsed)
			continue;
		
		b->isUsed = true;
		return b;
	}

	std::cout << "No free bullets" << std::endl;
	return nullptr;
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

	std::cout << "No free player bullets" << std::endl;
	return nullptr;
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
	std::cout << "No free enemies" << std::endl;
	return nullptr;
}

void PlayingStage::drawPBullets(Image& framebuffer)
{
	for (PBullet* b : pBullets)
	{
		if (b->isUsed) {
			b->updatePos(Game::instance->elapsed_time);
			b->drawEntity(framebuffer, *camera);
		}
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
	
	if (player->revive)
	{
		player->animate(framebuffer, spriteNum, *camera, 1);
		spriteNum = int((Game::instance->time - player->startRevive)*10)%player->reviveAnim->num;
		return;
	}
	else if (player->dead)
	{
		player->animate(framebuffer, spriteNum, *camera, 0);
		spriteNum = int((Game::instance->time - player->startDeath)*10)%player->deathAnim->num;
		return;
	}
	else if (!player->isHit || (player->isHit && int(Game::instance->time*10)%2)){
		if (player->rolling)
		{
			player->animateRoll(framebuffer, spriteNum, *camera);
			spriteNum = int ((Game::instance->time - player->startRoll)*20) %9;
		}
		else player->animate(framebuffer, *camera);
	}

	drawBullets( framebuffer);
	drawEnemies( framebuffer);
	drawPBullets(framebuffer);

	framebuffer.drawText( "Kill count: " + toString(player->killCount), 1, 20, minifont, 4, 6 );
	framebuffer.drawText( "Times snoozed: " + toString(player->killCount/10), 1, 30, minifont, 4, 6);
	framebuffer.drawText( "Time survived: " + toString(Game::instance->time - startTime), 1, 40, minifont, 4, 6 );
}

Enemy* PlayingStage::findClosestEnemy()
{
	Enemy* closest = nullptr;
	float min = INT8_MAX;

	for (Enemy* e : enemies)
	{
		if (!e->isUsed || e->dead) continue;
		float distance = e->position.distance(player->position);
		if (distance < min) {
			min = distance;
			closest = e;
		}
	}

	return closest;
}

void PlayingStage::update(float seconds_elapsed)
{
	if (player->dead) {
		if (spriteNum == player->deathAnim->num-1) switchStage();
		return;	
	}
	if (player->revive) {
		player->revive = !(spriteNum == player->reviveAnim->num-1);
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
	if (Input::wasKeyPressed(SDL_SCANCODE_X) && !player->fireCD)
	{
		Enemy* tmp = findClosestEnemy();
		if (tmp != nullptr)
		{
			player->fireCD = true;
			player->startFire = Game::instance->time;
			*findFreePBullet() = PBullet(*tmp, player->position);
		}
		else printf("No closest enemy\n");

	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z) && !player->rollCD)
	{
		player->rolling = true;
		player->startRoll = Game::instance->time;
		spriteNum = 0;
		player->rollCD = true;
		player->targetable = false;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A))
	{
		Enemy* enemy = findFreeEnemy();
		if (enemy) *enemy = Enemy(*player);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_K))
	{
		player->dead = true;
		player->startDeath = Game::instance->time;
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
			player->isHit = true;
			player->startHit = Game::instance->time;
			player->life -= b->damage;
			b->isUsed = false;
		}
	}

	bool hit = false;
	for (PBullet* b : pBullets)
	{
		if (!b->isUsed) continue;

		for (Enemy* enemy : enemies)
		{	
			if (!enemy->isUsed) continue;
			hit = enemy->inHitbox(b);
			if (hit) {
				enemy->life -= b->damage;
				if (enemy->life == 0){
					player->killCount++;
					enemy->dead = true;
					enemy->startDeath = Game::instance->time;	
				}
				b->isUsed = false;
				break;
			}
		}
		if (hit) break;
	}

	if (player->life <= 0 && !player->dead) 
	{
		player->startDeath = Game::instance->time;
		player->dead = true;
	}

	if (!player->targetable) player->targetable = (Game::instance->time - player->startRoll > 1);
	if (player->rollCD) player->rollCD = (int(Game::instance->time - player->startRoll) > 1) ? false : true;
	if (player->fireCD) player->fireCD = (Game::instance->time - player->startFire < 0.5);
	if (player->isHit) {
		bool tmp = (Game::instance->time - player->startHit < 2);
		player->isHit = tmp;
		player->targetable = !tmp;
	}
	if (Game::instance->time - currentTick > 10) {
		currentTick = Game::instance->time;
		generateEnemies();
	}

	player->speed = (player->rolling) ? 120.0f : 60.0f;
}

void PlayingStage::generateEnemies()
{
	for (int i = 0; i < (Game::instance->time - startTime)/5 + rand()%5; ++i)
	{
		Enemy* tmp = findFreeEnemy();
		if (tmp) *tmp = Enemy(*player);
	}
}

void PlayingStage::switchStage()
{
	currentStage = EndingStage::instance;
	EndingStage::instance->currentStage = EndingStage::instance;
	EndingStage::instance->finalTime = (player->startDeath - startTime);
	EndingStage::instance->score 	 = player->killCount/10;
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

	framebuffer.drawText( "Minutes late to class: " + toString(finalTime/60 + score*5), 1, framebuffer.height-30, minifont, 4, 6);
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