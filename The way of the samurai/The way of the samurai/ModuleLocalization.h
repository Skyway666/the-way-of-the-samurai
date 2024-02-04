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
	void Init() override;

	// Public methods
public:
	// Checks if a key exists within the "entries"
	bool Exists(string key) const;
	// Gets value for a key depending on the configured language. Returns an empty string if the key has no value
	string GetLocalizatedText(string key);
	// If 'text' is a localization key, substitutes its value depending on the configured language
	void HandleLocalization(string& text);
	// Tries to set the system's default language and if it fails assigns the first available language
	void SetUserDefaultUILanguage();
	// Sets language
	void SetLanguage(string newLanguage);
	// Gets language
	string GetLanguage() const;
	// Gets languages
	vector<string> GetLanguages() const;

	// Private variables
private:
	// Language file path
	const char* languagePath = "Language.json";
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
	void LoadLocalizationData();
	// Loads language stored in "Language.json"
	void LoadLanguage();
};

