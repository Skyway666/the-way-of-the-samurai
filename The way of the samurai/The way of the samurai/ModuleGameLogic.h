#pragma once
#include "Module.h"
#include <list>
#include <string>
#include <vector>
#include <map>
using namespace std;

// Forward declaration
typedef struct json_object_t JSON_Object;
struct MapEvent;
struct Event;
class LogicProcessor;
class MapNavigationLogic;
class MapEventsLogic;
class VariableSavingLogic;
class LanguageChoosingLogic;
class OptionsChoosingLogic;
enum class LogicProcessorResult;


class ModuleGameLogic : public Module
{
	// Public definitions
public:
	struct GameData
	{
		// Key variables that define the state of the game
		list<string> conditions;
		// Player objects that can be consumed
		list<string> objects;
		// Custom variables save
		map<string, string> savedVariables;
		// Position of the player in the map
		int currentGridPosition = 0;


		void Save(JSON_Object* s_GameData);
	};

	// Public methods
public:
	// Module inherited methods
	bool Init() override;
	bool Start() override;
	bool Update() override;
	bool CleanUp() override;

	// Replaces keys between '@' with gameplay variables
	void ReplaceVariables(string& text) const;
	// Private methods
private:

	// Sets a new logic processor while updating "lastLogicProcessor"
	void SetLogicProcessor(LogicProcessor* newLogicProcessor);
	// Modify logic state depending on the logic processor result
	bool HandleLogicProcessorResult(LogicProcessorResult result);
	// Returns current map event depending in the gameState.currentGridPosition
	MapEvent* GetCurrentMapEvent() const;
	// Switches the logic processor to "mapNavigation" and notifies the player
	void BackToMap();
	// Goes back to the previous logic processor
	void BackToLastLogicalState();

	// Private attributes
private:
	// ----------GAME STATE VARIABLES
	GameData gameState;

	// ---------- LOGIC PROCESSORS VARIABLES
	LogicProcessor* currentLogicProcessor = nullptr;
	LogicProcessor* lastLogicProcessor = nullptr;

	// ----------LOGIC PROCESSORS
	MapNavigationLogic* mapNavigation = nullptr;
	MapEventsLogic* mapEvents = nullptr;
	VariableSavingLogic* variableSaving = nullptr;
	LanguageChoosingLogic* languageChoosing = nullptr;
	OptionsChoosingLogic* optionsChoosing = nullptr;
};

// If 'text' is a localization key, substitutes its value depending on the configured language
void HandleLocalization(string& text);
// Logs a text after processing it as a gameplay one
void LogGameplayText(string text);
// Displays the options the user can choose from
void DisplayOptions(const vector<string>& options);
// Display a list of elements
void DisplayList(const vector<string>& options);
