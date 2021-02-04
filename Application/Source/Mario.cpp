#include "Mario.h"
#include "Blocks.h"
#include "Goomba.h"
#include "Assignment2.h"
#include "UpdateHandler.h"

Mario::Mario() {
    Init();
}

Mario::~Mario() {
}

void Mario::Init() {
    superStar = false;
    dead = false;
    kill = false;

    this->velocity.Set(0, 0, 0);
    this->position.Set(-15, 10, 0);
    this->velocityGoal = 0;

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
            if (displacementX < 0) {
                this->position.x += (xSize / 2 + entity.xSize / 2) + displacementX;
            }

            if (displacementX > 0) {
                this->position.x -= (xSize / 2 + entity.xSize / 2) - displacementX;
            }
                
            if (entity.type == GOOMBA) {
                if (superStar)
                    delete& entity;
                else
                    dead = true;
            }

        }
        else {

            if (displacementY < 0) {
                this->position.y += (ySize / 2 + entity.ySize / 2) + displacementY;
                grounded = true;
                kill = false;

                if (entity.type == GOOMBA) {
                    kill = true;
                    delete& entity;
                }

            }

            if (displacementY > 0 && velocity.y > 0) {

                this->position.y -= (ySize / 2 + entity.ySize / 2) - displacementY;

                if (((Blocks*)&entity)->blockType == BRICK && velocity.y > 0)
                    delete& entity;

                if (((Blocks*)&entity)->blockType == QUESTION_BLOCK && velocity.y > 0) {
                    ((Blocks*)&entity)->blockType = UNBREAKABLE;
                    Assignment2::World.push_back(new Blocks(STAR, Vector3(((Blocks*)&entity)->position.x, ((Blocks*)&entity)->position.y, 1), 1, 1, 1));
                }
                jumpTimeCounter = 0;
                this->velocity.y = -0.3;
            }
        }

        if (((Blocks*)&entity)->blockType == STAR) {
            superStar = true;
            delete& entity;
        }
    }
    else
        return false;
}
