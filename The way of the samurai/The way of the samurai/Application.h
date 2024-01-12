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
	State GetState() const;
	// Log messages to the console
	void Log(const char* message) const;
	// Log fatal error
	void LogFatalError(const char* message) const;

// Public attributes
public:

	// Pointers to modules for global access
	ModuleGameLogic* gameLogic = nullptr;
	ModuleInput* input = nullptr;
	ModuleGameImporter* gameImporter = nullptr;
	ModuleLocalization* localization = nullptr;

// Private attributes
private:
	// State of the app
	State state = State::CREATING;

	// Modules
	std::vector<Module*> modules;
};

extern Application* app;
