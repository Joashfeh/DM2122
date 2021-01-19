#include "UpdateHandler.h"
#include "Assignment2.h"
#include "Application.h"

void MovementHandler(float bodyAngle, double dt) {
	if (Application::IsKeyPressed('W')) {
		Mtx44 rotation;
		rotation.SetToRotation(bodyAngle, 0, 1, 0);
		Assignment2::MarioPos += rotation * Vector3(0, 0, 1);
	}
	if (Application::IsKeyPressed('A')) {
		Mtx44 rotation;
		rotation.SetToRotation(bodyAngle, 0, 1, 0);
		Assignment2::MarioPos += rotation * Vector3(1, 0, 0);
	}
	if (Application::IsKeyPressed('S')) {
		Mtx44 rotation;
		rotation.SetToRotation(bodyAngle, 0, 1, 0);
		Assignment2::MarioPos -= rotation * Vector3(0, 0, 1);
	}
	if (Application::IsKeyPressed('D')) {
		Mtx44 rotation;
		rotation.SetToRotation(bodyAngle, 0, 1, 0);
		Assignment2::MarioPos -= rotation * Vector3(1, 0, 0);
	}
}
