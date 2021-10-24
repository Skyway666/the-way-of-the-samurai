#include <stdio.h>
#include "Application.h"

Application* app = nullptr;

void main() 
{
	// Create our application and make the pointer globally accessible
	app = new Application();

	// Main loop
	while (true) 
	{
		bool quit = false;
		// Perform app actions depending on the state
		switch (app->GetState()) 
		{
			case Application::State::CREATING:
			{
				app->Init();
				break;
			}
			case Application::State::STARTING:
			{
				app->Start();
				break;
			}
			case Application::State::UPDATING:
			{
				app->Update();
				break;
			}
			case Application::State::QUITTING:
			{
				app->CleanUp();
				quit = true;
			}
		}

		if (quit)
			break;
	}

	app->log("Application terminated \n");

	// Clear app
	delete app;
	app = nullptr;
	return;
}