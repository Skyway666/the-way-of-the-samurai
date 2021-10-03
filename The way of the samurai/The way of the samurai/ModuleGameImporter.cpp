#include "ModuleGameImporter.h"
#include "Application.h"
#include "Third Party/parson.h"

bool ModuleGameImporter::Init()
{
	app->log("Loading game");
	return true;
}
