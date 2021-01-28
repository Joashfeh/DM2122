#pragma once
#include "Entities.h"

enum BLOCK_TYPE {
	BRICK,
	QUESTION_BRICK,
};

class Blocks : public Entities {
public:

	BLOCK_TYPE blockType;

	Blocks();
	Blocks(BLOCK_TYPE type, Position pos, float xSize, float ySize, float zSize);
	~Blocks();

	bool Collision(Entities& entity);

};

