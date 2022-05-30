#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Profiler.h"
#include <iostream>

//==============================================================================================================================================//
//  Timer.																																		//
//==============================================================================================================================================//

Timer::Timer(const char* name)
	: m_name(name)
{
    // Start the timer.
    m_startPoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	stop();
}

// Stop the timer.
void Timer::stop()
{
    // Get end time.
    auto endTimePoint = std::chrono::high_resolution_clock::now();
    // Get microseconds.
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
    float duration = (end - start) * 0.001f;
    std::cout << "[" << m_name << "] : [" << std::to_string(duration) << " ms] \n";
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//