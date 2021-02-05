#pragma once
#include "Entities.h"
class Fireball : public Entities {
public:
	Vector3 velocity;
	int direction; // 0 - left, 1 - right

	Fireball();
	Fireball(float angle);
	~Fireball();

	void Init();
	void Update(float angle);
	bool AABB(Entities& a, Entities& b);
	bool Collision(Entities& entity);

};

