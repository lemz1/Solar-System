#include "Core/Application.h"
#include "States/PlanetCreator.h"

int main(int, char**)
{
	Application* app = new Application(1280, 720, "App", new PlanetCreator());
	app->Run();
	delete app;
}
