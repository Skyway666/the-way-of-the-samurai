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
	input = new ModuleInput();
	gameLogic = new ModuleGameLogic();
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
		mdl->Init();
		if (state == State::QUITTING)
			return;
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
		mdl->Start();
		if (state == State::QUITTING)
			return;
	}

	// Everything went right, next step
	state = State::UPDATING;
}

// Update all modules
void Application::Update()
{
	// Module iteration
	for (Module* mdl : modules)
		mdl->Update();
}

// Clean up all the modules (free the memory for the most part)
void Application::CleanUp()
{
	// Module iteration
	for (Module* mdl : modules)
	{
		mdl->CleanUp();

		// Destroy module
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

void Application::Terminate(const char* message)
{
	// Notify the user about the fatal error
	printf("----FATAL ERROR----\n");


	// TODO: Print module that failed and state application

	// Update application state
	state = State::QUITTING;

	// Print desired message
	printf(message);

	// Logs are allways separated by a newline for clarity
	printf("\n");
}
