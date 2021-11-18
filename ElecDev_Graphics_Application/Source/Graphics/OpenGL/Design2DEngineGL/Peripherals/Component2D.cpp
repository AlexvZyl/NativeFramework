
#include "Component2D.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/Entities/Polygon.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/FontLoader.h"
#include "CoreGL/Entities/Text.h"
#include "Resources/ResourceHandler.h"
#include "CoreGL/Entities/Circle.h"

//Add font for component titles
Font Component2D::titleFont = loadFont(ARIAL_SDF_FNT, ARIAL_SDF_PNG);
unsigned Component2D::componentID = 0;


Component2D::Component2D(VertexArrayObject<VertexData>* trianglesVAO, 
						 VertexArrayObject<VertexData>* linesVAO, 
						 VertexArrayObject<VertexDataTextured>* texturedTrianglesVAO,
						 VertexArrayObject<VertexDataCircle>* circleVAO)
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

	vertices2.insert(vertices2.end(), glm::vec3(centre.x - width/2, centre.y - height/2, 0.0f));
	vertices2.insert(vertices2.end(), glm::vec3(centre.x + width/2, centre.y - height/2, 0.0f));
	vertices2.insert(vertices2.end(), glm::vec3(centre.x + width/2, centre.y + height/2, 0.0f));
	vertices2.insert(vertices2.end(), glm::vec3(centre.x - width/2, centre.y + height/2, 0.0f));

	engine_trianglesVAO = trianglesVAO;
	engine_linesVAO = linesVAO;
	engine_texturedTrianglesVAO = texturedTrianglesVAO;
	engine_circleVAO = circleVAO;
	shapeColour = glm::vec4(0.5f, 0.5f, 0.9f, 0.5f);
	titleColour = glm::vec4(0.3f, 0.3f, 0.9f, 0.5f);
	borderColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

	std::shared_ptr<VertexData> edgeVertices[4];

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
	title = std::make_shared<Text<VertexDataTextured>>(titleString, titlePos, titleColour, titleSize, engine_texturedTrianglesVAO, titleFont, this);
	title->update();
	// Port.
	portOffset = glm::vec2(centre+glm::vec2(width, 0.f));
	glm::vec3 portPos = glm::vec3(portOffset, componentLayer + borderLayerOffset);
	glm::vec3 port2Pos = glm::vec3(-portOffset, componentLayer + borderLayerOffset);
	PortType type = PORT_IN;
	port1 = std::make_shared<Port>(portPos, type, this);
	port2 = std::make_shared<Port>(port2Pos, type, this);
	//port1->setColor(borderColour);
	//port1->setLayer(componentLayer);
	//port1->update();


}

Component2D::Component2D(float centreCoords[2], 
						 VertexArrayObject<VertexData>* trianglesVAO, 
						 VertexArrayObject<VertexData>* linesVAO, 
						 VertexArrayObject<VertexDataTextured>* texturedTrianglesVAO,
						 VertexArrayObject<VertexDataCircle>* circleVAO)
	: Component2D(trianglesVAO, linesVAO, texturedTrianglesVAO, circleVAO)
{
	moveTo(centreCoords);
}

Component2D::~Component2D() 
{
}

void Component2D::moveTo(float pointerPos[2])
{
	glm::vec2 translateDestination(pointerPos[0], pointerPos[1]);
	shape->translateTo(translateDestination);
	border->translateTo(translateDestination);
	glm::vec2 titleDest = translateDestination + titleOffset;
	title->translateTo(titleDest);
	glm::vec2 portDest = translateDestination + portOffset;
	glm::vec2 port2Dest = translateDestination - portOffset;
	port1->moveTo(portDest);
	port2->moveTo(port2Dest);
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
	borderColour = { 0.f, 0.f, 0.f, 1.f };
	shape->setColor(shapeColour);
	border->setColor(borderColour);
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
	port1->setLayer(layer + portLayerOffset);
	port2->setLayer(layer + portLayerOffset);
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
	port1->update();
	port2->update();
}

void Component2D::destroy()
{
	this->~Component2D();
	//shape->destroy();
	//border->destroy();
	//title->destroy();
}