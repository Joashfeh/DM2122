#include "Entities.h"

Entities::Entities() {
	position.Set(0, 0, 0);

	xSize = 0.f;
	ySize = 0.f;
	zSize = 0.f;

	minX = position.x - xSize;
	maxX = position.x + xSize;

	minY = position.y - ySize;
	maxY = position.y + ySize;

	minZ = position.z - zSize;
	maxZ = position.z + zSize;
}

Entities::~Entities()
{
}
