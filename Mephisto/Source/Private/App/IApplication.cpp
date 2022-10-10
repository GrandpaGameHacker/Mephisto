#include "App/IApplication.h"
#include "GUI/Backend/imgui_impl_opengl3.h"
#include "GUI/Backend/imgui_impl_sdl.h"
#include "GUI/IWindow.h"
IApplication::~IApplication()
{
    spdlog::info("Application:"+ Name+ " - Is Being Destroyed");
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(&GLContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

IApplication::IApplication(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        std::string arg = argv[i];
        args.push_back(arg);
    }
}

bool IApplication::Create(const char* Title, SDL_Rect Dimensions, unsigned int Flags)
{
    Name = Title;
	EnableWindowsDpiScaling();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		WindowCreationError("SDL_Init(SDL_INIT_EVERYTHING) Failed");
		return false;
	}
	
	Window = SDL_CreateWindow(Title, Dimensions.x, Dimensions.y, Dimensions.w, Dimensions.h,
		Flags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	
	if (!Window)
	{
		WindowCreationError("SDL_CreateWindow Failed");
		return false;
	}

#ifdef _DEBUG_RENDERDOC_
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

	GLContext = SDL_GL_CreateContext(Window);
	if (!GLContext)
	{
		WindowCreationError("SDL_GL_CreateContext Failed");
		return false;
	}
	spdlog::info("App Window \"{}\" created successfully", Title);
	UpdateScreenRect();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(Window, GLContext);
	ImGui_ImplOpenGL3_Init();


	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glDepthFunc(GL_LESS);
	return true;
}

bool IApplication::Show(bool bShow)
{
	if (bIsWindowValid && bIsRunning)
	{
		if (bShow)
		{
			SDL_ShowWindow(Window);
		}
		else
		{
			SDL_HideWindow(Window);
		}
		return true;
	}
	return false;
}


SDL_GLContext IApplication::GetGLContext()
{
	if (IsValid()) {
		return GLContext;
	}
	return nullptr;
}

SDL_Window* IApplication::GetAppWindow()
{
	if (IsValid()) {
		return Window;
	}
	return nullptr;
}
bool IApplication::IsRunning()
{
	return bIsRunning;
}

bool IApplication::IsInitialized()
{
	return bIsWindowValid;
}

bool IApplication::IsValid() {
	if (bIsRunning && bIsWindowValid) {
		return true;
	}
	return false;
}

void IApplication::Exit(std::string ExitReason)
{
	spdlog::debug(ExitReason);
	exit(0);
}

SDL_Rect IApplication::UpdateScreenRect()
{
	int w, h;
	SDL_GetWindowSize(Window, &w, &h);
	ScreenRect = { 0, 0,w, h };
	return ScreenRect;
}

glm::ivec2 IApplication::GetDrawableSize() const
{
	int Width, Height;
	SDL_GL_GetDrawableSize(Window, &Width, &Height);
	return glm::ivec2({ Width,Height });
}

float IApplication::GetDeltaTime()
{
	typedef std::chrono::high_resolution_clock Time;
	static long long started = Time::now().time_since_epoch().count();
	long long now = Time::now().time_since_epoch().count();
	return static_cast<float>(now - started) / 1000000000.f;
}

long long IApplication::GetDeltaTimeNano()
{
	typedef std::chrono::high_resolution_clock Time;
	static long long started = Time::now().time_since_epoch().count();
	long long now = Time::now().time_since_epoch().count();
	return now - started;
}

void IApplication::WindowCreationError(std::string error)
{
	spdlog::info(error);
	bIsWindowValid = false;
	bIsRunning = false;
}

void IApplication::EventTick(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
	switch (event->type)
	{
	case SDL_QUIT:
		bIsRunning = false;
		break;
	case SDL_WINDOWEVENT:
		WindowEventDispatch(event);
	default:
		break;
	}
}

void IApplication::Begin()
{
	Initialize();
	while (bIsRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			EventTick(&event);
		}
		AppTick();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		IWindow::DrawGUI();
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);;
		DrawTick();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(Window);
	}
}

void IApplication::EnableWindowsDpiScaling()
{
	typedef enum PROCESS_DPI_AWARENESS
	{
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	} PROCESS_DPI_AWARENESS;

	void* userDLL;
	BOOL(WINAPI * SetProcessDPIAware)(void) = nullptr; // Vista and later
	void* shcoreDLL;
	HRESULT(WINAPI * SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS dpiAwareness) = nullptr; // Windows 8.1 and later

	userDLL = SDL_LoadObject("USER32.DLL");
	if (userDLL) {
		SetProcessDPIAware = (BOOL(WINAPI*)(void)) SDL_LoadFunction(userDLL, "SetProcessDPIAware");
	}

	shcoreDLL = SDL_LoadObject("SHCORE.DLL");
	if (shcoreDLL) {
		SetProcessDpiAwareness = (HRESULT(WINAPI*)(PROCESS_DPI_AWARENESS)) SDL_LoadFunction(shcoreDLL, "SetProcessDpiAwareness");
	}

	if (SetProcessDpiAwareness) {
		/* Try Windows 8.1+ version */
		const HRESULT result = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		spdlog::info("called SetProcessDpiAwareness -> {}", (result == S_OK) ? "succeeded" : "failed");
	}
	else if (SetProcessDPIAware) {
		/* Try Vista - Windows 8 version.
		This has a constant scale factor for all monitors.
		*/
		const BOOL success = SetProcessDPIAware();
		spdlog::info("called SetProcessDPIAware -> {}", (success) ? "succeeded" : "failed");
	}
}

void IApplication::WindowEventDispatch(SDL_Event* event)
{
	switch (event->window.event)
	{
	case SDL_WINDOWEVENT_NONE:
		return;

	case SDL_WINDOWEVENT_SHOWN:
		OnWindowShown(event);
		break;

	case SDL_WINDOWEVENT_HIDDEN:
		OnWindowHidden(event);
		break;

	case SDL_WINDOWEVENT_EXPOSED:
		OnWindowExposed(event);
		break;

	case SDL_WINDOWEVENT_MOVED:
		OnWindowMoved(event);
		break;

	case SDL_WINDOWEVENT_RESIZED:
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		OnWindowResize(event);
		break;

	case SDL_WINDOWEVENT_MINIMIZED:
		OnWindowMinimized(event);
		break;

	case SDL_WINDOWEVENT_MAXIMIZED:
		OnWindowMaximized(event);
		break;

	case SDL_WINDOWEVENT_RESTORED:
		OnWindowRestored(event);
		break;

	case SDL_WINDOWEVENT_ENTER:
		OnWindowEnter(event);
		break;

	case SDL_WINDOWEVENT_LEAVE:
		OnWindowLeave(event);
		break;

	case SDL_WINDOWEVENT_FOCUS_GAINED:
	case SDL_WINDOWEVENT_FOCUS_LOST:
		OnWindowFocusChanged(event);
		break;

	case SDL_WINDOWEVENT_CLOSE:
		OnWindowClose(event);
		break;

	case SDL_WINDOWEVENT_DISPLAY_CHANGED:
		OnWindowDisplayChanged(event);
	default:
		return;
	}
}

// Needs a refactor if you want to support multiple viewport cameras
void IApplication::OnWindowResize(SDL_Event* event)
{
	UpdateScreenRect();
	glViewport(ScreenRect.x, ScreenRect.x, ScreenRect.w, ScreenRect.h);
}

void IApplication::OnWindowClose(SDL_Event* event)
{
	bIsRunning = false;
}