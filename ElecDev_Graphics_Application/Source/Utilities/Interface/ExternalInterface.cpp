//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ExternalInterface.h"
#include "Application/Application.h"
#include <iostream>

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

std::string ExternalInterface::LUMEN_INTERFACE_START = "&[LUMEN][START_INTERFACE]";
std::string ExternalInterface::LUMEN_INTERFACE_END = "[INTERFACE_END]&";
std::string ExternalInterface::LUMEN_INTERFACE_FUNCTION_SEPARATOR = "::";
std::vector<ExternalFunctionCall> ExternalInterface::s_commandQueue;

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

void ExternalInterface::parseInputString(std::string& input)
{
	// Seperate the input into the seperate functions.
	std::vector<std::string> functionsStrings = seperateFunctions(input);

	// Check if input is valid.
	if (functionsStrings.size() < 2)
	{
		Lumen::getApp().pushNotification(NotificationType::Warning, 3000, "Received invalid command string.", "Interface");
		return;
	}
	else if (functionsStrings[0] != LUMEN_INTERFACE_START || functionsStrings.back() != LUMEN_INTERFACE_END)
	{
		Lumen::getApp().pushNotification(NotificationType::Warning, 3000, "Received invalid command string.", "Interface");
		return;
	}
	
	// Generate function calls.
	for (auto& command : functionsStrings)
		generateExternalFunctionCall(command);

	// Log success.
	Lumen::getApp().pushNotification(NotificationType::Success, 3000, "Parsed command string.", "Interface");
}

std::vector<std::string> ExternalInterface::seperateFunctions(const std::string& input)
{
	// String containing functions.
	std::vector<std::string> functionsStrings;
	
	// Parse the input.
	int size = input.size();
	std::string inputCopy = input;
	std::string temp;
	while (inputCopy.size())
	{
		// Check for function separator.
		if (inputCopy[0] == ExternalInterface::LUMEN_INTERFACE_FUNCTION_SEPARATOR[0] && 
			inputCopy[1] == ExternalInterface::LUMEN_INTERFACE_FUNCTION_SEPARATOR[1])
		{
			functionsStrings.push_back(temp);
			temp.clear();
			inputCopy.erase(inputCopy.begin(), inputCopy.begin() + 2);
		}
		// End of string.
		else if (inputCopy == "&")
		{
			temp.push_back(inputCopy[0]);
			functionsStrings.push_back(temp);
			temp.clear();
			inputCopy.clear();
		}
		// Add to temp.
		else 
		{
			temp.push_back(inputCopy[0]);
			inputCopy.erase(inputCopy.begin());
		}
	}

	return functionsStrings;
}

void ExternalInterface::generateExternalFunctionCall(const std::string& input)
{

}


void ExternalInterface::pushFunctionCall(std::string& function, std::vector<std::string>& parameters)
{
	s_commandQueue.push_back(ExternalFunctionCall(function, parameters));
}

void ExternalInterface::pushFunctionCall(ExternalFunctionCall& call)
{
	s_commandQueue.push_back(call);
}

void ExternalInterface::clear()
{
	s_commandQueue.clear();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//