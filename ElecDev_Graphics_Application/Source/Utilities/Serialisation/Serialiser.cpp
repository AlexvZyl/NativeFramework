//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <Windows.h>
#include "Utilities/Windows/WindowsUtilities.h"
#include "Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Circuit.h"
#include "Graphics/OpenGL/Design2DEngineGL/Design2D_Engine.h"
#include <sys/stat.h>
#include <iostream>
#include <direct.h>
#include <fstream>
#include "CoreGL/FontLoader.h"

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

void saveToYAML(Circuit& circuit, std::string folder)
{
	// ------------- //
	//  F O L D E R  //
	// ------------- //

	std::string exeLoc = getExecutableLocation();
	// Check if folder is provided.
	if (folder.length())	
	{
		struct stat info;
		std::string folderPath = exeLoc + assetsFolder + folder;
		// If folder does not exist, create one.
		if (stat(folderPath.c_str(), &info)) { _mkdir(folderPath.c_str()); }
	}

	// --------------------- //
	//  C O M P O N E N T S  //
	// --------------------- //

	YAML::Emitter yamlEmitter;
	// Begin circuit.
	yamlEmitter << YAML::BeginMap;
	yamlEmitter << YAML::Key << "Circuit" << YAML::Value;

		// Circuit data.
		yamlEmitter << YAML::BeginMap;
		yamlEmitter << YAML::Key << "Label" << YAML::Value << circuit.m_label;
		yamlEmitter << YAML::Key << "Type" << YAML::Value << circuit.m_type;
		yamlEmitter << YAML::EndMap;

	// End circuit.
	yamlEmitter << YAML::EndMap;

	// Begin components.
	yamlEmitter << YAML::BeginMap;
	yamlEmitter << YAML::Key << "Components" << YAML::Value;
	yamlEmitter << YAML::BeginSeq;

	int componentIndex = 0;
	for (std::shared_ptr<Component2D> comp : circuit.m_components)
	{
		//  Begin component.
		yamlEmitter << YAML::BeginMap;
		yamlEmitter << YAML::Key << "Component " + std::to_string(componentIndex) << YAML::Value << YAML::BeginMap;

			// Component data.
			yamlEmitter << YAML::Key << "Centre" << YAML::Value << comp->centre;
			yamlEmitter << YAML::Key << "Component Layer" << YAML::Value << comp->componentLayer;
			yamlEmitter << YAML::Key << "Border layer offset" << YAML::Value << comp->borderLayerOffset;
			yamlEmitter << YAML::Key << "Layer offset" << YAML::Value << comp->portLayerOffset;
			yamlEmitter << YAML::Key << "Number of ports" << YAML::Value << comp->numPorts;
			componentIndex++;
			// Begin ports.
			yamlEmitter << YAML::Key << "Ports" << YAML::Value << YAML::BeginSeq;
		
			// ------------ //
			// East ports.  //
			// ------------ //
			yamlEmitter << YAML::BeginMap;
			yamlEmitter << YAML::Key << "East Ports" << YAML::Value << YAML::BeginSeq;
			int eastPortsIndex = 0;
			for (std::shared_ptr<Port> port : comp->portsEast)
			{
				// Begin Port.
				yamlEmitter << YAML::BeginMap;
				yamlEmitter << YAML::Key << "East Port " + std::to_string(eastPortsIndex) << YAML::Value;
				eastPortsIndex++;
					// Port data.
					yamlEmitter << YAML::BeginMap;
					yamlEmitter << YAML::Key << "Label" << YAML::Value << port->m_label;
					yamlEmitter << YAML::Key << "Centre" << YAML::Value << port->centre;
					yamlEmitter << YAML::Key << "Layer" << YAML::Value << port->portLayer;
					yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->m_offset;
					yamlEmitter << YAML::Key << "Type" << YAML::Value << port->m_type;
					// Begin border.
					yamlEmitter << YAML::Key << "Border" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->border.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->border.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->borderColour;
					// End border.
					yamlEmitter << YAML::EndMap;	
					// Begin body.
					yamlEmitter << YAML::Key << "Body" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->body.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->body.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->bodyColour;
					// End body.
					yamlEmitter << YAML::EndMap;
					// Begin title.
					yamlEmitter << YAML::Key << "Title" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Font" << YAML::Value << port->titleFont.name;
						yamlEmitter << YAML::Key << "Size" << YAML::Value << port->titleSize;
						yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->titleOffset;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->titleColour;
					// End title.
					yamlEmitter << YAML::EndMap;
					// End port.
					yamlEmitter << YAML::EndMap;
				// End port.
				yamlEmitter << YAML::EndMap;
			}
			// End east ports.
			yamlEmitter << YAML::EndSeq << YAML::EndMap;

			// ------------ //
			// West ports.  //
			// ------------ //
			yamlEmitter << YAML::BeginMap;
			yamlEmitter << YAML::Key << "West Ports" << YAML::Value << YAML::BeginSeq;
			int westPortsIndex = 0;
			for (std::shared_ptr<Port> port : comp->portsWest)
			{
				// Begin Port.
				yamlEmitter << YAML::BeginMap;
				yamlEmitter << YAML::Key << "West Port " + std::to_string(westPortsIndex) << YAML::Value;
				westPortsIndex++;
					// Port data.
					yamlEmitter << YAML::BeginMap;
					yamlEmitter << YAML::Key << "Label" << YAML::Value << port->m_label;
					yamlEmitter << YAML::Key << "Centre" << YAML::Value << port->centre;
					yamlEmitter << YAML::Key << "Layer" << YAML::Value << port->portLayer;
					yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->m_offset;
					yamlEmitter << YAML::Key << "Type" << YAML::Value << port->m_type;
					// Begin border.
					yamlEmitter << YAML::Key << "Border" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->border.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->border.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->borderColour;
					yamlEmitter << YAML::EndMap;
					// Begin body.
					yamlEmitter << YAML::Key << "Body" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->body.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->body.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->bodyColour;
					// End body.
					yamlEmitter << YAML::EndMap;
					// Begin title.
					yamlEmitter << YAML::Key << "Title" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Font" << YAML::Value << port->titleFont.name;
						yamlEmitter << YAML::Key << "Size" << YAML::Value << port->titleSize;
						yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->titleOffset;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->titleColour;
					// End title.
					yamlEmitter << YAML::EndMap;
					// End port.
					yamlEmitter << YAML::EndMap;
				// End port.
				yamlEmitter << YAML::EndMap;
			}
			// End west ports.
			yamlEmitter << YAML::EndSeq << YAML::EndMap;

			// ------------- //
			// North ports.  //
			// ------------- //
			yamlEmitter << YAML::BeginMap;
			yamlEmitter << YAML::Key << "North Ports" << YAML::Value << YAML::BeginSeq;
			int northPortsIndex = 0;
			for (std::shared_ptr<Port> port : comp->portsNorth)
			{
				// Begin Port.
				yamlEmitter << YAML::BeginMap;
				yamlEmitter << YAML::Key << "North Port " + std::to_string(northPortsIndex) << YAML::Value;
				northPortsIndex++;
					// Port data.
					yamlEmitter << YAML::BeginMap;
					yamlEmitter << YAML::Key << "Label" << YAML::Value << port->m_label;
					yamlEmitter << YAML::Key << "Centre" << YAML::Value << port->centre;
					yamlEmitter << YAML::Key << "Layer" << YAML::Value << port->portLayer;
					yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->m_offset;
					yamlEmitter << YAML::Key << "Type" << YAML::Value << port->m_type;
					// Begin border.
					yamlEmitter << YAML::Key << "Border" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->border.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->border.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->borderColour;
					// End border.
					yamlEmitter << YAML::EndMap;
					// Begin body.
					yamlEmitter << YAML::Key << "Body" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->body.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->body.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->bodyColour;
					// End body.
					yamlEmitter << YAML::EndMap;
					// Begin title.
					yamlEmitter << YAML::Key << "Title" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Font" << YAML::Value << port->titleFont.name;
						yamlEmitter << YAML::Key << "Size" << YAML::Value << port->titleSize;
						yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->titleOffset;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->titleColour;
					// End title.
					yamlEmitter << YAML::EndMap;
					// End port.
					yamlEmitter << YAML::EndMap;
				// End port.
				yamlEmitter << YAML::EndMap;
			}
			// End north ports.
			yamlEmitter << YAML::EndSeq << YAML::EndMap;

			// ------------- //
			// South ports.  //
			// ------------- //
			yamlEmitter << YAML::BeginMap;
			yamlEmitter << YAML::Key << "South Ports" << YAML::Value << YAML::BeginSeq;
			int southPortsIndex = 0;
			for (std::shared_ptr<Port> port : comp->portsSouth)
			{
				// Begin Port.
				yamlEmitter << YAML::BeginMap;
				yamlEmitter << YAML::Key << "South Port " + std::to_string(southPortsIndex) << YAML::Value;
				southPortsIndex++;
					// Port data.
					yamlEmitter << YAML::BeginMap;
					yamlEmitter << YAML::Key << "Label" << YAML::Value << port->m_label;
					yamlEmitter << YAML::Key << "Centre" << YAML::Value << port->centre;
					yamlEmitter << YAML::Key << "Layer" << YAML::Value << port->portLayer;
					yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->m_offset;
					yamlEmitter << YAML::Key << "Type" << YAML::Value << port->m_type;
					// Begin border.
					yamlEmitter << YAML::Key << "Border" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->border.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->border.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->borderColour;
					// End border.
					yamlEmitter << YAML::EndMap;
					// Begin body.
					yamlEmitter << YAML::Key << "Body" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Thickness" << YAML::Value << port->body.m_thickness;
						yamlEmitter << YAML::Key << "Fade" << YAML::Value << port->body.m_fade;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->bodyColour;
					// End body.
					yamlEmitter << YAML::EndMap;
					// Begin title.
					yamlEmitter << YAML::Key << "Title" << YAML::Value;
					yamlEmitter << YAML::BeginMap;
						yamlEmitter << YAML::Key << "Font" << YAML::Value << port->titleFont.name;
						yamlEmitter << YAML::Key << "Size" << YAML::Value << port->titleSize;
						yamlEmitter << YAML::Key << "Offset" << YAML::Value << port->titleOffset;
						yamlEmitter << YAML::Key << "Colour" << YAML::Value << port->titleColour;
					// End title.
					yamlEmitter << YAML::EndMap;
					// End port.
					yamlEmitter << YAML::EndMap;
				// End port.
				yamlEmitter << YAML::EndMap;
			}
			// End south ports.
			yamlEmitter << YAML::EndSeq << YAML::EndMap;
		
			// End ports.
			yamlEmitter << YAML::EndSeq;

		// End component.
		yamlEmitter << YAML::EndMap << YAML::EndMap;	
	}	
	// End components.
	yamlEmitter << YAML::EndSeq << YAML::EndMap;

	// ------------- //
	//  C A B L E S  //
	// ------------- //

	// Begin cables.
	yamlEmitter << YAML::BeginMap;
	yamlEmitter << YAML::Key << "Cables" << YAML::BeginSeq;
	for (std::shared_ptr<Cable> comp : circuit.m_cables)
	{

	}
	// End cables.
	yamlEmitter << YAML::EndSeq << YAML::EndMap;

	// --------- //
	//  F I L E  //
	// --------- //

	const std::string yamlLocation = exeLoc + assetsFolder + folder + "\\" + circuit.m_label + ".lmn";
	std::ofstream yamlStream;
	yamlStream.open(yamlLocation);
	yamlStream << yamlEmitter.c_str();
}

void saveToBIN(Circuit& circuit, std::string folder)
{

}

//=============================================================================================================================================//
//  Deserialisation.																														   //
//=============================================================================================================================================//

void loadFromYAML(Design2DEngineGL& engine, std::string file)
{

}

void loadFromBIN(Design2DEngineGL& engine, std::string file)
{

}

//=============================================================================================================================================//
//  Utilities.																																   //
//=============================================================================================================================================//

inline YAML::Emitter& operator<<(YAML::Emitter& emitter, PortType type)
{
	std::string typeStr;
	if		(type == PortType::PORT_IN)		typeStr = "PORT_IN";
	else if (type == PortType::PORT_OUT)	typeStr = "PORT_OUT";
	else if (type == PortType::PORT_INOUT)	typeStr = "PORT_INOUT";
	return emitter << typeStr;
}

inline YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec2 vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << YAML::EndSeq;
}

inline YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3 vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << vec[3] << YAML::EndSeq;
}

inline YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec4 vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << vec[2] << vec[3] << YAML::EndSeq;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//