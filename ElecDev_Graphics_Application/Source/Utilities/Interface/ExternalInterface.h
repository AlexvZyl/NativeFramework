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

	// Seperates all of the functions inside the input.
	// Functions should be seperated by LUMEN_INTERFACE_FUNCTION_SEPARATOR.
	static std::vector<std::string> seperateFunctions(const std::string& input);

	// Create a ExternalFunctionCall from the string and add it to the queue.
	static void generateExternalFunctionCall(const std::string& input);

	// Parsing data.
	static std::string LUMEN_INTERFACE_START;
	static std::string LUMEN_INTERFACE_END;
	static std::string LUMEN_INTERFACE_FUNCTION_SEPARATOR;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//