#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Base engine.
#include "../BaseEngine/baseEngineCore.h"

// General.
#include <map>
#include "../stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  MCC Engine Struct.
//----------------------------------------------------------------------------------------------------------------------

struct MccStruct {

	ImVec2 viewportDimentions;
	bool isHovered = false;
	BaseEngineGL* engine;
	ImVec2 mouseCoords;
	bool resizeEvent=false;

	// Constructor.
	MccStruct(stateMachine* states) 
	{
		engine = new BaseEngineGL(states);
		viewportDimentions[0] = engine->m_imGuiViewportDimensions[0];
		viewportDimentions[1] = engine->m_imGuiViewportDimensions[1];
	}

	// Destructor.
	~MccStruct() 
	{
		delete engine;
	}
};

//----------------------------------------------------------------------------------------------------------------------
//  Class.
//----------------------------------------------------------------------------------------------------------------------
 
// MCC Drawing engine that inherets from the base engine.
class MccEngineGL {

public:

	//---------------------------------------------------------------------------------------
	// Members.
	//---------------------------------------------------------------------------------------

	// Dictionary containg all of the MCC drawing instances.
	std::map<std::string, MccStruct*> m_mccDictionary;
	// States.
	stateMachine* m_states;
	// Keep track of the active MCC.
	std::string m_activeMCC;

	//---------------------------------------------------------------------------------------
	// Contructor.
	//---------------------------------------------------------------------------------------

	// Constructor.
	MccEngineGL(stateMachine* states);
	// Destructor.
	~MccEngineGL();

	//---------------------------------------------------------------------------------------
	// Methods.
	//---------------------------------------------------------------------------------------
	// 
	// Render loop.
	void renderActiveEngine();
	// Checks if the given name is in the list of MCC's.
	bool isNameInDictionary(std::string mccName);

	//---------------------------------------------------------------------------------------
	// Events.
	//---------------------------------------------------------------------------------------

	// Resize event for the engine.
	void resizeEvent(int width, int height);
	void mouseMoveEvent(int buttonState);
	void mousePressEvent(int button, int action);
	void mouseScrollEvent(float yOffset);

	//---------------------------------------------------------------------------------------
	// API.
	//---------------------------------------------------------------------------------------

	// Functions supported by the base engine.  Added functionality to choose MCC.
	void drawLine(std::string mccName, float position1[2], float position2[2], float color[4]);
	void drawTriangleClear(std::string mccName, float position1[2], float position2[2], float position3[2], float color[4]);
	void drawTriangleFilled(std::string mccName, float position1[2], float position2[2], float position3[2], float color[4]);
	void drawQuadClear(std::string mccName, float position[2], float width, float height, float color[4]);
	void drawQuadFilled(std::string mccName, float position[2], float width, float height, float color[4]);
	void drawCircleClear(std::string mccName, float position[2], float radius, float color[4]);
	void drawCircleFilled(std::string mccName, float position[2], float radius, float color[4]);
	void drawText(std::string mccName, std::string text, float coords[2], float color[4], float scale);
	// Adds an MCC to the dict.
	void addMCC(std::string mccName);
	// Removes an MCC from the dict.
	void removeMCC(std::string mccName);
	void display();

	//---------------------------------------------------------------------------------------

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------