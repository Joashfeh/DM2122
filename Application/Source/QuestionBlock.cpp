#include "QuestionBlock.h"

QuestionBlock::QuestionBlock() {
}

QuestionBlock::~QuestionBlock() {
}

QuestionBlock::QuestionBlock(ITEM_TYPE type, Vector3 pos, float xSize, float ySize, float zSize) {
	starRotateAmount = 0.f;
	timeCounter = 0.f;
	this->itemType = type;
	this->type = BLOCK;
	this->blockType = QUESTION_BLOCK;
	hit = false;
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

bool QuestionBlock::Collision(Entities& entity) {
    return false;
}
