#include "Mario.h"

Mario::Mario() {
    Init();
}

Mario::~Mario() {
}

void Mario::Init() {
    this->position.Set(0, 0, 0);
    xSize = 2.4f;
    ySize = 5.2f;
    zSize = 1.f;

    minX = position.x - xSize;
    maxX = position.x + xSize;

    minY = position.y;
    maxY = position.y + ySize;

    minZ = position.z - zSize;
    maxZ = position.z + zSize;
}

bool AABB(Entities& a, Entities& b) {
    return (a.position.x <= b.position.x + b.xSize - b.xSize/4) &&
        ((a.position.x + a.xSize - a.xSize/4) >= b.position.x) &&
        (a.position.y <= (b.position.y + b.ySize)) &&
        (a.position.y + a.ySize) >= b.position.y;
}

bool Mario::Collision(Entities& entity) {
    if (AABB(*this, entity)) {
        if (this->position.y >= entity.position.y)
        {
            grounded = true;
            return true;
        }

        if (this->position.y < entity.position.y && this->maxX <= entity.minX)
        {
            rightWallContact = true;
            return true;
        }

        if (this->position.y < entity.position.y && this->minX <= entity.maxX)
        {
            leftWallContact = true;
            return true;
        }
    }
    else
        return false;
}
