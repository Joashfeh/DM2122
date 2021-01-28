#include "Mario.h"

Mario::Mario() {
    Init();
}

Mario::~Mario() {
}

void Mario::Init() {
    this->position.Set(0, 50, 0);
    xSize = 0.8f;
    ySize = 3.4f;
    zSize = 1.f;

    minX = position.x - xSize;
    maxX = position.x + xSize;

    minY = position.y;
    maxY = position.y + ySize;

    minZ = position.z - zSize;
    maxZ = position.z + zSize;
}

bool AABB(Entities& a, Entities& b) {
    return (a.xSize / 2 + b.xSize / 2) > abs(b.position.x - a.position.x) &&
        (a.ySize / 2 + b.ySize / 2) > abs(b.position.y - a.position.y);
}

bool Mario::Collision(Entities& entity) {
    if (AABB(*this, entity)) {

        float displacementX = entity.position.x - this->position.x;
        float displacementY = entity.position.y - this->position.y;

        if (abs(displacementX) > abs(displacementY)) {
            if (displacementX < 0)
                this->position.x += (xSize / 2 + entity.xSize / 2) + displacementX;

            if (displacementX > 0)
                this->position.x -= (xSize / 2 + entity.xSize / 2) - displacementX;
        }
        else {
            if (displacementY < 0) {
                this->position.y += (ySize / 2 + entity.ySize / 2) + displacementY;
                grounded = true;
            }

            if (displacementY > 0)
                this->position.y -= (ySize / 2 + entity.ySize / 2) - displacementY;
        }
    }
    else
        return false;
}
