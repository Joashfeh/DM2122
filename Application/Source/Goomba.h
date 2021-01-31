#pragma once
#include "Entities.h"

class Goomba : public Entities {
public:
	bool Dead;

	Goomba();
	Goomba(Vector3 pos);
	~Goomba();

	bool Collision(Entities& entity);

};

