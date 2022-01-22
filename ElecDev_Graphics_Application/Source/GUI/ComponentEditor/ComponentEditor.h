#pragma once
/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "Graphics/graphicsHandler.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"

struct GUIState;

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

class ComponentEditor
{
private:

	GUIState* m_guiState;
	GraphicsHandler* m_graphicsHandler;
	bool addingPort = false;
	std::string newName = "Untitled";
	int newType = 2;
	int newPos = 0;

public:

	std::string m_windowContext;
	bool m_contextSaved = false;

	// Constructor.
	ComponentEditor(GUIState* guiState, GraphicsHandler* graphicsHandler);
	// Render call.
	void render();

};

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/