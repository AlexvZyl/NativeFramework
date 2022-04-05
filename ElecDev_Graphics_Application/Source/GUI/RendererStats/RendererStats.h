#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include <unordered_map>

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class RendererStats : public GuiElementCore
{
public:

	// Constructor.
	RendererStats(std::string name, int windowFlags);

	// Rendering functions.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

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