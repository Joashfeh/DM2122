#include "UpdateHandler.h"
#include "Assignment2.h"
#include "Application.h"
#include "Mario.h"
#include "Fireball.h"

float jumpTime = 0.2f;
float jumpTimeCounter = 0.f;
float shootTimer = .5f;
bool stoppedJumping = true;
bool shootFireball = false;

void UpdateHandler(float& bodyAngle, bool& jump, double dt) {

	if (Application::IsMousePressed(0) && !Assignment2::player.shoot) {
		Assignment2::player.shoot = true;
		shootFireball = false;
	}

	if (Assignment2::player.shoot) {
		shootTimer -= dt;
	}

	if (shootTimer < 0) {
		Assignment2::player.shoot = false;
		shootTimer = .5f;
	}
		
	 
	if (!Application::IsMousePressed(0) && !shootFireball && Assignment2::player.flower) {
		Assignment2::World.push_back(new Fireball(bodyAngle));
		shootFireball = true;
	}

	Assignment2::player.position += Assignment2::player.velocity;

	JumpHandler(jump, dt);
	MovementHandler(bodyAngle, dt);

	Assignment2::player.grounded = false;
	if (Assignment2::player.position.y < -20)
		Assignment2::player.dead = true;		

	if (Assignment2::player.kill) {
		Assignment2::player.grounded = false;
		jump = true;
		stoppedJumping = false;

		if (jumpTimeCounter > 0) {
			Assignment2::player.velocity.y += 9.81 * dt * 1.f;
			jumpTimeCounter -= dt;
		}
	}
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
		Assignment2::player.velocityGoal = -0.21;
		if (Application::IsKeyPressed(VK_LSHIFT)) {
			Assignment2::player.velocityGoal = -0.32;
		}
	}

	if (Application::IsKeyPressed('D')) {
		bodyAngle = 90;
		Assignment2::player.velocityGoal = 0.21;
		if (Application::IsKeyPressed(VK_LSHIFT)) {
			Assignment2::player.velocityGoal = 0.32;
		}
	}
}

// Jumping
void JumpHandler(bool& jump, double dt) {

	float highJumpMultiplier = 2.5f;
	float lowJumpMultiplier = 0.5f;

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