#include "Camera4.h"
#include "Application.h"
#include "Mtx44.h"
#include "Vertex.h"
#include "Assignment2.h"

Camera4::Camera4()
{
}

Camera4::~Camera4()
{
}

void Camera4::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->PositionVector = position - Vector3(Assignment2::player.position.x, Assignment2::player.position.y, Assignment2::player.position.z);
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera4::Update(double dt)
{
	static const float CAMERA_SPEED = 45.f;
	static const float ZOOM_SPEED = 20.f;

		{
			pitch = -Application::camera_pitch * CAMERA_SPEED * static_cast<float>(dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position = rotation * position;
			PositionVector = rotation * PositionVector;
		}
		{
			yaw = -Application::camera_yaw * CAMERA_SPEED * static_cast<float>(dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position = rotation * position;
			PositionVector = rotation * PositionVector;
			up = rotation * up;
		}

		position = Vector3(Assignment2::player.position.x, Assignment2::player.position.y, Assignment2::player.position.z) + PositionVector;
		target.Set(Assignment2::player.position.x, Assignment2::player.position.y + 2.4, Assignment2::player.position.z);

	

	if(Application::IsKeyPressed('N'))
	{
		Vector3 view = target - position;
		if(view.Length() > 1) //Prevent the camera from touching the origin
		{
			view.Normalize();
			position += view * ZOOM_SPEED * dt;
		}
	}

	if(Application::IsKeyPressed('M'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * ZOOM_SPEED * dt;
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}

}

void Camera4::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}