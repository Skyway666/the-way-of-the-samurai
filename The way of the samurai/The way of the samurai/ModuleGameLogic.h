#pragma once
#include "Module.h"
#include <list>
#include <string>s
#include <vector>
#include <map>
using namespace std;

// Forward definition
typedef struct json_object_t JSON_Object;
struct MapEvent;
struct Event;



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
		map<string,string> savedVariables;
		// Position of the player in the map
		int currentGridPosition = 0;

		void Save(JSON_Object* s_GameData);
	};

	// Events are loaded based on the logic state
	enum class LogicState
	{
		INITIALIZATION,
		NAVIGATING_MAP,
		IN_EVENT,
		SAVING_VARIABLE,
		SAVING_VARIABLE_CONFIRMATION,
		SKIP
	};

	// Possible outcomes of loading an event
	enum class PlayerInputResult
	{
		FATAL_ERROR,
		EVENT_LOADED,
		EVENT_BRANCHED,
		EVENT_ENDED,
		INVALID_INPUT,
		VARIABLE_SAVED,
		VARIABLE_LOADED,
		VARIABLE_DISCARTED
	};

// Public methods
public:
	// Module inherited methods
	bool Init() override;
	bool Start() override;
	bool Update() override;

// Private methods
private:
	// ----------MAIN UPDATE METHODS
	// Handles player input
	PlayerInputResult HandlePlayerInput();
	// Handles an event
	void HandleCurrentEvent();


	// ----------LOGIC ENCAPSULATORS (only for readibility)
	
	// ----------RECURRENT ACTIONS
	// Returns current map event depending in the currentGridPosition
	MapEvent* GetCurrentMapEvent() const;
	// Use input to update grid position. Returns false if the input was invalid
	bool UpdateGridPosition();
	// Sets the new event to handle checking for alternative events
	void SetHandlingEvent(Event* newHandlingEvent);
	// Displays the options the user can choose from
	void DisplayOptions(vector<string>& options) const;
	// Sets the logic state to "NAVIGATING_MAP" and notifies the player
	void BackToMap();
	// Handles the display of sub events of an event
	void HandleSubEventsDisplay(Event* eventToBranch);
	// Logs a text after processing it as a gameplay one
	void logGameplayText(string text) const;
	// Replaces keys between '@' with gameplay variables
	void replaceVariables(string& text) const;

// Private attributes
private:
	// ----------GAME STATE VARIABLES
	GameData gameState;

	// ----------MANAGE LOGIC VARIABLES
	// State in which the logical machine is
	LogicState logicState = LogicState::INITIALIZATION;
	// Event being handled. A nullptr means that is time to get a map event
	Event* handlingEvent = nullptr;
	// Previous position of the player
	int previousGridPosition = 0;

	// If set to "true" skips an update frame
	bool skipUpdate = false;
};

