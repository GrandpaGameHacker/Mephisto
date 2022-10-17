#include "Game.h"
#include <SDL2/SDL.h>
#include <Core/MephistoAssert.h>
#include <App/CommandLine.h>
#include <Strings/StringUtilities.h>
Game::Game()
{

}

Game::Game(int argc, char* argv[])
{
	CommandLine::Initialize(argc, argv);
}

Game::~Game()
{

}

// Initialize Major Game State Here
void Game::Initialize()
{
	Create("3D Game Test App", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600 }, NULL);
};

// Calculate Game Logic Here Per frame
void Game::AppTick()
{

}

// OpenGL clear and render is already handled by IApplication
// You just draw objects here
void Game::DrawTick()
{

}


// Add Kb/Mouse/Other Event Handlers here
// Overload OnWindow... functions for window events
void Game::EventTick(SDL_Event* event)
{

	IApplication::EventTick(event);
}
