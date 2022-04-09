//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Layers/Layer.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Application/Layers/LayerStack.h"
#include <memory>

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

void Layer::setName(const std::string& newName, bool updateStackKey)
{
	// New name with ID.
	std::string newNameWithID = newName + "###LumenLayer" + std::to_string(m_ID);
	
	if (updateStackKey)
	{
		// Get the label of the previous key.
		std::string prevName = getName();
		// Reinsert with a new key.
		LayerStack* ls = Lumen::getApp().m_layerStack.get();
		auto ptr = std::move(ls->getLayers().at(prevName));
		ls->getLayers().erase(prevName);
		ls->getLayers().insert({ newNameWithID, std::move(ptr) });
	}

	// Set children names.
	setNamesOfChildren(newNameWithID);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//