//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Lumen.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Lumen Namespace implementations.																											//
//==============================================================================================================================================//

void Lumen::setApp(Application* app) 
{
	s_applicationSingleton = app;
}

void Lumen::setAvtiveScriptGui(ScriptGui* gui) 
{
	s_activeScriptGui = gui;
}

ScriptGui* Lumen::getActiveScriptGui() 
{
	return s_activeScriptGui;
}

Application& Lumen::getApp() 
{
	return *s_applicationSingleton;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//