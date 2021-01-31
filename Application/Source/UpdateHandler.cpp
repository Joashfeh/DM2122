#include "UpdateHandler.h"
#include "Assignment2.h"
#include "Application.h"

float jumpTime = 0.2f;
float jumpTimeCounter = 0.f;
bool stoppedJumping = true;

void UpdateHandler(float& bodyAngle, bool& jump, double dt) {
	Assignment2::player.position += Assignment2::player.velocity;
	MovementHandler(bodyAngle, dt);
	JumpHandler(jump, dt);

	Assignment2::player.grounded = false;
}

// Movement
void MovementHandler(float& bodyAngle, double dt) {

	// Interpolation
	if (Assignment2::player.grounded)
		Assignment2::player.velocity.x = Interpolate(Assignment2::player.velocityGoal, Assignment2::player.velocity.x, dt * 2.5);

	else if (!Assignment2::player.grounded)
		Assignment2::player.velocity.x = Interpolate(Assignment2::player.velocityGoal, Assignment2::player.velocity.x, dt * 0.25);

	if (Application::IsKeyReleased('A')) {
		Assignment2::player.velocityGoal = 0;
	}

	if (Application::IsKeyReleased('D')) {
		Assignment2::player.velocityGoal = 0;
	}

	if (Application::IsKeyPressed('A')) {
		bodyAngle = 270;
		Assignment2::player.velocityGoal = -0.4;
	}

	if (Application::IsKeyPressed('D')) {
		bodyAngle = 90;
		Assignment2::player.velocityGoal = 0.4;
	}
}

// Jumping
void JumpHandler(bool& jump, double dt) {

	float highJumpMultiplier = 3.f;
	float lowJumpMultiplier = 1.f;

	if (Assignment2::player.grounded) {
		Assignment2::player.velocity.y = 0;
		jumpTimeCounter = jumpTime;
		jump = false;

		if (Application::IsKeyPressed(VK_SPACE)) {
			Assignment2::player.velocity.y = 9.81 * dt * lowJumpMultiplier;
			stoppedJumping = false;
			jump = true;

		}
	}

	if (Assignment2::player.kill) {
		jump = true;
		stoppedJumping = true;

		if (jumpTimeCounter > 0) {
			Assignment2::player.velocity.y += 9.81 * dt * 1.5f;
			jumpTimeCounter -= dt;
		}
	}

	if (!Assignment2::player.grounded) {
		if (Assignment2::player.velocity.y > -1)
			Assignment2::player.velocity.y -= dt * 1.4;

		if (Application::IsKeyPressed(VK_SPACE) && !stoppedJumping) {
			if (jumpTimeCounter > 0) {
				Assignment2::player.velocity.y = 9.81 * dt * highJumpMultiplier;
				jumpTimeCounter -= dt;
			}
		}

		if (Application::IsKeyReleased(VK_SPACE)) {
			jumpTimeCounter = 0;
			stoppedJumping = true;
		}
	}
}

float Interpolate(float GoalVelocity, float CurrentVelocity, double dt) {
	float flDifference = GoalVelocity - CurrentVelocity;

	if (flDifference > dt)
		return CurrentVelocity + dt;

	if (flDifference < -dt)
		return CurrentVelocity - dt;

	return GoalVelocity;
}