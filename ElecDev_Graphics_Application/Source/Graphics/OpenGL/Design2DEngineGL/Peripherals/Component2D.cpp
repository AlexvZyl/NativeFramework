//=============================================================================================================================================//
// Includes.																																   //
//=============================================================================================================================================//

#include "Component2D.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/Entities/Polygon.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/FontLoader.h"
#include "CoreGL/Entities/Text.h"
#include "Utilities/Resources/ResourceHandler.h"
#include "CoreGL/Entities/Circle.h"
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Circuit.h"

//=============================================================================================================================================//
//  Variables.																																   //
//=============================================================================================================================================//

//Add font for component titles
Font Component2D::titleFont = msdfLoadFont(ROBOTO_MEDIUM_MSDF_JSON);
unsigned Component2D::componentID = 0;

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Component2D::Component2D(VertexArrayObject<VertexData>* trianglesVAO, 
						 VertexArrayObject<VertexData>* linesVAO, 
						 VertexArrayObject<VertexDataTextured>* texturedTrianglesVAO,
						 VertexArrayObject<VertexDataCircle>* circleVAO, Circuit* parent):Entity(EntityType::COMPONENT, parent)
{
	// --------------------------- //
	//  I N I T I A L   S E T U P  //
	// --------------------------- //

	// Create vertices.
	centre = glm::vec2(0.0f, 0.0f);
	vertices.insert(vertices.end(), glm::vec3(centre.x - width, centre.y - height, 0.0f));
	vertices.insert(vertices.end(), glm::vec3(centre.x + width, centre.y - height, 0.0f));
	vertices.insert(vertices.end(), glm::vec3(centre.x + width, centre.y + height, 0.0f));
	vertices.insert(vertices.end(), glm::vec3(centre.x - width, centre.y + height, 0.0f));

	/*/
	vertices2.insert(vertices2.end(), glm::vec3(centre.x - width/2, centre.y - height/2, 0.0f));
	vertices2.insert(vertices2.end(), glm::vec3(centre.x + width/2, centre.y - height/2, 0.0f));
	vertices2.insert(vertices2.end(), glm::vec3(centre.x + width/2, centre.y + height/2, 0.0f));
	vertices2.insert(vertices2.end(), glm::vec3(centre.x - width/2, centre.y + height/2, 0.0f));
	*/

	engine_trianglesVAO = trianglesVAO;
	engine_linesVAO = linesVAO;
	engine_texturedTrianglesVAO = texturedTrianglesVAO;
	engine_circleVAO = circleVAO;
	shapeColour = { 0.5f, 0.5f, 0.9f, 0.5f };
	titleColour = { 0.3f, 0.3f, 0.9f, 0.5f };
	borderColour = { 0.8f, 1.0f, .8f, 1.f };

	// Main shape.
	shape = std::make_shared<Polygon2D<VertexData>>(vertices, engine_trianglesVAO, this);
	shape->setColor(shapeColour);
	shape->setLayer(0.001f);//temp fix
	shape->update();
	// Component border.
	border = std::make_shared<Polygon2D<VertexData>>(vertices, engine_linesVAO, this);
	border->setColor(borderColour);
	border->setLayer(componentLayer + borderLayerOffset);
	border->update();
	// Component title.
	glm::vec3 titlePos = glm::vec3(centre+titleOffset, componentLayer + borderLayerOffset);
	titleString = "Component " + std::to_string(componentID++);
	title = std::make_shared<Text<VertexDataTextured>>(titleString, titlePos, titleColour, titleSize, engine_texturedTrianglesVAO, titleFont, this, "C", "B");
	title->update();
	// Add some test ports. (TO BE REMOVED)
	addPort(0, PORT_IN, "Test in");
	addPort(1, PORT_OUT, "Test out");

}

Component2D::Component2D(float centreCoords[2], 
						 VertexArrayObject<VertexData>* trianglesVAO, 
						 VertexArrayObject<VertexData>* linesVAO, 
						 VertexArrayObject<VertexDataTextured>* texturedTrianglesVAO,
						 VertexArrayObject<VertexDataCircle>* circleVAO, Circuit* parent)
	: Component2D(trianglesVAO, linesVAO, texturedTrianglesVAO, circleVAO, parent)
{
	moveTo(centreCoords);
}

Component2D::~Component2D() 
{
	//engine_circleVAO->updateGPU();
}

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

void Component2D::moveTo(float pointerPos[2])
{
	glm::vec2 translateDestination(pointerPos[0], pointerPos[1]);
	shape->translateTo(translateDestination);
	border->translateTo(translateDestination);
	glm::vec2 titleDest = translateDestination + titleOffset;
	title->translateTo(titleDest);
	//port1->moveTo(portDest);
	//port2->moveTo(port2Dest);
	for (int i = 0; i < portsWest.size(); i++) {
		portsWest[i]->moveTo(translateDestination);
	}
	for (int i = 0; i < portsEast.size(); i++) {
		portsEast[i]->moveTo(translateDestination);
	}
	for (int i = 0; i < portsNorth.size(); i++) {
		portsNorth[i]->moveTo(translateDestination);
	}
	for (int i = 0; i < portsSouth.size(); i++) {
		portsSouth[i]->moveTo(translateDestination);
	}
	shape->update();
	border->update();
	title->update();
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
}

