#pragma once
#include "App/IApplication.h"
#include "Assets/ModelAsset.h"
#include <Camera/BasicCamera.h>

struct GameObject {
    std::shared_ptr<ModelAsset> Model;
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
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
    Shader GameShader;
    BasicCamera MyCamera;
    GameObject GameObject1, GameObject2;
};

