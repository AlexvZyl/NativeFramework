#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "Application/LumenWindow/LumenWindow.h"
#include <unordered_map>

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class RendererStats : public LumenWindow
{
public:

	// Constructor.
	RendererStats(std::string name, int windowFlags);

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

private:

	int m_framesPerAverage = 30;
	float m_updateTime = 1.5f;
	int m_currentFrame = 0;
	std::unordered_map<std::string, float> m_prevFrameAverages;
	std::unordered_map<std::string, float> m_currentFrameAverages;
};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//