#pragma once
#include "Entities.h"
class Mario : public Entities {
public:
	bool grounded;
	bool leftWallContact;
	bool rightWallContact;

	Mario();
	~Mario();

	void Init();
	bool Collision(Entities& entity);

};

