#pragma once
#include "Entities.h"

enum BLOCK_TYPE {
	BARRIER, 
	UNBREAKABLE,
	BRICK,
	QUESTION_BLOCK,
	STAR,
};

class Blocks : public Entities {
public:

	// ive gotten too far into not using polymorphism im gonna put all my variables into here sorry :/

	double	   starRotateAmount;
	double     timeCounter;
	Vector3    defaultPosition;
	BLOCK_TYPE blockType;

	Blocks();
	Blocks(BLOCK_TYPE type, 
		Vector3 pos, 
		float xSize, 
		float ySize, 
		float zSize);
	~Blocks();

	bool Collision(Entities& entity);

};

