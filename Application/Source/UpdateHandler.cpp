#include "UpdateHandler.h"
#include "Assignment2.h"
#include "Application.h"

float jumpTime = 0.4f;
float jumpTimeCounter = 0.f;
bool stoppedJumping = true;

void UpdateHandler(float& bodyAngle, bool& jump, double dt) {
	MovementHandler(bodyAngle, dt);
	JumpHandler(jump, dt);
}

// Movement
void MovementHandler(float& bodyAngle, double dt) {
	if (Application::IsKeyPressed('A')) {
		bodyAngle = 270;
		Assignment2::player.position.x -= 0.3;
	}
	if (Application::IsKeyPressed('D')) {
		bodyAngle = 90;
		Assignment2::player.position.x += 0.3;
	}
}

// Jumping
void JumpHandler(bool& jump, double dt) {
	if (Assignment2::player.grounded) {
		jumpTimeCounter = jumpTime;
		jump = false;
	}

	if (Application::IsKeyPressed(VK_SPACE)) {
		if (Assignment2::player.grounded) {
			Assignment2::player.position.y += dt * 30;
			stoppedJumping = false;
			jump = true;
		}
	}

	if (Application::IsKeyPressed(VK_SPACE) && !stoppedJumping) {
		if (jumpTimeCounter > 0) {
			Assignment2::player.position.y += dt * 30;
			jumpTimeCounter -= dt;
		}
	}

	if (Application::IsKeyReleased(VK_SPACE)) {
		jumpTimeCounter = 0;
		stoppedJumping = true;
	}

	if (!Assignment2::player.grounded && jumpTimeCounter <= 0)
		Assignment2::player.position.y -= dt * 30;
}