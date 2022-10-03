#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraType
{
	Orthographic,
	Perspective,
};

enum class CameraMode
{
	Vertex,
	Wireframe,
	Normal,
};


class ICamera
{
public:
	ICamera();
	virtual ~ICamera() {};
	virtual void UpdateFirstPerson();
	virtual void Update();
	virtual void Update(glm::vec3 pos, glm::vec3 rot, glm::vec3 lookat);
	virtual void Reset();
	virtual void ChangePitch(float degrees);
	virtual void ChangeYaw(float degrees);
	virtual void ChangeRoll(float degrees);
	virtual void MouseInput(int x, int y);

	void UpdateDrawMode();
	void SetType(CameraType type);
	void SetMode(CameraMode mode);

	void SetPosition(glm::vec3 pos);
	void SetLookAtTarget(glm::vec3 pos);
	void SetFOV(double fov);

	void GetViewport(SDL_Rect& viewport);
	void SetViewport(SDL_Rect& viewport);

	CameraType GetType();
	CameraMode GetMode();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

protected:
	CameraType EType;
	CameraMode EMode;

	SDL_Rect Viewport;

	double AspectRatio;
	double FOV;
	glm::tvec2<double> Clipping;
	glm::vec3 EulerRotation;

	glm::vec3 Position;
	glm::vec3 PositionDelta;
	glm::vec3 LookAt;
	glm::vec3 Direction;

	glm::vec3 Up;
	glm::quat Rotation;
	glm::vec3 Mouse;
};

