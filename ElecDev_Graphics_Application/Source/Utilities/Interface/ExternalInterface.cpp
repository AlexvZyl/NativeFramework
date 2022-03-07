//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ExternalInterface.h"
#include <iostream>

//==============================================================================================================================================//
//  External Interface.																															//
//==============================================================================================================================================//

std::vector<ExternalFunctionCall> ExternalInterface::s_commandQueue;

void ExternalInterface::inputThread()
{
	std::string input;
	while(true)
	{
		// Read input.
		std::getline(std::cin, input);

		// Start parsing.
		if (input == "&[LUMEN][START_INTERFACE]")
		{
			std::cout << "Starting read of input commands.\n";


			std::cout << "Stopped read of input commands.\n.";
			input.clear();
		}
		else 
		{
			std::cout << "Read input, not responding.\n";
			input.clear();
		}
	}
}

void ExternalInterface::parseInputString(std::string& input)
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