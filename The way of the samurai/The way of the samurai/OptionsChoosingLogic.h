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
    void (*displayList)(const vector<string>&);
    list<string>* conditions;
    list<string>* objects;

    // Parameters:
    vector<string> options;
    string optionsMenuIntroductionText;
    string avaliableObjectsText;
    string currentConditionsText;
    string invalidOptionText;

    friend class ModuleGameLogic;
};

