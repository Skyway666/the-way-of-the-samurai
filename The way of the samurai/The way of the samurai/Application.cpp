#include "Application.h"
#include "ModuleGameImporter.h"
#include "ModuleInput.h"
#include "ModuleGameLogic.h"
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
	modules.push_back((Module*)gameLogic);
	modules.push_back((Module*)input);
	modules.push_back((Module*)gameImporter);
	modules.push_back((Module*)localization);


	// Initialize
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		if (!(*it)->Init())
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
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it) 
	{
		// If a module fails the application quits
		if (!(*it)->Start()) 
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
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		// If a module fails the application quits
		if (!(*it)->Update())
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
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it) 
	{
		// If a module fails to clean up a log is printed. 
		if (!(*it)->CleanUp())
			log(("Module " + (*it)->name + " failed to clean up").c_str());

		// Completelly erase the module
		delete (*it);
	}

	// Clear the module's list
	modules.clear();
}

Application::State Application::GetState() const
{
	return state;
}

void Application::log(const char* message) const
{
	printf(message);

	// Logs are allways separated by a newline for clarity
	printf("\n");
}

void Application::logFatalError(const char* message) const
{
	// Notify the user about the fatal error
	printf("----FATAL ERROR----");

	// Print desired message
	printf(message);

	// Logs are allways separated by a newline for clarity
	printf("\n");
}
