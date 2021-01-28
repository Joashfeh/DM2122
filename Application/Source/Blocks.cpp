#include "Blocks.h"

Blocks::Blocks() {
}

Blocks::~Blocks() {
}

Blocks::Blocks(BLOCK_TYPE type, Position pos, float xSize, float ySize, float zSize) {
	blockType = type;
	position = pos;

	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;

	minX = position.x - xSize / 2;
	maxX = position.x + xSize / 2;

	minY = position.y - ySize / 2;
	maxY = position.y + ySize / 2;

	minZ = position.z - zSize / 2;
	maxZ = position.z + zSize / 2;
}

bool Blocks::Collision(Entities& entity) {
    return false;
}
