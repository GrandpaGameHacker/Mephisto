#include "Game.h"
#include <SDL2/SDL.h>

Game::Game()
{

}

Game::~Game()
{

}

// Initialize Major Game State Here
void Game::Initialize()
{
	Create("3D Game Test App", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600 }, NULL);
	GameShader.AddStageFile(EShaderStage::Vertex, "/GameAssets/Shaders/BasicShader.vs");
	GameShader.AddStageFile(EShaderStage::Fragment, "/GameAssets/Shaders/BasicShader.fs");
	GameShader.Compile();
	GameObject1.Model = ModelAsset::LoadCachedAsset("/GameAssets/Models/Backpack/backpack.obj");
	GameObject2.Model = ModelAsset::LoadCachedAsset("/GameAssets/Models/Backpack/backpack.obj");
	MyCamera.SetViewport(ScreenRect);
	MyCamera.SetType(CameraType::Perspective);
	MyCamera.SetPosition({ 0.f, 0.f, -50.f });

	GameObject1.Position = { 0,5,0 };
	GameObject1.Scale = { 1, 2, 1 };

	GameObject2.Position = { 5,0,0 };
	GameObject2.Scale = { 2, 1, 1 };

}

// Calculate Game Logic Here Per frame
void Game::AppTick()
{
	GameObject1.Rotation += glm::vec3{ 0,glm::radians(1.0f),0 };
	GameObject2.Rotation += glm::vec3{ glm::radians(1.0f),0,0 };
}

// OpenGL clear and render is already handled by IApplication
// You just draw objects here
void Game::DrawTick()
{

	MyCamera.Update();

	glm::mat4 Model = glm::mat4(1.0f);
	auto View = MyCamera.GetViewMatrix();
	auto Projection = MyCamera.GetProjectionMatrix();
	Model = glm::translate(Model, GameObject1.Position);
	Model = glm::rotate(Model, GameObject1.Rotation.x, { -1,0,0 });
	Model = glm::rotate(Model, GameObject1.Rotation.y, { 0,1,0 });
	Model = glm::scale(Model, GameObject1.Scale);
	GameShader.Bind();
	glm::mat4 MVP = Projection * View * Model;
	GameShader.Set("MVP", MVP);
	GameObject1.Model->Draw(GameShader);

	Model = glm::mat4(1.0f);
	View = MyCamera.GetViewMatrix();
	Projection = MyCamera.GetProjectionMatrix();
	Model = glm::translate(Model, GameObject2.Position);
	Model = glm::rotate(Model, GameObject2.Rotation.x, { -1,0,0 });
	Model = glm::rotate(Model, GameObject2.Rotation.y, { 0,1,0 });
	Model = glm::scale(Model, GameObject2.Scale);
	GameShader.Bind();
	MVP = Projection * View * Model;
	GameShader.Set("MVP", MVP);
	GameObject1.Model->Draw(GameShader);
}


// Add Kb/Mouse/Other Event Handlers here
// Overload OnWindow... functions for window events
void Game::EventTick(SDL_Event* event)
{

	IApplication::EventTick(event);
}
