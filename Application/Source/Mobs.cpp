#include "Mobs.h"

Mobs::Mobs() {
}

Mobs::~Mobs() {
}

void Mobs::Init() {
    this->position.Set(0, 0, 0);
    xSize = 1.f;
    ySize = 1.f;
    zSize = 0.f;

    minX = position.x - xSize / 2;
    maxX = position.x - xSize / 2;

    minY = position.y - ySize / 2;
    maxY = position.y - ySize / 2;

    minZ = position.z - zSize / 2;
    maxZ = position.z - zSize / 2;
}

bool Mobs::Collision(Entities& entity) {
        return false;
}
