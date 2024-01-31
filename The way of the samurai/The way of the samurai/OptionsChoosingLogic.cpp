#include "OptionsChoosingLogic.h"

LogicProcessorResult OptionsChoosingLogic::Step(string input)
{
    LogicProcessorResult ret = LogicProcessorResult::NONE;

    // Process input and options
    processGameplayText(input);
    vector<string> processedOptions;
    for (int i = 0; i < options.size(); i++)
    {
        string option = options[i];
        processGameplayText(option);
        processedOptions.push_back(option);
    }

    // Compute option index of the input
    vector<string>::iterator selectedOptionIt = std::find(processedOptions.begin(), processedOptions.end(), input);
    int optionIndex = -1;
    if (selectedOptionIt != processedOptions.end())
        optionIndex = selectedOptionIt - processedOptions.begin();

    // Execute seleted option
    switch (optionIndex)
    {
        // Objects
        case 0:
        {
            log(availableObjectsText);
            vector<string> displayVector(objects->begin(), objects->end());
            displayList(displayVector);
            ret = LogicProcessorResult::OPTIONS_CHOOSING_CHOOSING;
            break;
        }
        // Conditions
        case 1:
        {
            log(currentConditionsText);
            vector<string> displayVector(conditions->begin(), conditions->end());
            displayList(displayVector);
            ret = LogicProcessorResult::OPTIONS_CHOOSING_CHOOSING;
            break;
        }
        // Help
        case 2:
        {
            // TODO: Develop help message
            log("Help message still not avaliable");
            ret = LogicProcessorResult::OPTIONS_CHOOSING_CHOOSING;
            break;
        }
        // Language
        case 3:
        {
            ret = LogicProcessorResult::OPTIONS_CHOOSING_CHOOSE_LANGUAGE;
            break;
        }
        // Tutorial
        case 4:
        {
            // TODO: Develop tutorial
            log("Tutorial still not avaliable");
            ret = LogicProcessorResult::OPTIONS_CHOOSING_CHOOSING;
            break;
        }
        // Resume
        case 5:
        {
            ret = LogicProcessorResult::OPTIONS_CHOOSING_RESUME;
            break;
        }
        // Exit
        case 6:
        {
            ret = LogicProcessorResult::OPTIONS_CHOOSING_EXIT;
            break;
        }

        // Invalid option
        case -1:
        {
            log(invalidOptionText);
            ret = LogicProcessorResult::OPTIONS_CHOOSING_CHOOSING;
            break;
        }
    }


    return ret;
}

void OptionsChoosingLogic::StartOptionChoosing()
{
    log(optionsMenuIntroductionText);
    displayList(options);
}