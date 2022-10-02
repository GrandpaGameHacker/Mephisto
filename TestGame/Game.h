#pragma once
#include "App/IApplication.h"
#include "Assets/ShaderAsset.h"
#include "Assets/TextureAsset.h"
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
    std::shared_ptr<TextureAsset> Tex;
};

