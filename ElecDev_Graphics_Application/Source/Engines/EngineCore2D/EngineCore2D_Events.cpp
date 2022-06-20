//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Logger/Logger.h"
#include "Engines/EngineCore2D/EngineCore2D.h"
#include "Application/Events/Events.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

void EngineCore2D::onMouseDragEvent(const MouseDragEvent& event)
{
	if (event.isType(EventType_MouseButtonMiddle) || event.isType(EventType_MouseButtonLeft | EventType_SpaceBar))
	{
		getScene().getCamera().translate(pixelToWorldDistance(event.currentFrameDelta)); 
	}
}

void EngineCore2D::onMouseScrollEvent(const MouseScrollEvent& event)
{
	getScene().getCamera().incrementZoomAroundCursor(event.yOffset, event.mousePosition);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//