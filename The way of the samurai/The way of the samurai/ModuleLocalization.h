#pragma once
#include "Module.h"
#include <map>
#include <vector>
#include <string>

// Loads the data in the "Localization.json" file and exposes it to the rest of the engine
class ModuleLocalization : public Module
{
	// Public methods
public:
	bool Init() override;

	// Public variables
public:
	vector<string> languages;
	map<string, map<string, string>> entries;
};

