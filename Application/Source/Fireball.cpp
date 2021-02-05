#include "Fireball.h"
#include "Assignment2.h"

Fireball::Fireball() {
    Init();
}

Fireball::Fireball(float angle) {
    Init();

    if (angle == 90) {
        position = Assignment2::player.position;
        position.x += 1.f;
        position.y += 0.25f;
        direction = 0;
    }
    else {
        position = Assignment2::player.position;
        position.x -= 1.f;
        position.y -= 0.25f;
        direction = 1;
    }

}

Fireball::~Fireball() {
}

void Fireball::Init() {
    velocity.Set(0.6, 0, 0);
    this->type = FIREBALL;

    xSize = 0.5f;
    ySize = 0.5f;
    zSize = 0.5f;

    minX = position.x - xSize;
    maxX = position.x + xSize;

    minY = position.y - ySize;
    maxY = position.y + ySize;

    minZ = position.z - zSize;
    maxZ = position.z + zSize;
}

void Fireball::Update(float angle) {
    switch (direction) {
    case 0:
        position += velocity;
        break;
    case 1:
        position -= velocity;
        break;
    }
}

bool Fireball::AABB(Entities& a, Entities& b) {
    return (a.xSize / 2 + b.xSize / 2) > abs(b.position.x - a.position.x) &&
        (a.ySize / 2 + b.ySize / 2) > abs(b.position.y - a.position.y);
}

bool Fireball::Collision(Entities& entity) {
        if (AABB(*this, entity)) {
            if (entity.type == GOOMBA) {
                delete& entity;
            }
            delete this;
        }
    return false;
}
