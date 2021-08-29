//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

//  General.
#include <string>
#include <iostream>

#include "guiHandler.h"
#include "Ribbons/ribbons.h"
#include "Toolbar/toolbar.h"
#include "GraphicsWindow/graphicsWindow.h"

// Constructor.
GUIHandler::GUIHandler()
{

};

void GUIHandler::renderGraphics()
{
	this->toolbar.renderToolbar();
	this->ribbons.renderRibbons();
	//this->graphicsWindow.renderGraphics();
};
