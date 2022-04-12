//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Utilities/Serialisation/Serialiser.h"
#include "OpenGL/Primitives/Text.h"
#include "Graphics/Fonts/FontLoader.h"
#include  "OpenGL/Renderer/RendererGL.h"

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Text* text) 
{
	// Begin text data.
	emitter << YAML::BeginMap;

	// Data.
	emitter << YAML::Key << "String" << YAML::Value << text->m_string;
	emitter << YAML::Key << "Position" << YAML::Value << text->m_trackedCenter;
	emitter << YAML::Key << "Color" << YAML::Value << text->m_colour;
	emitter << YAML::Key << "Scale" << YAML::Value << text->m_textScale;
	emitter << YAML::Key << "Font" << YAML::Value << text->m_font->name;
	emitter << YAML::Key << "Horizontal Alignment" << YAML::Value << text->m_horizontalAlign;
	emitter << YAML::Key << "Vertical Alignment" << YAML::Value << text->m_verticalAlign;
	emitter << YAML::Key << "Box Color" << YAML::Value << text->m_boxColor;

	// End text data.
	emitter << YAML::EndMap;
	return emitter;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<Text>& text) 
{
	return emitter << text.get();
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Text>& text) 
{
	return emitter << text.get();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//