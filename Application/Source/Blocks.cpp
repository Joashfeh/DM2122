#include "Blocks.h"

Blocks::Blocks() {
}

Blocks::~Blocks() {
}

Blocks::Blocks(BLOCK_TYPE type, Vector3 pos, float xSize, float ySize, float zSize, bool pipe) {
	this->pipe = pipe;
	starRotateAmount = 0.f;
	timeCounter = 0.f;
	this->type = BLOCK;
	blockType = type;
	position = defaultPosition = pos;
	defaultPosition.y += 2.3f;

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

Blocks::Blocks(BLOCK_TYPE type, Vector3 pos, float xSize, float ySize, float zSize) {
	pipe = false;
	starRotateAmount = 0.f;
	timeCounter = 0.f;
	this->type = BLOCK;
	blockType = type;
	position = defaultPosition = pos;
	defaultPosition.y += 2.3f;

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
