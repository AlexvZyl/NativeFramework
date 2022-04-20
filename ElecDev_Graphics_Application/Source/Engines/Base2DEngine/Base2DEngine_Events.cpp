//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

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
	if (event.isType(EventType_MouseButtonMiddle))
	{
		getScene().getCamera().translate(pixelDistanceToWorldDistance(event.currentFrameDelta));
	}
}

void Base2DEngine::onMouseScrollEvent(const MouseScrollEvent& event)
{
	getScene().getCamera().incrementZoomAroundCursor2D(event.yOffset, event.mousePosition);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//