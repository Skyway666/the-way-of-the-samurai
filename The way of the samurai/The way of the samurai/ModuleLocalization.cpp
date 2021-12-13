﻿#include "ModuleLocalization.h"
#include "Application.h"
#include "Third Party/parson.h"

bool ModuleLocalization::Init()
{
	name = "Localization";

	bool ret = true;

	// Load localization data
	ret = LoadLocalizationData();

	// Load configured language for the games
	ret = ret && LoadLanguage();
	
	return ret;
}

bool ModuleLocalization::Exists(string key) const
{
	return app->localization->entries.find(key) != app->localization->entries.end();
}

string ModuleLocalization::GetLocalizatedText(string key)
{
	string ret = "";

	// Return localizated text if the key exists
	if (Exists(key) && language != "none")
		ret = entries[key][language];

	return ret;
}

void ModuleLocalization::SetLanguage(string newLanguage)
{
	language = newLanguage;

	// TODO: Upadte "Language.json" with new language
}

string ModuleLocalization::GetLanguage() const
{
	return language;
}

vector<string> ModuleLocalization::GetLanguages() const
{
	return languages;
}

bool ModuleLocalization::LoadLocalizationData()
{
	// Load localization file
	JSON_Value* rawFile = json_parse_file("Localization.json");
	// File was not found
	if (rawFile == nullptr)
		return true;

	// Get root localization object
	JSON_Object* localization = json_value_get_object(rawFile);
	// Error handling
	if (localization == nullptr)
	{
		app->logFatalError("No root object in the localization file");
		return false;
	}

	// Get languages array
	JSON_Array* s_languages = json_object_get_array(localization, "languages");
	// Error handling
	if (s_languages == nullptr)
	{
		app->logFatalError("No languages array in the localization file");
		return false;
	}

	// Load languages array
	for (int i = 0; i < json_array_get_count(s_languages); i++)
	{
		const char* language = json_array_get_string(s_languages, i);

		// Error handling
		if (language == nullptr)
		{
			app->logFatalError("Language array in localization file contains non 'string' type");
			return false;
		}
		else
			languages.push_back(language);
	}

	// Get entries array
	JSON_Array* entries_s = json_object_get_array(localization, "entries");
	// Error handling
	if (entries_s == nullptr)
	{
		app->logFatalError("No entries array in the localization file");
		return false;
	}

	// Load entries array
	for (int i = 0; i < json_array_get_count(entries_s); i++)
	{
		JSON_Object* entry = json_array_get_object(entries_s, i);

		if (entry == nullptr)
		{
			app->logFatalError("A value assigned to the entries array was not an object");
			return false;
		}

		const char* key = json_object_get_string(entry, "key");

		if (key == nullptr)
		{
			app->logFatalError("The key of an entry in the localization file was not of type 'string'");
			return false;
		}

		// Access each language value
		for (string language : languages)
		{
			const char* value = json_object_get_string(entry, language.c_str());

			if (value == nullptr)
			{
				app->logFatalError(("No 'string' type value for the '" + language + "' in the '" + key + "' entry").c_str());
				return false;
			}

			entries[key][language] = value;
		}
	}

	// TODO: Make sure memory is freed even if the function needs to return false
	// Use exception (try/catch/throw)
	json_value_free(rawFile);
	return true;
}

bool ModuleLocalization::LoadLanguage()
{
	JSON_Value* rawFile = json_parse_file("Language.json");

	// A language was stored
	if (rawFile != nullptr)
	{
		const char* char_language = json_value_get_string(rawFile);

		if (char_language == nullptr)
		{
			app->logFatalError(("Root value of 'Language.json' wasn't a string'"));
			return false;
		}

		language = char_language;
	}
	else
		language = "none";

	json_value_free(rawFile);
	return true;
}