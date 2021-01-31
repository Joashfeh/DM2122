#include "Goomba.h"

Goomba::Goomba() {
	this->type = GOOMBA;
	Dead = false;
}

Goomba::Goomba(Vector3 pos) {
	this->type = GOOMBA;
	this->position = pos;
	Dead = false;

	this->xSize = 1.8;
	this->ySize = 1.8;
	this->zSize = 1.8;

}

Goomba::~Goomba() {
}

bool Goomba::Collision(Entities& entity)
{
	return false;
}
