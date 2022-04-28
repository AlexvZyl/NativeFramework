#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"
#include "Lumen.h"
#include "Profiler.h"

//==============================================================================================================================================//
//  Timer.																																		//
//==============================================================================================================================================//

Timer::Timer(const char* name)
	: m_name(name)
{
	// Save profiler state on timer start.
	m_profilerActiveOnStart = Lumen::getApp().m_profilerActive;
	if (m_profilerActiveOnStart)
	{
		// Start the timer.
		m_startPoint = std::chrono::high_resolution_clock::now();
	}
}

Timer::~Timer()
{
	stop();
}

// Stop the timer.
void Timer::stop()
{
	// Check if the profiler is still active.
	if (Lumen::getApp().m_profilerActive && m_profilerActiveOnStart)
	{
		// Get end time.
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		// Get microseconds.
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
		float duration = (end - start) * 0.001f;

		// Add result to be displayed.
		Lumen::getApp().m_profilerResults.push_back(ProfileResult(m_name, duration));
	}
	else if(m_profilerActiveOnStart)
	{
		// Add null result.
		Lumen::getApp().m_profilerResults.push_back(ProfileResult(m_name, 0));
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//