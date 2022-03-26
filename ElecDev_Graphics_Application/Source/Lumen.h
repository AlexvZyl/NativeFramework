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

//==============================================================================================================================================//
//  Macros.																																		//
//==============================================================================================================================================//

#ifdef LUMEN_PROFILING_FUNCTIONS

	#define LUMEN_DRAW_CALL()   Lumen::getApp().m_rendererData.drawCall();
	#define LUMEN_RENDER_PASS() Lumen::getApp().m_rendererData.renderPass();

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


//float pi = 3.1415926538;
//
//// Get vertex data.
//vec3 pos0 = vec3(gl_in[0].gl_Position);
//vec3 pos1 = vec3(gl_in[1].gl_Position);
//vec3 pos2 = vec3(gl_in[2].gl_Position);
//
//// Calculate the normalised edge vectors.
//vec3 vec01 = pos1 - pos0;
//vec3 vec12 = pos2 - pos1;
//vec3 vec20 = pos0 - pos2;
//
//// Calculate normalised directional vector.
//
//vec3 normVec0 = vec01 + vec20;
//normVec0 /= length(normVec0) * -1;
//
//vec3 normVec1 = vec01 + vec12;
//normVec1 /= length(normVec1) * -1;
//
//vec3 normVec2 = vec12 + vec20;
//normVec2 /= length(normVec2) * -1;
//
//// Manipulate each vector to create outline.
//
//gl_Position = vec4(vec3(gl_in[0].gl_Position) + normVec0 * data_in[0].outline / 100, 1.0f);
//data_out[0].outline = data_in[0].outline;
//data_out[0].entityID = data_in[0].entityID;
//EmitVertex();
//
//gl_Position = vec4(vec3(gl_in[1].gl_Position) + normVec1 * data_in[1].outline / 100, 1.0f);
//data_out[1].outline = data_in[1].outline;
//data_out[1].entityID = data_in[1].entityID;
//EmitVertex();
//
//gl_Position = vec4(vec3(gl_in[2].gl_Position) + normVec2 * data_in[2].outline / 100, 1.0f);
//data_out[2].outline = data_in[2].outline;
//data_out[2].entityID = data_in[2].entityID;
//EmitVertex();
//
//EndPrimitive();