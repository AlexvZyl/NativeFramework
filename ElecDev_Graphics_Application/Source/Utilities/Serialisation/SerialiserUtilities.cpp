//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Port.h"
#include "Serialiser.h"

//=============================================================================================================================================//
//  Port serialisation.																														   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, PortType type)
{
	std::string typeStr;
	if		(type == PortType::PORT_IN)		typeStr = "PORT_IN";
	else if (type == PortType::PORT_OUT)	typeStr = "PORT_OUT";
	else if (type == PortType::PORT_INOUT)	typeStr = "PORT_INOUT";
	return emitter << typeStr;
}

//=============================================================================================================================================//
//  GLM serialisation.																														   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec2 vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << YAML::EndSeq;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3 vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << vec[3] << YAML::EndSeq;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec4 vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << vec[2] << vec[3] << YAML::EndSeq;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//