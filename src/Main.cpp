#include "core/Application.h"
#include "states/PlanetCreator.h"

int main()
{
	Application* app = new Application(1280, 720, "App", new PlanetCreator());
	app->Run();
	delete app;

	return 0;
}
