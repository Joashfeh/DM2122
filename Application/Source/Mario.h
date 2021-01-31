#pragma once
#include "Entities.h"
class Mario : public Entities {
public:
	Vector3 velocity;
	float velocityGoal;

	bool kill;
	bool grounded;

	Mario();
	~Mario();

	void Init();
	bool Collision(Entities& entity);

};

