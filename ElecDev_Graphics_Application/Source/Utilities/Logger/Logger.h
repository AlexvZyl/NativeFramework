#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Lumen.h"

//=============================================================================================================================================//
//  Data.																																	   //
//=============================================================================================================================================//

enum class LoggerLevel 
{
	Debug,
	Info,
	Success,
	Warning,
	Error
};

struct LoggerMessage 
{
	std::string content = "";
	std::string title = "";
	std::string function = "";
	LoggerLevel level = LoggerLevel::Info;
};

//=============================================================================================================================================//
//  Macros.																																	   //
//=============================================================================================================================================//

//  NOTE: The title string can be an empty string, the default title will then be used.
//		  Errors are also pushed as notifications.

// ----------- //
//  D E B U G  //
// ----------- //	

#ifdef _DEBUG										
#define LUMEN_LOG_DEBUG(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.function = __FUNCTION__;		\
	loggerMessage.level = LoggerLevel::Debug;	\
	Logger::pushMessage(loggerMessage);			\
}		
#else
#define LUMEN_LOG_DEBUG(contentStr, titleStr)	
#endif

// --------- //
//  I N F O  //
// --------- //

#define LUMEN_LOG_INFO(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.function = __FUNCTION__;		\
	loggerMessage.level = LoggerLevel::Info;	\
	Logger::pushMessage(loggerMessage);			\
}

// --------------- //
//  S U C C E S S  //
// --------------- //

#define LUMEN_LOG_SUCCESS(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.function = __FUNCTION__;		\
	loggerMessage.level = LoggerLevel::Success;	\
	Logger::pushMessage(loggerMessage);			\
}

// --------------- //
//  W A R N I N G  //
// --------------- //	

#define LUMEN_LOG_WARN(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.function = __FUNCTION__;		\
	loggerMessage.level = LoggerLevel::Warning;	\
	Logger::pushMessage(loggerMessage);			\
}

// ----------- //
//  E R R O R  //
// ----------- //	

#define LUMEN_LOG_ERROR(contentStr, titleStr)	\
{												\
	LoggerMessage loggerMessage;				\
	loggerMessage.content = contentStr;			\
	loggerMessage.title = titleStr;				\
	loggerMessage.function = __FUNCTION__;		\
	loggerMessage.level = LoggerLevel::Error;	\
	Logger::pushMessage(loggerMessage);			\
}

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

class Logger 
{
public:

	// Push a message into the queue.
	static void pushMessage(const LoggerMessage& msg);

	// Log all of the messages in the queue and clear it.
	static void flushQueue();

	// Clear all of the messgages in the queue.
	static void clear();

private:

	// Private constructor.
	inline Logger() = default;

	// Queue containing messages.
	inline static std::vector<LoggerMessage> s_messageQueue;

};

//=============================================================================================================================================//
//  Overloads.																																   //
//=============================================================================================================================================//

namespace std 
{
	_NODISCARD inline string to_string(const glm::vec4& vec)
	{
		return "[ " + to_string(vec.x) + " , " + to_string(vec.y) + " , " + to_string(vec.z) + to_string(vec.w) + " ]";
	}

	_NODISCARD inline string to_string(const glm::vec3& vec)
	{
		return "[ " + to_string(vec.x) + " , " + to_string(vec.y) + " , " + to_string(vec.z) + " ]";
	}

	_NODISCARD inline string to_string(const glm::vec2& vec)
	{
		return "[ " + to_string(vec.x) + " , " + to_string(vec.y) + " ]";
	}
}

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//