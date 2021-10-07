#pragma once
#include "Module.h"
#include <list>
#include <string>
using namespace std;

// Forward declaration
typedef struct json_object_t JSON_Object;
struct MapEvent;
struct Event;



class ModuleGameLogic : public Module
{
// Public structures
public:
	struct GameData
	{
		// Key variables that define the state of the game
		list<string> conditions;
		// Player objects that can be consumed
		list<string> objects;
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
		SAVING_VARIABLE
	};

	// Possible outcomes of loading an event
	enum class LoadEventResult
	{
		FATAL_ERROR,
		EVENT_LOADED,
		EVENT_ENDED,
		INVALID_INPUT
	};

// Public methods
public:
	// Module inherited methods
	bool Init() override;
	bool Start() override;
	bool Update() override;

// Private methods
private:
	// Handles an event
	void HandleCurrentEvent();
	// Load map event
	LoadEventResult LoadEvent();

	// Returns current map event depending in the currentGridPosition
	MapEvent* GetCurrentMapEvent() const;
	// Use input to update grid position. Returns false if the input was invalid
	bool UpdateGridPosition();
	// Sets the logic state to "NAVIGATING_MAP" and notifies the player
	void BackToMap();
	// Sets logic state to "SAVING_VARIABLE" and assigns variableSaving.
	void SaveVariable(string newVariableSaving);

// Private attributes
private:
	// The game state
	GameData gameState;

	// Variables to manage logic
	// Event being handled. A nullptr means that is time to get a map event
	Event* handlingEvent = nullptr;
	// Previous position of the player
	int previousGridPosition = 0;
	// Variable to save in the next frame
	string variableSaving;

	// State in which the logical machine is
	LogicState logicState = LogicState::INITIALIZATION;
};

