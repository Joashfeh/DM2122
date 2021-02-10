#pragma once
#include "Entities.h"

enum BLOCK_TYPE {
	UNBREAKABLE,
	BRICK,
	PIPE,
	QUESTION_BLOCK,
	STAR,
	FLOWER,
	SHROOM,
	COIN,
	POLE,
};

enum PIPE_TYPE {
	NIL,
	ENTRANCE,
	EXIT,
};

class Blocks : public Entities {
public:

	// ive gotten too far into not using polymorphism im gonna put all my variables into here sorry :/
	double	   starRotateAmount;
	double     timeCounter;
	Vector3    defaultPosition;
	BLOCK_TYPE blockType;
	PIPE_TYPE  pipe;

	Blocks();
	Blocks(BLOCK_TYPE type, 
		Vector3 pos, 
		float xSize, 
		float ySize, 
		float zSize);
	Blocks(BLOCK_TYPE type, 
		Vector3 pos, 
		float xSize, 
		float ySize, 
		float zSize,
		PIPE_TYPE pipe);
	~Blocks();

	bool Collision(Entities& entity);

};

