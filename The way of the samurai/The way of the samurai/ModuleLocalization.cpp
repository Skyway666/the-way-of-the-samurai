#include "ModuleLocalization.h"
#include "Application.h"
#include "Third Party/parson.h"

bool ModuleLocalization::Init()
{
	name = "Localization";

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

	return true;
}
