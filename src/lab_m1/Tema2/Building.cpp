#include "Building.h"

#include "core/gpu/mesh.h"
#include "components/simple_scene.h"

using namespace m1;
using namespace std;

Building::Building(Mesh* mesh, glm::vec3 pos, int angle, float length, float width, glm::vec3 color)
{
	this->mesh = mesh;
	this->pos = pos;
	this->angle = angle;
	this->length = length;
	this->width = width;
	this->color = color;
}

Building::~Building()
{
}

Mesh* Building::getMesh()
{
	return mesh;
}

glm::vec3 Building::getPosition()
{
	return pos;
}

glm::vec3 Building::getColor()
{
	return color;
}

int Building::getAngle()
{
	return angle;
}

bool Building::isHit(glm::vec3 position, float radius)
{
	switch (angle) {
		case 0:
		case 180:
			return abs(pos.x - position.x) <= (length / 2 - radius) && abs(pos.z - position.z) <= (width / 2 - radius);
			break;
		case 90:
		case 270:
			return abs(pos.z - position.z) <= (length / 2 - radius) && abs(pos.x - position.x) <= (width / 2 - radius);
	}

	return false;
}
