//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Logger/Logger.h"
#include "External/Misc/ConsoleColor.h"
#include "Lumen.h"
#include "Application/Application.h"
#include <sstream>

//==============================================================================================================================================//
//  Logger Functions.																															//
//==============================================================================================================================================//

void Logger::pushMessage(const LoggerMessage& msg)
{
	s_messageQueue.push_back(msg);
}

void Logger::flushQueue()
{
	// Return if nothing to log.
	if (!s_messageQueue.size())return;

	Application& app = Lumen::getApp();

	// Log all of the messages.
	for (auto& msg : s_messageQueue)
	{
		// Default color.
		auto color = white;

		switch (msg.level)
		{
		case LoggerLevel::Debug:
			if (!msg.title.size()) msg.title = "Debug";
			break;

		case LoggerLevel::Info:
			if (!msg.title.size()) msg.title = "Info";
			color = blue;
			break;

		case LoggerLevel::Success:
			if (!msg.title.size()) msg.title = "Success";
			color = green;
			break;

		case LoggerLevel::Warning:
			if (!msg.title.size()) msg.title = "Warning";
			color = yellow;
			break;

		case LoggerLevel::Error:
			if (!msg.title.size()) msg.title = "Error";
			color = red;
			app.pushNotification(NotificationType::Error, 10000, msg.content, msg.title);
			break;
		}

		std::cout << color << "[Lumen] [" << msg.title << "] ";
#ifdef LUMEN_DETAILED_LOGGING
		std::cout << "[" << msg.function << "] ";
#endif
		std::cout << ": " << white << msg.content << "\n";
	}

	std::cout.flush();
	// Clear queue.
	Logger::clear();
}

void Logger::clear()
{
	s_messageQueue.clear();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//