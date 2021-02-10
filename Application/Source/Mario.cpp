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

    Score = 0;

    Win = false;
    pipeAnimationActive = false;
    hitTimerActive = false;
    canMove = true;
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

        if (((Blocks*)&entity)->blockType == STAR) {
            Score += 1000;
            superStar = true;
            delete& entity;
            return true;
        }

        if (((Blocks*)&entity)->blockType == FLOWER) {
            Score += 1000;
            flower = true;
            delete& entity;
            return true;
        }

        if (((Blocks*)&entity)->blockType == SHROOM) {
            Assignment2::frames = 0;
            scaled = true;
            delete& entity;
            return true;
        }

        if (((Blocks*)&entity)->blockType == COIN) {
            Score += 200;
            delete& entity;
            return true;
        }

        if (((Blocks*)&entity)->blockType == POLE) {
            Score += floor(position.y * 1000);
            canMove = false;
            Win = true;
            return false;
        }

        float displacementX = entity.position.x - this->position.x;
        float displacementY = entity.position.y - this->position.y;

        // help
        if (abs(displacementX) * 1.f / entity.xSize > abs(displacementY) * 1.f / entity.ySize) {

            if (entity.type == GOOMBA) {
                if (!hitTimerActive) {
                    if (superStar) {
                        delete& entity;
                        return true;
                    }
                    else if (scaled) {
                        scaled = false;
                        bodySize = 0.5;
                        hitTimerActive = true;
                        velocity.x = 0;
                        return false;
                    }

                    else
                        dead = true;
                }
                else {
                    return false;
                }
            }

            if (displacementX < 0) {
                this->position.x += (xSize / 2 + entity.xSize / 2) + displacementX;
            }

            if (displacementX > 0) {
                this->position.x -= (xSize / 2 + entity.xSize / 2) - displacementX;
            }
              

        }
        // me
        else {
            if (displacementY < 0) {
                if (((Blocks*)&entity)->blockType == PIPE) {
                    if (((Blocks*)&entity)->pipe == ENTRANCE) {
                        if (!pipeAnimationActive && abs(position.x - entity.position.x) < 0.5) {
                            if (Application::IsKeyPressed('S')) {
                                pipeAnimationActive = true;
                                canMove = false;
                            }
                        }
                    }
                }

                if (entity.type == GOOMBA) {

                    if (hitTimerActive)
                        return false;

                    this->Score += 100;
                    this->grounded = true;
                    this->kill = true;
                    delete& entity;
                    return true;
                }

                this->position.y += (ySize / 2 + entity.ySize / 2) + displacementY;
                grounded = true;
                kill = false;
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

                        if (((QuestionBlock*)&entity)->itemType == GOLDCOIN)
                            Assignment2::World.push_back(new Blocks(COIN, Vector3(((Blocks*)&entity)->position.x, ((Blocks*)&entity)->position.y, 0), 1, 1, 1));
                        
                        ((QuestionBlock*)&entity)->hit = true;
                        return false;
                    }
                }

                if (((Blocks*)&entity)->blockType == BRICK && scaled) {
                    delete& entity;
                    return true;
                }
            }
        }
    }
    return false;
}
