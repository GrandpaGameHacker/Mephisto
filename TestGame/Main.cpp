#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include "Main.h"
#include "Game.h"
#include "stb_image.h"

int main(int argc, char* argv[])
{
	SDL_SetMainReady();

	stbi_set_flip_vertically_on_load(true);
	stbi_set_flip_vertically_on_load_thread(true);
	Game app = Game(argc, argv);
	app.Begin();
	return 0;
}