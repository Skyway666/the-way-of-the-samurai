#pragma once
#include "Module.h"
#include <vector>

// Forward declaration
class ModuleGameLogic;
class ModuleGameLogic;
class ModuleInput;
class ModuleGameImporter;
class ModuleLocalization;


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

// Public methods
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
	State GetState() const;
	// Log messages to the console
	void Log(const char* message) const;
	// Log fatal error
	void Terminate(const char* message, const Module* module);

// Public variables
public:

	// Pointers to modules for global access
	ModuleGameLogic* gameLogic = nullptr;
	ModuleInput* input = nullptr;
	ModuleGameImporter* gameImporter = nullptr;
	ModuleLocalization* localization = nullptr;

// Private methods
private:
	string State2String(State state) const;

// Private variables
private:
	// State of the app
	State state = State::CREATING;

	// Modules
	std::vector<Module*> modules;
};

extern Application* app;
