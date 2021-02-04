#pragma once
#include "Vertex.h"
#include "Camera.h"

extern float jumpTime;
extern float jumpTimeCounter;

void UpdateHandler(float& bodyAngle, bool& jump, double dt);
void MovementHandler(float& bodyAngle, double dt);
void JumpHandler(bool& jump, double dt);
float Interpolate(float GoalVelocity, float CurrentVelocity, double dt);