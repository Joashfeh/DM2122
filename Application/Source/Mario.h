#pragma once
#include "Entities.h"
class Mario : public Entities {
public:
	Vector3 velocity;
	float   velocityGoal; // for interpolation
	float   bodySize; // for model size

	int     Score;
	bool    Win;
	bool    hitTimerActive; // after hit with mushroom
	bool    pipeAnimationActive;
	
	bool    canMove; // for win and pipe
	bool    scaled; // mushroom
	bool    shoot; // flower
	bool    flower; // flower, cant remember why i have shoot honestly
	bool    superStar; // star.
	bool    dead; // die
	bool    kill; // for kill bounce
	bool    grounded; // help

	Mario();
	~Mario();

	void Init();
	bool AABB(Entities& a, Entities& b);
	bool Collision(Entities& entity);

};

