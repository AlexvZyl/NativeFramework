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

void Logger::pushMessage(LoggerMessage& msg)
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
		switch (msg.level)
		{

		// ----------- //
		//  D E B U G  //
		// ----------- //

		case LoggerLevel::Debug:
			if (!msg.title.size()) msg.title = "DEBUG";
			std::cout << white << "[LUMEN] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		// --------- //
		//  I N F O  //
		// --------- //

		case LoggerLevel::Info:
			if (!msg.title.size()) msg.title = "INFO";
			std::cout << blue << "[LUMEN] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		// --------------- //
		//  S U C C E S S  //
		// --------------- //

		case LoggerLevel::Success:
			if (!msg.title.size()) msg.title = "SUCCESS";
			std::cout << green << "[LUMEN] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		// --------------- //
		//  W A R N I N G  //
		// --------------- //

		case LoggerLevel::Warning:
			if (!msg.title.size()) msg.title = "WARN";
			std::cout << yellow << "[LUMEN] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		// ----------- //
		//  E R R O R  //
		// ----------- //

		case LoggerLevel::Error:
			if (!msg.title.size()) msg.title = "ERROR";
			std::cout << red << "[LUMEN] [" << msg.title << "] : " << white << msg.content << "\n";
			app.pushNotification(NotificationType::Error, 10000, msg.content, msg.title);
			break;
		}
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