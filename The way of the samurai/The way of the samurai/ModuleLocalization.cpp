﻿#include "ModuleLocalization.h"
#include "Application.h"
#include "Third Party/parson.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

void ModuleLocalization::Init()
{
	name = "Localization";

	// Load localization data
	LoadLocalizationData();

	// Load configured language for the games
	LoadLanguage();
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

void ModuleLocalization::HandleLocalization(string& text)
{
	// 'text' exists as a localization entry key
	if (Exists(text))
	{
		// Overrwrite 'text' with localization value
		text = GetLocalizatedText(text);
	}
}

void ModuleLocalization::SetUserDefaultUILanguage()
{
	// Retrieve system code (Windows code)
	char isoCodeCharArr[3];
	GetLocaleInfoA(GetUserDefaultUILanguage(), LOCALE_SISO639LANGNAME, isoCodeCharArr, sizeof(isoCodeCharArr));
	string isoCode = isoCodeCharArr;

	// Compute defaultLanguageIndex within the avaliable languages
	vector<string>::iterator defaultLanguageIt = std::find(languages.begin(), languages.end(), isoCode);
	int defaultLanguageIndex = 0;
	if (defaultLanguageIt != languages.end())
		defaultLanguageIndex = defaultLanguageIt - languages.begin();
	
	// Set defaultLanguage
	SetLanguage(languages[defaultLanguageIndex]);
}

void ModuleLocalization::SetLanguage(string newLanguage)
{
	// Update language
	language = newLanguage;

	// Save in language file
	ofstream language_json(string(app->userFilesFolderPath) + "Language.json");
	language_json << "\"" << language << "\"";
	language_json.close();
}

string ModuleLocalization::GetLanguage() const
{
	return language;
}

vector<string> ModuleLocalization::GetLanguages() const
{
	return languages;
}

void ModuleLocalization::LoadLocalizationData()
{
	// Load localization file
	JSON_Value* rawFile = json_parse_file_with_comments("Localization.json");
	// File was not found
	if (rawFile == nullptr)
		return;

	// Get root localization object
	JSON_Object* localization = json_value_get_object(rawFile);
	// Error handling
	if (localization == nullptr)
	{
		TerminateApplication("No root object in the localization file");
		return;
	}

	// Get languages array
	JSON_Array* s_languages = json_object_get_array(localization, "languages");
	// Error handling
	if (s_languages == nullptr)
	{
		TerminateApplication("No languages array in the localization file");
		return;
	}

	// Load languages array
	for (int i = 0; i < json_array_get_count(s_languages); i++)
	{
		const char* language = json_array_get_string(s_languages, i);

		// Error handling
		if (language == nullptr)
		{
			TerminateApplication("Language array in localization file contains non 'string' type");
			return;
		}
		else
			languages.push_back(language);
	}

	// Get the tables array
	JSON_Array* tables = json_object_get_array(localization, "tables");
	// Error handling
	if (tables == nullptr) 
	{
		TerminateApplication("No tables array in the localization file");
		return;
	}

	// Load tables
	for (int i = 0; i < json_array_get_count(tables); i++) 
	{
		// Get file name
		const char* tableFileName = json_array_get_string(tables, i);
		// Error handling
		if (tableFileName == nullptr) 
		{
			TerminateApplication("Tables array contains non 'string' type");
			return;
		}

		// Load table file
		JSON_Value* tableRawFile = json_parse_file_with_comments(tableFileName);
		// Error handling
		if (tableRawFile == nullptr) 
		{
			TerminateApplication(("Couldn't load '" + string(tableFileName) + "' as localization table").c_str());
			return;
		}

		// Get table array
		JSON_Array* table = json_value_get_array(tableRawFile);
		// Error handling
		if (table == nullptr) 
		{
			TerminateApplication((string(tableFileName) + " didn't contain an array as root value").c_str());
			return;
		}

		// Load table entries array
		for (int i = 0; i < json_array_get_count(table); i++)
		{
			JSON_Object* entry = json_array_get_object(table, i);

			if (entry == nullptr)
			{
				TerminateApplication("A value assigned to the entries array was not an object");
				return;
			}

			const char* key = json_object_get_string(entry, "key");

			if (key == nullptr)
			{
				TerminateApplication("The key of an entry in the localization file was not of type 'string'");
				return;
			}

			// Access each language value
			for (string language : languages)
			{
				const char* value = json_object_get_string(entry, language.c_str());

				if (value == nullptr)
				{
					TerminateApplication(("No 'string' type value for the '" + language + "' in the '" + key + "' entry").c_str());
					return;
				}

				entries[key][language] = value;
			}
		}

		json_value_free(tableRawFile);
	}

	// TODO: Make sure memory is freed even if the function needs to return false
	// Use exception (try/catch/throw)
	json_value_free(rawFile);
}

void ModuleLocalization::LoadLanguage()
{
	JSON_Value* rawFile = json_parse_file_with_comments((string(app->userFilesFolderPath) + "Language.json").c_str());

	// A language was stored
	if (rawFile != nullptr)
	{
		// Load language
		const char* char_language = json_value_get_string(rawFile);

		if (char_language == nullptr)
		{
			TerminateApplication(("Root value of 'Language.json' wasn't a string'"));
			return;
		}

		// Assign language
		language = char_language;

		json_value_free(rawFile);
	}
	else
		language = "none";
}