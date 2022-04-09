#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <string>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Event;

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

class Layer
{
public:

	// Pass an event to the layer.
	inline virtual void onEvent(Event& event) = 0;

	// Render the layer.
	inline virtual void onRender() = 0;

	// Dispatch the ImGUI events, if there are elements
	// in the layer that has to do this.
	inline virtual void onUpdate() = 0; 

	// Set the name of the layer as well as change the key in the layerstack.
	void setName(const std::string& newName, bool updateStackKey = false);

	// Get the name of the layer.
	inline virtual std::string getName() = 0;

	// Checks if the layer is hovered.`
	inline virtual bool isHovered() = 0;

	// Focus the layer.
	inline virtual void focus() = 0;

	// Destructor.
	inline virtual ~Layer() = default;

	// Set the layer ID.
	// This also has to rename it.
	inline void setID(unsigned ID, bool updateStackKey = false)
	{ 
		m_ID = ID;
		setName(getName(), updateStackKey);
	}

protected:

	// Sets the names of all of the layers children.
	inline virtual void setNamesOfChildren(std::string newName) = 0;

	// Protected constructor.
	inline Layer() = default;	
	
	// The ID given to the layer for ImGui rendering.
	unsigned m_ID = 0;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//