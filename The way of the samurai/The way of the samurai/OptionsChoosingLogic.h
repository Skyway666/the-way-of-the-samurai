#pragma once
#include "LogicProcessor.h"
#include <vector>
#include <list>
class OptionsChoosingLogic : public LogicProcessor
{
private:
    // Reads user input to select correct option
    LogicProcessorResult Step(string input) override;
    void StartOptionChoosing();

    // References:
    void (*displayList)(const vector<string>&) = nullptr;
    list<string>* conditions = nullptr;
    list<string>* objects = nullptr;

    // Parameters:
    vector<string> options;
    string optionsMenuIntroductionText;
    string availableObjectsText;
    string currentConditionsText;
    string invalidOptionText;

    friend class ModuleGameLogic;
};

