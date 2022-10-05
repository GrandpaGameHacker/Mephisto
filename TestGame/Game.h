#pragma once
#include "App/IApplication.h"
#include "Assets/ModelAsset.h"
#include "Camera/BasicCamera.h"
#include "GUI/IWindow.h"

struct GameObject {
	std::shared_ptr<ModelAsset> Model;
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};

class TestGUI : public IWindow
{
public:
    TestGUI();
    void Draw() override;
    const char* WindowTitle = "TestingGUI";
    GameObject* ObjPtr1;
    GameObject* ObjPtr2;
};

class Game :
    public IApplication
{
public:
    Game();
    virtual ~Game();
    void Initialize() override;
    void AppTick() override;
    void DrawTick() override;
    void EventTick(SDL_Event* event) override;
    TestGUI Gui;
    Shader GameShader;
    BasicCamera MyCamera;
    GameObject GameObject1, GameObject2;
};

