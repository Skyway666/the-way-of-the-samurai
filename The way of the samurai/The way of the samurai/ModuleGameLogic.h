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

	enum LogicState
	{
		Initialization,
		NavigatingMap,
		InEvent
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
	bool LoadEvent();

	// Returns current map event depending in the currentGridPosition
	MapEvent* GetCurrentMapEvent() const;
	// Use input to update grid position
	void UpdateGridPosition();

// Private attributes
private:
	// The game state
	GameData gameState;

	// Event being handled. A nullptr means that is time to get a map event
	Event* handlingEvent = nullptr;

	// State in which the logical machine is
	LogicState logicState = LogicState::Initialization;
};

