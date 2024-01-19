#include "Projectile.h"

#include "core/engine.h"
#include "utils/gl_utils.h"


using namespace std;
using namespace m1;


Projectile::Projectile(glm::vec3 pos, glm::vec3 dir, bool isEnemy)
{
	this->pos = pos;
	this->dir = dir;
	this->lifetime = 8;
	this->isEnemy = isEnemy;
}

Projectile::~Projectile()
{
}

glm::vec3 Projectile::getPosition()
{
	return pos;
}

glm::vec3 Projectile::getDir()
{
	return dir;
}

float Projectile::getSpeed()
{
	return speed;
}

bool Projectile::isEnemyProjectile()
{
	return isEnemy;
}

void Projectile::Translate(float deltaTime)
{
	pos += speed * deltaTime * dir;
	lifetime -= deltaTime;
}

bool Projectile::reachedLifetime()
{
	return lifetime <= 0;
}
