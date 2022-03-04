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
	//while(true)
	//{
	//	//input = std::cin.get();
	//	std::cout << "\nPassed the input reading.";
	//}
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