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
		case LoggerLevel::Debug:
			if (!msg.title.size()) msg.title = "Debug";
			std::cout << white << "[Lumen] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		case LoggerLevel::Info:
			if (!msg.title.size()) msg.title = "Info";
			std::cout << blue << "[Lumen] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		case LoggerLevel::Success:
			if (!msg.title.size()) msg.title = "Success";
			std::cout << green << "[Lumen] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		case LoggerLevel::Warning:
			if (!msg.title.size()) msg.title = "Warning";
			std::cout << yellow << "[Lumen] [" << msg.title << "] : " << white << msg.content << "\n";
			break;

		case LoggerLevel::Error:
			if (!msg.title.size()) msg.title = "Error";
			std::cout << red << "[Lumen] [" << msg.title << "] : " << white << msg.content << "\n";
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