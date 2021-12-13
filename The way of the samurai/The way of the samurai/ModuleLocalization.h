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

	// Public methods
public:
	// Checks if a key exists within the "entries"
	bool Exists(string key) const;
	// Gets value for a key depending on the configured language. Returns an empty string if the key has no value
	string GetLocalizatedText(string key);
	// Sets language
	void SetLanguage(string newLanguage);
	// Gets language
	string GetLanguage() const;
	// Gets languages
	vector<string> GetLanguages() const;

	// Private variables
private:
	// Current configured language. It has the value "none" if no language is configured
	string language;
	// Languages supported by the game
	vector<string> languages;
	// Entries to the localization system
	// - entries[key][language]
	map<string, map<string, string>> entries;

	// Private methods
private:
	// ----------LOGIC ENCAPSULATORS (only for readibility, this functions shouldn't be used more than once)
	// Loads "Localization.json"
	bool LoadLocalizationData();
	// Loads language stored in "Language.json"
	bool LoadLanguage();
};

