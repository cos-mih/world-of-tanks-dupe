#ifndef BUILDING_H
#define BUILDING_H

#include "core/gpu/mesh.h"
#include "components/simple_scene.h"

#include "Projectile.h"

namespace m1 {
	class Building
	{
	public:
		Building(Mesh* mesh, glm::vec3 pos, int angle, float length, float width, glm::vec3 color);
		~Building();

		Mesh* getMesh();
		glm::vec3 getPosition();
		glm::vec3 getColor();
		int getAngle();
		bool isHit(glm::vec3 position, float radius);

	protected:
		Mesh* mesh;
		glm::vec3 pos, color;
		float length, width;
		int angle;
	};
}

#endif