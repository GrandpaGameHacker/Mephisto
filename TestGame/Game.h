#pragma once
#include <App/IApplication.h>
#include <Camera/BasicCamera.h>
#include <GUI/IWindow.h>
#include <Math/GameMath.h>
#include <Assets/ShaderAsset.h>
#include <Core/GameObject.h>

class Game :
    public IApplication
{
public:
    Game();
    Game(int argc, char* argv[]);
    virtual ~Game();
    void Initialize() override;
    void AppTick() override;
    void DrawTick() override;
    void EventTick(SDL_Event* event) override;
};