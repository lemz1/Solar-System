#include "Core/Application.h"
#include "States/PlanetCreator.h"

#include "Celestial-Bodies/IcoSphere.h"
#include "Util/AssetManager.h"

int main()
{
	Application* app = new Application(1280, 720, "App", new PlanetCreator());
	app->Run();
	delete app;

	return 0;
}
