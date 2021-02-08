#pragma once
#include "Blocks.h"

enum ITEM_TYPE {
	SUPERSTAR,
	FIREFLOWER,
	MUSHROOM,
	GOLDCOIN,
};

class QuestionBlock : public Blocks {
public:


	ITEM_TYPE  itemType;
	Vector3    defaultPosition;
	bool       hit;
	

	QuestionBlock();
	QuestionBlock(ITEM_TYPE type,
		Vector3 pos, 
		float xSize, 
		float ySize, 
		float zSize);
	~QuestionBlock();

	bool Collision(Entities& entity);

};

