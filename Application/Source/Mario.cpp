#include "Mario.h"
#include "Blocks.h"
#include "Goomba.h"
#include "Assignment2.h"
#include "UpdateHandler.h"
#include "QuestionBlock.h"

Mario::Mario() {
    Init();
}

Mario::~Mario() {
}

void Mario::Init() {
    scaled = false;
    flower = false;
    superStar = false;
    dead = false;
    kill = false;
    shoot = false;
    bodySize = 0.5f;

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

bool Mario::AABB(Entities& a, Entities& b) {
    return (a.xSize / 2 + b.xSize / 2) > abs(b.position.x - a.position.x) &&
        (a.ySize / 2 + b.ySize / 2) > abs(b.position.y - a.position.y);
}

bool Mario::Collision(Entities& entity) {

    if (AABB(*this, entity)) {
     
        float displacementX = entity.position.x - this->position.x;
        float displacementY = entity.position.y - this->position.y;

        if (abs(displacementX) * 1.f / entity.xSize > abs(displacementY) * 1.f / entity.ySize) {
            if (displacementX < 0) {
                this->position.x += (xSize / 2 + entity.xSize / 2) + displacementX;
            }

            if (displacementX > 0) {
                this->position.x -= (xSize / 2 + entity.xSize / 2) - displacementX;
            }
                
            if (entity.type == GOOMBA) {
                if (superStar) {
                    delete& entity;
                    return true;
                }

                else
                    dead = true;
            }

        }
        else {

            if (((Blocks*)&entity)->blockType == PIPE) {
                std::cout << abs(displacementX) << std::endl;
                std::cout << abs(displacementY) << std::endl;
            }

            if (displacementY < 0) {

                if (entity.type != STAR) {
                    this->position.y += (ySize / 2 + entity.ySize / 2) + displacementY;
                    grounded = true;
                    kill = false;
                }

                if (entity.type == GOOMBA) {
                    kill = true;
                    delete& entity;
                    return true;
                }

            }

            if (displacementY > 0 && velocity.y > 0) {

                this->position.y -= (ySize / 2 + entity.ySize / 2) - displacementY;
                jumpTimeCounter = 0;
                this->velocity.y = -0.3;

                if (((Blocks*)&entity)->blockType == QUESTION_BLOCK) {
                    if (!((QuestionBlock*)&entity)->hit) {
                        if (((QuestionBlock*)&entity)->itemType == SUPERSTAR)
                            Assignment2::World.push_back(new Blocks(STAR, Vector3(((Blocks*)&entity)->position.x, ((Blocks*)&entity)->position.y, 0), 1, 1, 1));
                       
                        if (((QuestionBlock*)&entity)->itemType == FIREFLOWER)
                            Assignment2::World.push_back(new Blocks(FLOWER, Vector3(((Blocks*)&entity)->position.x, ((Blocks*)&entity)->position.y, 0), 1, 1, 1));

                        if (((QuestionBlock*)&entity)->itemType == MUSHROOM)
                            Assignment2::World.push_back(new Blocks(SHROOM, Vector3(((Blocks*)&entity)->position.x, ((Blocks*)&entity)->position.y, 0), 1, 1, 1));
                        
                        ((QuestionBlock*)&entity)->hit = true;
                        return false;
                    }
                }

                if (((Blocks*)&entity)->blockType == BRICK) {
                    delete& entity;
                    return true;
                }
            }
        }

        if (((Blocks*)&entity)->blockType == STAR) {
            superStar = true;
            delete& entity;
            return true;
        }

        if (((Blocks*)&entity)->blockType == FLOWER) {
            flower = true;  
            delete& entity;
            return true;
        }

        if (((Blocks*)&entity)->blockType == SHROOM) {
            scaled = true;
            delete& entity;
            return true;
        }
    }
    return false;
}
