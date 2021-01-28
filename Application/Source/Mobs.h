#pragma once
#include "Entities.h"
class Mobs : public Entities {
public:
	Mobs();
	~Mobs();

	void Init();
	bool Collision(Entities& entity);

};

