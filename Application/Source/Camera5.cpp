#include "Camera5.h"
#include "Application.h"
#include "Mtx44.h"
#include "Vertex.h"
#include "Assignment2.h"

Camera5::Camera5()
{
}

Camera5::~Camera5()
{
}

void Camera5::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->type = SIDESCROLL;
	this->position = defaultPosition = Vector3(0, 8, 35);
	this->PositionVector = position - Vector3(Assignment2::player.position.x, Assignment2::player.position.y, Assignment2::player.position.z);
	this->target = defaultTarget = Vector3(0, 8, 0);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera5::Update(double dt)
{
	static const float CAMERA_SPEED = 100.f;
	static const float ZOOM_SPEED = 20.f;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	this->up = defaultUp = right.Cross(view).Normalized();
	switch (type) {
	case SIDESCROLL:
		if (Assignment2::player.position.x > target.x) {
			target.x = Assignment2::player.position.x;
			position.x = Assignment2::player.position.x;
		}
		break;
	case FIRSTPERSON:
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		static const float CAMERA_SPEED = 100.f;
		if (Application::IsKeyPressed('A'))
		{
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);

			position = position - (right * float(CAMERA_SPEED * dt));
			target = position + view;
		}
		if (Application::IsKeyPressed('D'))
		{
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);;

			position = position + (right * float(CAMERA_SPEED * dt));
			target = position + view;
		}
		if (Application::IsKeyPressed('W'))
		{
			Vector3 view = (target - position).Normalized();
			Vector3 temp = view;
			position = position + (view * float(CAMERA_SPEED * dt));
			target = position + view;
		}
		if (Application::IsKeyPressed('S'))
		{
			Vector3 view = (target - position).Normalized();
			Vector3 temp = view;
			position = position - (view * float(CAMERA_SPEED * dt));
			target = position + view;
		}

		if (Application::IsKeyPressed(VK_SPACE))
		{
			position.y += dt * 50.f;
			target = position + view;
		}

		if (Application::IsKeyPressed(VK_CONTROL))
		{
			position.y -= dt * 50.f;
			target = position + view;
		}

		if (Application::IsKeyPressed(VK_LEFT))
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(CAMERA_SPEED * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			target = position + view;
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(-CAMERA_SPEED * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			target = position + view;
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
		}
		if (Application::IsKeyPressed(VK_UP))
		{
			float pitch = (float)(CAMERA_SPEED * (float)dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
		if (Application::IsKeyPressed(VK_DOWN))
		{
			float pitch = (float)(-CAMERA_SPEED * (float)dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
		//Update the camera direction based on mouse move
		// left-right rotate
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			target = position + view;
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
		}
		{
			float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
			if (pitch > 89.f)
				pitch = 89.f;
			if (pitch < -89.f)
				pitch = -89.f;

			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
		break;
	}

	if (Application::IsKeyPressed(VK_F9)) {
		Reset();
		type = SIDESCROLL;
	}

	if (Application::IsKeyPressed(VK_F10)) {
		Reset();
		type = FIRSTPERSON;
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}

}

void Camera5::Reset()
{
	up = defaultUp;
	position = defaultPosition;
	target = defaultTarget;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}