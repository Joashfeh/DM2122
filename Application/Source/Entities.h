#pragma once
#include "Vertex.h"
#include <Vector3.h>

enum ENTITY_TYPE {
	GOOMBA,
	BLOCK,
};

class Entities {
public:
	ENTITY_TYPE type;
	Vector3 position;
	float   xSize;
	float   ySize;
	float   zSize;

	float	minX, maxX;
	float	minY, maxY;
	float	minZ, maxZ;

	Entities();
	~Entities();

	virtual bool Collision(Entities& entity) = 0;
};

