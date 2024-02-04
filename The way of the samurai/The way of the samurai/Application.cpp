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

void Application::Exit()
{
	state = State::QUITTING;
}

void Application::Terminate(const char* message, const Module* module)
{
	// Notify the user about the fatal error
	Log("----FATAL ERROR----");


	// TODO: Print module that failed and state application
	Log((module->name + " failed during " + State2String(state)).c_str());

	// Exit application
	Exit();

	// Print desired message
	Log(message);
}

string Application::State2String(State state) const
{
	switch (state) 
	{
		case State::CREATING: 
			return "CREATING";
			break;
		case State::STARTING:
			return "STARTING";
			break;
		case State::UPDATING:
			return "UPDATING";
			break;
		case State::QUITTING:
			return "QUITTING";
			break;
	}
}
