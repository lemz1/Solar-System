#include "core/Application.h"
#include "state/MoonCreator.h"

int main()
{
	Application* app = new Application(1280, 720, "App", new MoonCreator());
	app->Run();
	delete app;

	return 0;
}
