#include "Tank.h"
#include "Building.h"

#include "core/engine.h"
#include "utils/gl_utils.h"


using namespace std;
using namespace m1;

Tank::Tank()
{
}

Tank::~Tank()
{
}

Tank::Tank(glm::vec3 pos, glm::vec3 dir, glm::vec3 tCColor, glm::vec3 bColor, glm::vec3 tColor, float fireInt)
{
	this->pos = pos;
	this->dir = this->turretDir = this->cannonDir = dir;

	this->trackCannonColor = tCColor;
	this->bodyColor = bColor;
	this->turretColor = tColor;

	this->angle = this->turretAngle = this->cannonAngle = 0;

	this->timeElapsedSinceFiring = 2;
	this->fireInterval = fireInt;

	this->hp = 3;

	this->state = MovementState::Forward;
	this->stateInterval = 2 + (float)(rand()) / (float)(RAND_MAX / 2.0f);
}

glm::vec3 Tank::getTracksCannonColor()
{
	return trackCannonColor;
}

glm::vec3 Tank::getBodyColor()
{
	return bodyColor;
}

glm::vec3 Tank::getTurretColor()
{
	return turretColor;
}

float Tank::getSpeed()
{
	return speed;
}

float Tank::getAngularSpeed()
{
	return angularSpeed;
}

int Tank::getHP()
{
	return hp;
}

float Tank::getTimeElapsed()
{
	return timeElapsedSinceFiring;
}

void Tank::incrementTimeElapsed(float deltaTime)
{
	timeElapsedSinceFiring += deltaTime;
}

void Tank::resetTimeElapsed()
{
	timeElapsedSinceFiring = 0;
}

bool Tank::canFire()
{
	return timeElapsedSinceFiring > fireInterval;
}

glm::vec3 Tank::getPosition()
{
	return pos;
}

glm::vec3 Tank::getTankDir()
{
	return dir;
}

glm::vec3 Tank::getTurretDir()
{
	return turretDir;
}

glm::vec3 Tank::getCannonDIr()
{
	return cannonDir;
}

float Tank::getAngle()
{
	return angle;
}

float Tank::getTurretAngle()
{
	return turretAngle;
}

float Tank::getCannonAngle()
{
	return cannonAngle;
}

void Tank::TranslateTank(float distance)
{
	pos += distance * dir;
}

void Tank::TranslateTankDir(glm::vec3 dist)
{
	pos += dist;
}

void Tank::RotateTank(float deltaAngle)
{
	angle += deltaAngle;
	dir = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), deltaAngle, glm::vec3(0, 1, 0)) * glm::vec4(dir, 1)));
	turretDir = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), deltaAngle, glm::vec3(0, 1, 0)) * glm::vec4(turretDir, 1)));
}

void Tank::RotateTurret(float deltaAngle)
{
	turretAngle += deltaAngle;
	turretDir = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), deltaAngle, glm::vec3(0, 1, 0)) * glm::vec4(turretDir, 1)));
}

glm::vec3 Tank::getCannonTipPos()
{
	return pos + glm::vec3(0, 1.15f, 0) + 1.15f * turretDir;
}

bool Tank::isHit(Projectile projectile)
{
	return glm::distance(pos, projectile.getPosition()) <= 2.5f;
}

void Tank::decrementHP()
{
	if (hp > 0)
	{
		hp -= 1;
	}
}

bool Tank::isActive()
{
	return hp > 0;
}

float Tank::collidesWithTank(Tank tank)
{
	return 3 - glm::distance(tank.getPosition(), pos);
}

void Tank::decrementStateInterval(float deltaTime)
{
	stateInterval -= deltaTime;
}

bool Tank::stateActive()
{
	return stateInterval >= 0;
}

void Tank::generateNewState()
{
	int choice = rand() % 2;

	switch (state) {
		case MovementState::Forward:
		case MovementState::Backward:
			state = (choice == 1) ? MovementState::RotateLeft : MovementState::RotateRight;
			stateInterval = (float)(rand()) / (float)(RAND_MAX / 2.0f);
			break;

		case MovementState::RotateLeft:
		case MovementState::RotateRight:
			state = (choice == 1) ? MovementState::Forward : MovementState::Backward;
			stateInterval = 2.0f + (float)(rand()) / (float)(RAND_MAX / 2.0f);
	}
}

void Tank::applyMovement(float deltaTime)
{
	switch (state) {
		case MovementState::Forward:
			this->TranslateTank(enemySpeed * deltaTime);
			break;
		case MovementState::Backward:
			this->TranslateTank(-enemySpeed * deltaTime);
			break;
		case MovementState::RotateLeft:
			this->RotateTank(angularSpeed * deltaTime);
			break;
		case MovementState::RotateRight:
			this->RotateTank(-speed * deltaTime);
	}
}

void Tank::aimTurret(glm::vec3 target)
{
	float newAngle;
	if ((target - pos).z > 0) {
		newAngle = acos(glm::dot(target - pos, dir) / (glm::distance(glm::vec3(0, 0, 0), dir) * glm::distance(glm::vec3(0, 0, 0), (target - pos))));
	}
	else {
		newAngle = 6.28f - acos(glm::dot(target - pos, dir) / (glm::distance(glm::vec3(0, 0, 0), dir) * glm::distance(glm::vec3(0, 0, 0), (target - pos))));
	}

	this->RotateTurret(newAngle - turretAngle);
}

