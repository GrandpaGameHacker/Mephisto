#pragma once
#include <spdlog/spdlog.h>
#include <gl/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <App/CommandLine.h>

class IApplication
{
public:
	virtual ~IApplication();
	IApplication() {};

	SDL_Window* GetAppWindow();
	SDL_GLContext GetGLContext();

	bool IsRunning();
	bool IsInitialized();

	SDL_Rect UpdateScreenRect();
	glm::ivec2 GetDrawableSize() const;

	float GetDeltaTime();
	long long GetDeltaTimeNano();
	void Begin();

private:
	void EnableWindowsDpiScaling();
	void WindowCreationError(std::string error);
	bool IsValid();

protected:

	virtual void Initialize() {};

	virtual void WindowEventDispatch(SDL_Event* event);

	/*
	WindowEventDispatch calls these event handlers
	Please call IApplication::On... in the overridden handlers
	*/
	virtual void OnWindowResize(SDL_Event* event);
	virtual void OnWindowClose(SDL_Event* event);

	virtual void OnWindowShown(SDL_Event* event) {};
	virtual void OnWindowHidden(SDL_Event* event) {};
	virtual void OnWindowExposed(SDL_Event* event) {};
	virtual void OnWindowMoved(SDL_Event* event) {};
	virtual void OnWindowMinimized(SDL_Event* event) {};
	virtual void OnWindowMaximized(SDL_Event* event) {};
	virtual void OnWindowRestored(SDL_Event* event) {};
	virtual void OnWindowEnter(SDL_Event* event) {};
	virtual void OnWindowLeave(SDL_Event* event) {};
	virtual void OnWindowFocusChanged(SDL_Event* event) {};
	virtual void OnWindowDisplayChanged(SDL_Event* event) {};

	virtual void EventTick(SDL_Event* event);
	virtual void AppTick() {};
	virtual void DrawTick() {};

	virtual void Exit(std::string ExitReason);
	bool Create(const char* Title, SDL_Rect Dimensions, unsigned int Flags);
	bool Show(bool bShow);

	std::string Name;
	SDL_Window* Window = nullptr;
	SDL_GLContext GLContext = nullptr;
	SDL_Rect ScreenRect = {};
	bool bIsRunning = true;
	bool bIsWindowValid = true;
};

