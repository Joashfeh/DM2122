#pragma once
#include "Entities.h"
class Mario : public Entities {
public:
	Vector3 velocity;
	float velocityGoal;

	bool grounded;
	bool leftWallContact;
	bool rightWallContact;

	Mario();
	~Mario();

	void Init();
	bool Collision(Entities& entity);

};

