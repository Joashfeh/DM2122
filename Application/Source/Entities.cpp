#include "Entities.h"

Entities::Entities() {
	position.Set(0, 0, 0);

	xSize = 0.f;
	ySize = 0.f;
	zSize = 0.f;

	minX = position.x - xSize / 2;
	maxX = position.x + xSize / 2;

	minY = position.y - ySize / 2;
	maxY = position.y + ySize / 2;

	minZ = position.z - zSize / 2;
	maxZ = position.z + zSize / 2;
}

Entities::~Entities()
{
}
