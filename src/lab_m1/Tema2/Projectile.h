#ifndef PROJECTILE_H
#define PROJECTILE_H


#include "core/gpu/mesh.h"
#include "components/simple_scene.h"

namespace m1 {
	class Projectile
	{
	public:
		Projectile(glm::vec3 pos, glm::vec3 dir, bool isEnemy);
		~Projectile();

		glm::vec3 getPosition();
		glm::vec3 getDir();
		float getSpeed();
		bool isEnemyProjectile();

		void Translate(float deltaTime);
		bool reachedLifetime();

	protected:
		glm::vec3 pos, dir;
		float speed = 10;
		float lifetime;
		bool isEnemy;
	};
}
#endif