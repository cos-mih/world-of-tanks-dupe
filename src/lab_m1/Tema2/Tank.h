#include "core/gpu/mesh.h"
#include "components/simple_scene.h"
#include "Projectile.h"

namespace m1 {
	class Tank
	{
	public:
		Tank();
		Tank(glm::vec3 pos, glm::vec3 dir, glm::vec3 tCColor, glm::vec3 bColor, glm::vec3 tCbolor, float fireInt);
		~Tank();

		enum class MovementState
		{
			Forward,
			Backward,
			RotateLeft,
			RotateRight
		};

		glm::vec3 getTracksCannonColor();
		glm::vec3 getBodyColor();
		glm::vec3 getTurretColor();

		float getSpeed();
		float getAngularSpeed();
		int getHP();

		float getTimeElapsed();
		void incrementTimeElapsed(float deltaTime);
		void resetTimeElapsed();
		bool canFire();

		glm::vec3 getPosition();
		glm::vec3 getTankDir();
		glm::vec3 getTurretDir();
		glm::vec3 getCannonDIr();

		float getAngle();
		float getTurretAngle();
		float getCannonAngle();

		void TranslateTank(float distance);
		void TranslateTankDir(glm::vec3 dist);
		void RotateTank(float deltaAngle);
		void RotateTurret(float deltaAngle);

		glm::vec3 getCannonTipPos();
		bool isHit(Projectile projectile);
		void decrementHP();
		bool isActive();

		float collidesWithTank(Tank tank);

		void decrementStateInterval(float deltaTime);
		bool stateActive();
		void generateNewState();
		void applyMovement(float deltaTime);

		void aimTurret(glm::vec3 target);

	protected:
		glm::vec3 pos, dir, turretDir, cannonDir;
		float angle, turretAngle, cannonAngle, speed = 7, angularSpeed = 3, enemySpeed = 5;
		glm::vec3 trackCannonColor, bodyColor, turretColor;
		float timeElapsedSinceFiring, fireInterval;
		int hp;
		MovementState state;
		float stateInterval;
	};
}