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
	this->position = defaultPosition = Vector3(0, 6, 35);
	this->PositionVector = position - Vector3(Assignment2::player.position.x, Assignment2::player.position.y, Assignment2::player.position.z);
	this->target = defaultTarget = Vector3(0, 6, 0);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera5::Update(double dt)
{
	static const float CAMERA_SPEED = 45.f;
	static const float ZOOM_SPEED = 20.f;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	this->up = defaultUp = right.Cross(view).Normalized();

	if (Assignment2::player.position.x > target.x) {
		target.x = Assignment2::player.position.x;
		position.x = Assignment2::player.position.x;
	}

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

void Camera5::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}