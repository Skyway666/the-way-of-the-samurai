#include "Module.h"
#include "Application.h"

void Module::TerminateApplication(const char* message) const
{
    app->Terminate(message, this);
}
