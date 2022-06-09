//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Logger/Logger.h"
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Application/Events/Events.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Lumen.h"
#include "Application/Application.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

void Base2DEngine::onMouseDragEvent(const MouseDragEvent& event)
{
	if (event.isType(EventType_MouseButtonMiddle) || event.isType(EventType_MouseButtonLeft | EventType_SpaceBar))
	{
		getScene().getCamera().translate(pixelToWorldDistance(event.currentFrameDelta)); 
	}
}

void Base2DEngine::onMouseScrollEvent(const MouseScrollEvent& event)
{
	getScene().getCamera().incrementZoomAroundCursor(event.yOffset, event.mousePosition);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//