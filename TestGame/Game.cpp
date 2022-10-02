#include "Game.h"
#include <SDL2/SDL.h>
Game::Game()
{
}

Game::~Game()
{

}

void Game::Initialize()
{
	Create("3D Game Test App", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600 }, NULL);
	GameShader.AddStageFile(EShaderStage::Vertex, "/GameAssets/Shaders/BasicShader.vs");
	GameShader.AddStageFile(EShaderStage::Fragment, "/GameAssets/Shaders/BasicShader.fs");
	GameShader.Compile();

	Tex = TextureAsset::LoadAsset("/GameAssets/Textures/tex.bmp", GL_TEXTURE_2D);
	Tex->Bind(0);
}

void Game::AppTick()
{
	// Calculate Game Logic Here
}

void Game::DrawTick()
{
	// OpenGL clear and render is already handled by IApplication
	// You just draw objects here
}

void Game::EventTick(SDL_Event* event)
{
	// Add event handlers here. overload OnWindow.. functions for Window event handling
	// OnWindowResize and OnWindowClose are already implemented, so call IApplication::OnWindowResize in your overridden function
	IApplication::EventTick(event);
	//switch (event->type)
	//{
	//default:
	//	break;
	//}
}
