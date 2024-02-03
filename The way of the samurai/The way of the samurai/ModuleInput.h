#pragma once
#include "Module.h"
#include <string>
using namespace std;

// Detects the current update loop input and exposes it to the rest of the engine
class ModuleInput : public Module
{
// Public methods
public:
	// Module inherited method
	void Update() override;

// Public variables
public:
	// Input provided in this loop
	string currentLoopInput;

// Private variables
private:
	char invalidCharacters[MAX_INVALID_CHARS] = { '@' };
	
};

