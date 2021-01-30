#pragma once
#include "Entities.h"

enum BLOCK_TYPE {
	BARRIER, 
	BRICK,
	QUESTION_BRICK,
};

class Blocks : public Entities {
public:

	BLOCK_TYPE blockType;

	Blocks();
	Blocks(BLOCK_TYPE type, Vector3 pos, float xSize, float ySize, float zSize);
	~Blocks();

	bool Collision(Entities& entity);

};

