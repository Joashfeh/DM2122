#pragma once
#include "Entities.h"

class Goomba : public Entities {
public:
	Vector3 defaultPosition;
	bool    Dead;
	float   bodyAngle;

	Goomba();
	Goomba(Vector3 pos);
	~Goomba();

	bool Collision(Entities& entity);

};

