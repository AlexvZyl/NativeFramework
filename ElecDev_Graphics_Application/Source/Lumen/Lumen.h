#pragma once

//==============================================================================================================================================//
//  Defines.																																	//
//==============================================================================================================================================//

#define TOOLBAR_PADDING 7
#define VERTEX_BUFFER_INCREMENTS 1000
#define VERTEX_BUFFER_REDUCTION_SCALE 1.25
#define MAX_VERTEX_BUFFER_SIZE 10000  // Unused atm.
#define GUI_INDENT_WIDTH 15  // Rather use ImGui::PushStyleVar()?
#define FPS_FRAME_COUNT 60
//#define PROFILE_IMGUI_OVERHEAD
#define LUMEN_PROFILING_FUNCTIONS
#define LUMEN_DETAILED_LOGGING

//==============================================================================================================================================//
//  Macros.																																		//
//==============================================================================================================================================//

#ifdef LUMEN_PROFILING_FUNCTIONS

	#define LUMEN_DRAW_CALL()   Lumen::getApp().getRendererData()->drawCall();
	#define LUMEN_RENDER_PASS() Lumen::getApp().getRendererData()->renderPass();

#else

	#define LUMEN_DRAW_CALL()   
	#define LUMEN_RENDER_PASS() 

#endif

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Application;
class ScriptGui;

//==============================================================================================================================================//
//  Lumen.																																		//
//==============================================================================================================================================//

class Lumen
{

public:

	// Set the singleton Lumen uses.
	static void setApp(Application* app);
	// Get the singleton Lumen is using.
	static Application& getApp();

	// Set the active script GUI.
	static void setAvtiveScriptGui(ScriptGui* gui);
	// Get the active script gui.
	static ScriptGui* getActiveScriptGui();

private:

	// No instantiations.
	inline Lumen() = default;

	// The singleton application.
	inline static Application* s_applicationSingleton = nullptr;

	// The active ScriptGui.  Used for callbacks.
	inline static ScriptGui* s_activeScriptGui = nullptr;

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//