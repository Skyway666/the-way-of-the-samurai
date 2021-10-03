#pragma once
#include "Module.h"
#include "ModuleGameImporter.h"
#include "ModuleInput.h"
#include <vector>

// Manages the application
class Application
{
	// Sub structures
public:

	enum class State
	{
		CREATING,
		STARTING,
		UPDATING,
		QUITTING
	};

// Public Methods
public:
	// Initialization of the application
	void Init();
	// First logical frame
	void Start();
	// Logic update of the app
	void Update();
	// To be called when the application is shut down
	void CleanUp();


	// Check state of the app
	const State GetState();
	// Log messages to the console
	const void log(const char* message);

// Public attributes
public:

	// Pointers to modules for global access
	ModuleGameImporter* gameImporter = nullptr;
	ModuleInput* input = nullptr;

// Private attributes
private:
	// State of the app
	State state = State::CREATING;

	// Modules
	std::vector<Module*> modules;
};

extern Application* app;
