//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Graphics/Fonts/FontLoader.h"
#include "OpenGL/Primitives/PolyLine.h"

//=============================================================================================================================================//
//  Component 2D serialiser.   																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Component2D* comp) 
{
	// Begin component data.
	emitter << YAML::BeginMap;

	// Component data.
	emitter << YAML::Key << "Filename" << YAML::Value << comp->equipType + ".lmcp";
	emitter << YAML::Key << "Internal Circuit" << YAML::Value << "Test_AE_234.lmct";
	emitter << YAML::Key << "Equipment Type" << YAML::Value << comp->equipType;
	emitter << YAML::Key << "Border Layer Offset" << YAML::Value << comp->borderLayerOffset;
	emitter << YAML::Key << "Dictionary" << YAML::Value << comp->dataDict;

	// Title.
	emitter << YAML::Key << "Title" << YAML::Value << comp->title;
	
	// Ports.
	emitter << YAML::Key << "Ports" << YAML::Value << comp->ports;

	// Polygons.
	emitter << YAML::Key << "Polygons" << YAML::Value;
	emitter << YAML::BeginMap;
	int index = 0;
	for (auto& poly : comp->m_polygons)
	{
		PolyLine* polyline = dynamic_cast<PolyLine*>(poly);
		if (polyline)
		{
			emitter << YAML::Key << "Polyline " + std::to_string(index) << YAML::Value << polyline;
		}
		else 
		{
			emitter << YAML::Key << "Polygon " + std::to_string(index) << YAML::Value << poly;
		}
		index++;
	}
	emitter << YAML::EndMap;

	// Line Segments.
	emitter << YAML::Key << "Line Segments" << YAML::Value;
	emitter << YAML::BeginMap;
	index = 0;
	for (auto& line : comp->m_lines)
	{
		emitter << YAML::Key << "Line Segment " + std::to_string(index) << YAML::Value << line;
		index++;
	}
	emitter << YAML::EndMap;

	// Text.
	emitter << YAML::Key << "Text" << YAML::Value;
	emitter << YAML::BeginMap;
	index = 0;
	for (auto& text : comp->m_text)
	{
		emitter << YAML::Key << "Text " + std::to_string(index) << YAML::Value << text;
		index++;
	}
	emitter << YAML::EndMap;

	// Circles.
	emitter << YAML::Key << "Circles" << YAML::Value;
	emitter << YAML::BeginMap;
	index = 0;
	for (auto& circle : comp->m_circles)
	{
		emitter << YAML::Key << "Circle " + std::to_string(index) << YAML::Value << circle;
		index++;
	}
	emitter << YAML::EndMap;

	// End componenet data.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Component2D Vector serialiser.   																										   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Component2D>>& compVector) 
{
	// Begin.
	emitter << YAML::BeginMap;
	int componentIndex = 0;
	for (std::shared_ptr<Component2D> comp : compVector)
	{
		// Begin component.
		emitter << YAML::Key << "Component " + std::to_string(componentIndex) << YAML::Value << comp.get();
		componentIndex++;
	}
	// Done.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