void Component2D::place(float pos[2])
{	//ensure the component is at the desired position
	moveTo(pos);
	setLayer(0.0f);
	shapeColour = { 0.f, 0.f, 1.f, 0.5f };
	titleColour = { 0.f, 0.f, 1.f, 1.0f };
	shape->setColor(shapeColour);
	title->setColor(titleColour);
	//port1->setColor(borderColour);
	shape->update();
	border->update();
	title->update();
	//Move to placement layer
}

void Component2D::setLayer(float layer)
{
	shape->setLayer(layer);
	border->setLayer(layer + borderLayerOffset);
	title->setLayer(layer + borderLayerOffset);
	//port1->setLayer(layer + portLayerOffset);
	//port2->setLayer(layer + portLayerOffset);
	for (int i = 0; i < portsWest.size(); i++) {
		portsWest[i]->setLayer(layer + portLayerOffset);
	}
	for (int i = 0; i < portsEast.size(); i++) {
		portsEast[i]->setLayer(layer + portLayerOffset);
	}
	for (int i = 0; i < portsNorth.size(); i++) {
		portsNorth[i]->setLayer(layer + portLayerOffset);
	}
	for (int i = 0; i < portsSouth.size(); i++) {
		portsSouth[i]->setLayer(layer + portLayerOffset);
	}
	componentLayer = layer;
}

void Component2D::setContext(GUIState* guiState)
{
	guiState->clickedZone.component = true;
}

void Component2D::update()
{
	shape->update();
	border->update();
	title->update();
	for (int i = 0; i < portsWest.size(); i++) {
		portsWest[i]->update();
	}
	for (int i = 0; i < portsEast.size(); i++) {
		portsEast[i]->update();
	}
	for (int i = 0; i < portsNorth.size(); i++) {
		portsNorth[i]->update();
	}
	for (int i = 0; i < portsSouth.size(); i++) {
		portsSouth[i]->update();
	}
	//port1->update();
	//port2->update();
}

void Component2D::highlight()
{
	borderColour = { 0.8f, 1.0f, .8f, 1.f };
	border->setColor(borderColour);
	border->update();

	for (int i = 0; i < portsWest.size(); i++) {
		portsWest[i]->highlight();
	}
	for (int i = 0; i < portsEast.size(); i++) {
		portsEast[i]->highlight();
	}
	for (int i = 0; i < portsNorth.size(); i++) {
		portsNorth[i]->highlight();
	}
	for (int i = 0; i < portsSouth.size(); i++) {
		portsSouth[i]->highlight();
	}
}

void Component2D::unhighlight()
{
	borderColour = { 0.f, 0.f, 0.f, 1.f };
	border->setColor(borderColour);
	border->update();

	for (int i = 0; i < portsWest.size(); i++) {
		portsWest[i]->unhighlight();
	}
	for (int i = 0; i < portsEast.size(); i++) {
		portsEast[i]->unhighlight();
	}
	for (int i = 0; i < portsNorth.size(); i++) {
		portsNorth[i]->unhighlight();
	}
	for (int i = 0; i < portsSouth.size(); i++) {
		portsSouth[i]->unhighlight();
	}
}

void Component2D::addPort(int side, PortType type, std::string name)
{
	switch(side){
	case 0:
		portsWest.push_back(std::make_shared<Port>(glm::vec2(-width, 0.f), type, this, name));
		break;
	case 1:
		portsEast.push_back(std::make_shared<Port>(glm::vec2(width, 0.f), type, this, name));
		break;
	case 2:
		portsNorth.push_back(std::make_shared<Port>(glm::vec2(0.f, height), type, this, name));
		break;
	case 3:
		portsSouth.push_back(std::make_shared<Port>(glm::vec2(0.f, -height), type, this, name));
		break;
	default:
		//If we get here, then the side value is not valid.
		std::cout << yellow << "\n[Design2D] [WARNING]: " << white << "Cannot add a port to side " << side << " (require side < 4).";
	}
	updatePortPositions();
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

		if (port_to_remove != end(portsSide)) {
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
			switch (i) {
			case 0:
				//Set west offsets
				offset_side = glm::vec2(-width, -height);
				for (int j = 0; j < *n_ports; j++) {
					glm::vec2 offset_j = glm::vec2(0.f, 2.f * (j+1) * height / (*n_ports + 1));
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;
			case 1:
				//Set east offsets
				offset_side = glm::vec2(width, -height);
				for (int j = 0; j < *n_ports; j++) {
					glm::vec2 offset_j = glm::vec2(0.f, 2.f * (j + 1) * height / (*n_ports + 1));
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;
			case 2:
				//Set north offsets
				offset_side = glm::vec2(-width, height);
				for (int j = 0; j < *n_ports; j++) {
					glm::vec2 offset_j = glm::vec2(2.f * (j + 1) * width / (*n_ports + 1), 0.f);
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;
			case 3:
				//Set south offsets
				offset_side = glm::vec2(-width, -height);
				for (int j = 0; j < *n_ports; j++) {
					glm::vec2 offset_j = glm::vec2(2.f * (j + 1) * width / (*n_ports + 1), 0.f);
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;
			}
		}
	}

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