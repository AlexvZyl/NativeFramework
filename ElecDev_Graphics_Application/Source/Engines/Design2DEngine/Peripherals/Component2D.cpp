//============================================================================================================================================//
// Includes.																																   //
//=============================================================================================================================================//

#include "Component2D.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "Graphics/Fonts/FontLoader.h"
#include "OpenGL/Primitives/Text.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/Primitives/Circle.h"
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Circuit.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/RendererGL.h"

//=============================================================================================================================================//
//  Variables.																																   //
//=============================================================================================================================================//

//Add font for component titles
Font Component2D::titleFont = msdfLoadFont(ARIAL_NORMAL_JSON);
//Font Component2D::titleFont = msdfLoadFont(ARIAL_BOLD_MSDF_JSON);
unsigned Component2D::componentID = 0;

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Component2D::Component2D(Circuit* parent)
	: Entity(EntityType::COMPONENT, parent)
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Create vertices.
	centre = glm::vec2(0.0f, 0.0f);
	vertices.reserve(4);
	vertices.emplace_back(glm::vec3(centre.x - width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y + height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x - width, centre.y + height, 0.0f));

	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	// Main shape.
	shape = Renderer::addPolygon2D(vertices, this);
	shape->setColor(shapeColour);
	shape->setLayer(0.001f);//temp fix
	// Component border.
	border = Renderer::addPolygon2DClear(vertices, this);
	border->setColor(borderColour);
	border->setLayer(componentLayer + borderLayerOffset);
	// Component title.
	glm::vec3 titlePos = glm::vec3(centre+titleOffset, componentLayer + borderLayerOffset);
	titleString = "Component " + std::to_string(componentID++);
	title = Renderer::addText2D(titleString, titlePos, titleColour, titleSize, "C", "B", this);
	// Add some test ports. (TO BE REMOVED). PLease keep this here while we are testing (at least until we have some generic components that can be added). 
	// It is a bit of a pain setting up ports every time we test.
	addPort(0, PortType::PORT_IN, "LX1");
	addPort(1, PortType::PORT_OUT, "RX1");
	highlight();

	// Dictionary for GUI of component for data automation.
	cableDict.insert(std::pair<std::string, std::string>("FromTagNumber",	"From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("ToTagNumber",		"From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("From",			"From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("To", "From(Circuit Database)"));
	cableDict.insert(std::pair<std::string, std::string>("Description", "From(Circuit Database)"));
}

Component2D::Component2D(const glm::vec2& centreCoords, Circuit* parent)
	: Component2D(parent)
{
	moveTo(centreCoords);
}

Component2D::~Component2D() 
{
	Renderer::remove(shape);
	Renderer::remove(border);
	Renderer::remove(title);
}

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

void Component2D::moveTo(const glm::vec2& pointerPos)
{
	glm::vec2 translateDestination(pointerPos[0], pointerPos[1]);
	shape->translateTo(translateDestination);
	border->translateTo(translateDestination);
	glm::vec2 titleDest = translateDestination + titleOffset;
	title->translateTo(titleDest);

	for (int i = 0; i < portsWest.size(); i++) 
		portsWest[i]->moveTo(translateDestination);

	for (int i = 0; i < portsEast.size(); i++) 
		portsEast[i]->moveTo(translateDestination);
	
	for (int i = 0; i < portsNorth.size(); i++) 
		portsNorth[i]->moveTo(translateDestination);
	
	for (int i = 0; i < portsSouth.size(); i++) 
		portsSouth[i]->moveTo(translateDestination);
	
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
}

void Component2D::move(const glm::vec2& translation)
{
	shape->translate(translation);
	border->translate(translation);
	title->translate(translation);

	for (int i = 0; i < portsWest.size(); i++) 
		portsWest[i]->move(translation);
	
	for (int i = 0; i < portsEast.size(); i++) 
		portsEast[i]->move(translation);
	
	for (int i = 0; i < portsNorth.size(); i++) 
		portsNorth[i]->move(translation);
	
	for (int i = 0; i < portsSouth.size(); i++) 
		portsSouth[i]->move(translation);
	
	centre += translation;
}

void Component2D::place(const glm::vec2& pos)
{	
	// Ensure the component is at the desired position.
	moveTo(pos);
	setLayer(0.0f);
	shapeColour = { 0.f, 0.f, 1.f, 0.5f };
	titleColour = { 0.f, 0.f, 0.f, 1.0f };
	shape->setColor(shapeColour);
	title->setColor(titleColour);
	// Move to placement layer.
}

void Component2D::setLayer(float layer)
{
	shape->setLayer(layer);
	border->setLayer(layer + borderLayerOffset);
	title->setLayer(layer + borderLayerOffset);

	for (int i = 0; i < portsWest.size(); i++) 
		portsWest[i]->setLayer(layer + portLayerOffset);
	
	for (int i = 0; i < portsEast.size(); i++) 
		portsEast[i]->setLayer(layer + portLayerOffset);
	
	for (int i = 0; i < portsNorth.size(); i++) 
		portsNorth[i]->setLayer(layer + portLayerOffset);
	
	for (int i = 0; i < portsSouth.size(); i++) 
		portsSouth[i]->setLayer(layer + portLayerOffset);
	
	componentLayer = layer;
}

void Component2D::setContext(GUIState* guiState)
{
	//guiState->clickedZone.component = true;
}

void Component2D::highlight()
{
	borderColour = { 0.f, 0.f, 1.0f, 1.f };
	border->setColor(borderColour);

	for (int i = 0; i < portsWest.size(); i++) 
		portsWest[i]->highlight();
	
	for (int i = 0; i < portsEast.size(); i++) 
		portsEast[i]->highlight();
	
	for (int i = 0; i < portsNorth.size(); i++) 
		portsNorth[i]->highlight();
	
	for (int i = 0; i < portsSouth.size(); i++) 
		portsSouth[i]->highlight();
	
}

void Component2D::unhighlight()
{
	borderColour = { 0.f, 0.f, 0.f, 1.f };
	border->setColor(borderColour);

	for (int i = 0; i < portsWest.size(); i++) 
		portsWest[i]->unhighlight();
	
	for (int i = 0; i < portsEast.size(); i++) 
		portsEast[i]->unhighlight();
	
	for (int i = 0; i < portsNorth.size(); i++) 
		portsNorth[i]->unhighlight();
	
	for (int i = 0; i < portsSouth.size(); i++) 
		portsSouth[i]->unhighlight();
	
}

unsigned Component2D::addPort(int side, PortType type, const std::string& name)
{
	switch(side)
	{
	case 0:
		portsWest.push_back(std::make_shared<Port>(glm::vec2(-width, 0.f), type, this, name));
		updatePortPositions();
		return portsWest.back()->m_entityID;

	case 1:
		portsEast.push_back(std::make_shared<Port>(glm::vec2(width, 0.f), type, this, name));
		updatePortPositions();
		return portsEast.back()->m_entityID;

	case 2:
		portsNorth.push_back(std::make_shared<Port>(glm::vec2(0.f, height), type, this, name));
		updatePortPositions();
		return portsNorth.back()->m_entityID;

	case 3:
		portsSouth.push_back(std::make_shared<Port>(glm::vec2(0.f, -height), type, this, name));
		updatePortPositions();
		return portsSouth.back()->m_entityID;

	default:
		// Invalid value passed.
		std::cout << yellow << "\n[Design2D] [WARNING]: " << white << "Cannot add a port to side " << side << " (require side < 4).";
		return NULL;
	}
}

void Component2D::removePort(std::shared_ptr<Port> port)
{

	std::vector<std::shared_ptr<Port>>* allPorts[4] = {&portsWest,
														&portsEast,
														&portsNorth,
														&portsSouth};

	for (int i = 0; i < 4; i++) {
		std::vector<std::shared_ptr<Port>>& portsSide = *allPorts[i];

		auto port_to_remove = std::find(begin(portsSide), end(portsSide), port);

		if (port_to_remove != end(portsSide)) 
		{
			portsSide.erase(port_to_remove);
			portsSide.shrink_to_fit();
			updatePortPositions();
			return;
		}
	}
	//Warn the user if the port was not found on this component (i.e. if we have not returned yet).
	std::cout << yellow << "\n[Design2D] [WARNING]: " << white << "Tried to delete port "<<port->m_label<< ", but it does not belong to component "<< titleString <<".";

}

void Component2D::updatePortPositions()
{
	std::vector<std::shared_ptr<Port>>* allPorts[4] = {	&portsWest,
														&portsEast,
														&portsNorth,
														&portsSouth };

	unsigned* n_ports_sides[4] = {&n_ports_west, &n_ports_east, &n_ports_north, &n_ports_south};

	for (int i = 0; i < 4; i++) {
		std::vector<std::shared_ptr<Port>>& portsSide = *allPorts[i];
		unsigned* n_ports = n_ports_sides[i];
		if (portsSide.size() != *n_ports) {
			*n_ports = portsSide.size();

			//Placement is dependent on which side we are on.
			glm::vec2 offset_side;
			switch (i) 
			{
			case 0:
				//Set west offsets
				offset_side = glm::vec2(-width, -height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(0.f, 2.f * (j+1) * height / (*n_ports + 1));
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;

			case 1:
				//Set east offsets
				offset_side = glm::vec2(width, -height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(0.f, 2.f * (j + 1) * height / (*n_ports + 1));
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;

			case 2:
				//Set north offsets
				offset_side = glm::vec2(-width, height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(2.f * (j + 1) * width / (*n_ports + 1), 0.f);
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;

			case 3:
				//Set south offsets
				offset_side = glm::vec2(-width, -height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(2.f * (j + 1) * width / (*n_ports + 1), 0.f);
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;
			}
		}
	}

}

void Component2D::translateTitle(glm::vec2 translation)
{
	titleOffset += translation;
	title->translate(translation);
}

//void Component2D::destroy()
//{
	/*this->~Component2D();*/
	//shape->destroy();
	//border->destroy();
	//title->destroy();
//}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//