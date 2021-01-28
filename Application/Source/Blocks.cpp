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

	minX = position.x - xSize;
	maxX = position.x + xSize;

	minY = position.y - ySize;
	maxY = position.y + ySize;

	minZ = position.z - zSize;
	maxZ = position.z + zSize;
}

bool Blocks::Collision(Entities& entity) {
    return false;
}
