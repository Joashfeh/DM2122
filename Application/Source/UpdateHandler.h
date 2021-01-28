#pragma once
#include "Vertex.h"
#include "Camera.h"

void UpdateHandler(float& bodyAngle, bool& jump, double dt);
void MovementHandler(float& bodyAngle, double dt);
void JumpHandler(bool& jump, double dt);