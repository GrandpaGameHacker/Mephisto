#include "Utils/Camera/ICamera.h"
#include <spdlog/spdlog.h>
#include <gl/glew.h>
#include <glm/gtx/quaternion.hpp>
ICamera::ICamera()
{
	EMode = CameraMode::Normal;
	EType = CameraType::Perspective;

	Viewport = { 0,0,0,0 };
	AspectRatio = 0.0f;
	Clipping.x = 0.01f; Clipping.y = 1000.0f;
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	FOV = 45.0f;

	Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	PositionDelta = glm::vec3(0.0f, 0.0f, 0.0f);

	EulerRotation = glm::vec3(0,0,0);

	LookAt = glm::vec3(0, 0, 0);
	Direction = glm::vec3(0, 0, 0);
	Mouse = glm::vec3(0, 0, 0);
	UpdateDrawMode();

	spdlog::info("Constructed ICamera at pos {}, {}, {}", Position.x, Position.y, Position.z);
}
void ICamera::Update()
{
	Direction = glm::normalize(LookAt - Position);
	glm::vec3 PAxis = glm::cross(Direction, Up);
	glm::quat QPitch = glm::angleAxis(EulerRotation.x, PAxis);
	glm::quat QYaw = glm::angleAxis(EulerRotation.y, Up);
	glm::quat QRoll = glm::angleAxis(EulerRotation.z, Direction);
	glm::quat QTemp = glm::cross(QPitch, QYaw);
	QRoll = glm::normalize(QRoll);
	QTemp = glm::normalize(QTemp);
	Direction = glm::rotate(QTemp, Direction);
	Up = glm::rotate(QRoll, Up);
	Position += PositionDelta;
	LookAt = Position + Direction * 1.0f;
}

// no roll calculation
void ICamera::UpdateFirstPerson()
{
	EulerRotation.z = 0;
	Update();
}



void ICamera::Update(glm::vec3 pos, glm::vec3 rot, glm::vec3 lookat)
{
	Position = pos;
	EulerRotation = rot;
	LookAt = lookat;
	Update();
}

void ICamera::Reset()
{

}

void ICamera::UpdateDrawMode()
{
	switch (EMode)
	{
	case CameraMode::Vertex:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case CameraMode::Wireframe:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case CameraMode::Normal:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

void ICamera::ChangePitch(float degrees)
{
	EulerRotation.x += degrees;

	if (EulerRotation.x > 360.0f) {
		EulerRotation.x -= 360.0f;
	}
	else if (EulerRotation.x < -360.0f) {
		EulerRotation.x += 360.0f;
	}
}

void ICamera::ChangeYaw(float degrees)
{

	if (EulerRotation.x > 90 && EulerRotation.x < 270 || (EulerRotation.x < -90 && EulerRotation.x > -270)) {
		EulerRotation.y -= degrees;
	}
	else {
		EulerRotation.y += degrees;
	}

	if (EulerRotation.y > 360.0f) {
		EulerRotation.y -= 360.0f;
	}
	else if (EulerRotation.y < -360.0f) {
		EulerRotation.y += 360.0f;
	}
}

void ICamera::ChangeRoll(float degrees)
{
	EulerRotation.z += degrees;
}

void ICamera::MouseInput(int x, int y)
{
	glm::vec3 MouseDelta = Mouse - glm::vec3(x, y, 0);
	Mouse = glm::vec3(x, y, 0);
}

void ICamera::SetType(CameraType type)
{
	EType = type;
}

void ICamera::SetMode(CameraMode mode)
{
	EMode = mode;
	Up = glm::vec3{ 0,1,0 };
	Rotation = glm::quat(1, 0, 0, 0);
}

void ICamera::SetPosition(glm::vec3 pos)
{
	Position = pos;
}

void ICamera::SetLookAtTarget(glm::vec3 pos)
{
		LookAt = pos;
}

void ICamera::SetFOV(double fov)
{
	FOV = fov;
}

void ICamera::GetViewport(SDL_Rect& viewport)
{
	viewport = Viewport;
}

void ICamera::SetViewport(SDL_Rect& viewport)
{
	Viewport = viewport;
	AspectRatio = static_cast<double>(viewport.w) / static_cast<double>(viewport.h);
}

CameraType ICamera::GetType()
{
	return CameraType();
}

CameraMode ICamera::GetMode()
{
	return CameraMode();
}

glm::mat4 ICamera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Direction, Up);
}

glm::mat4 ICamera::GetProjectionMatrix()
{
	if (EType == CameraType::Orthographic)
	{
		return glm::ortho(-1.5f * (float)AspectRatio, -1.5f * (float)AspectRatio, -1.5f, -1.5f, -10.f, -10.f);
	}
	else if (EType == CameraType::Perspective)
	{
		return glm::perspective(FOV, AspectRatio, Clipping.x, Clipping.y);
	}
	return glm::mat4(1);
}
