#pragma once
#include "Entities.h"
class Mario : public Entities {
public:
	Vector3 velocity;
	float   velocityGoal;
	float   bodySize;

	int     Score;
	bool    Win;
	bool    hitTimerActive;
	bool    pipeAnimationActive;
	
	bool    canMove;
	bool    scaled;
	bool    shoot;
	bool    flower;
	bool    superStar;
	bool    dead;
	bool    kill;
	bool    grounded;

	Mario();
	~Mario();

	void Init();
	bool AABB(Entities& a, Entities& b);
	bool Collision(Entities& entity);

};

