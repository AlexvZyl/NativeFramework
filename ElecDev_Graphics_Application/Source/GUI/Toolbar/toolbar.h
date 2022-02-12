#pragma once

/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include "GUI/GuiElementCore/GuiElementCore.h"

/*=======================================================================================================================================*/
/* Forward declarations                                                                                                                  */
/*=======================================================================================================================================*/



/*=======================================================================================================================================*/
/* Class.                                                                                                                                */
/*=======================================================================================================================================*/

class Toolbar : public GuiElementCore
{

public:

	// Constructor.
    Toolbar(std::string& name, int windowFlags);

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

	//  Data for the lumen logo.
	float m_texWidth = 0;
	float m_texHeight = 0;
	unsigned m_texID = 0;

};

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/