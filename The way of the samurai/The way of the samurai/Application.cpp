#include "Application.h"
#include "ModuleGameLogic.h"
#include "ModuleInput.h"
#include "ModuleGameImporter.h"
#include "ModuleLocalization.h"
#include <stdio.h>

// Initializes app by creating the Modules and calling "Init" on them
void Application::Init()
{
	// Create modules
	gameLogic = new ModuleGameLogic();
	input = new ModuleInput();
	gameImporter = new ModuleGameImporter();
	localization = new ModuleLocalization();


	// Save in the app
	modules.push_back((Module*)input);
	modules.push_back((Module*)gameLogic);
	modules.push_back((Module*)gameImporter);
	modules.push_back((Module*)localization);


	// Initialize
	for (Module* mdl : modules)
	{
		if (!mdl->Init())
		{
			state = State::QUITTING;
			return;
		}
	}

	// Everything went right, next step
	state = State::STARTING;
}

// Start all the modules
void Application::Start()
{
	// Module iteration
	for (Module* mdl : modules)
	{
		// If a module fails the application quits
		if (!mdl->Start())
		{
			state = State::QUITTING;
			return;
		}
	}

	// Everything went right, next step
	state = State::UPDATING;
}

// Update all modules
void Application::Update()
{
	// Module iteration
	for (Module* mdl : modules)
	{
		// If a module fails the application quits
		if (!mdl->Update())
		{
			state = State::QUITTING;
			return;
		}
	}

	// Everything went right, keep updating;
}

// Clean up all the modules (free the memory for the most part)
void Application::CleanUp()
{
	// Module iteration
	for (Module* mdl : modules)
	{
		// If a module fails to clean up a log is printed. 
		if (!mdl->CleanUp())
			Log(("Module " + mdl->name + " failed to clean up").c_str());

		// Completelly erase the module
		delete mdl;
	}

	// Clear the module's list
	modules.clear();
}

Application::State Application::GetState() const
{
	return state;
}

void Application::Log(const char* message) const
{
	printf(message);

	// Logs are allways separated by a newline for clarity
	printf("\n");
}

void Application::LogFatalError(const char* message) const
{
	// Notify the user about the fatal error
	printf("----FATAL ERROR----");

	// Print desired message
	printf(message);

	// Logs are allways separated by a newline for clarity
	printf("\n");
}
