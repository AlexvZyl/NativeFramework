#pragma once

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
	inline virtual void dispatchEvents() = 0; 

	// Set the name of all the elements in the layer.
	inline virtual void setName(std::string& newName) = 0;

	// Get the name of the layer.
	inline virtual std::string& getName() = 0;

	// Checks if the layer is hovered.
	inline virtual bool isHovered() = 0;

protected:

	// Protected constructor.
	inline Layer() = default;	
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
