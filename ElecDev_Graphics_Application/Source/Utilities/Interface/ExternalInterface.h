#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <unordered_map>
#include <string>
#include <vector>

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

// Contains the information regarding the API call.
struct ExternalFunctionCall 
{
	// Constructor.
	ExternalFunctionCall(std::string& function, std::vector<std::string>& parameters)
		: function(function), parameters(parameters)
	{}

	// Data.
	std::string function;
	std::vector<std::string> parameters;
};

//==============================================================================================================================================//
//  Class.																																		//
//==============================================================================================================================================//

// Class that handles external API calls to Lumen.
class ExternalInterface 
{
public:

	// Function that reads input from the pipe.
	static void inputThread();

	// Parse a input string and create a function call.
	static void parseInputString(std::string& input);

	// Construct a call.
	static void pushFunctionCall(std::string& function, std::vector<std::string>& parameters);
	// Copy a call.
	static void pushFunctionCall(ExternalFunctionCall& call);

	// Clear the interface.
	static void clear();
	
private:

	// Commands to be run.
	static std::vector<ExternalFunctionCall> s_commandQueue;

	// Private Constructor.
	inline ExternalInterface() = default;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//