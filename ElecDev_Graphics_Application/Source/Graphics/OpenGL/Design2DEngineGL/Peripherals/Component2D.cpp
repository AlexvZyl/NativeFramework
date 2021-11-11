
#include "Component2D.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/FontLoader.h"
#include "CoreGL/Entities/Text.h"
#include "Resources/ResourceHandler.h"

//Add font for component titles
Font Component2D::titleFont = loadFont(ARIAL_SDF_FNT, ARIAL_SDF_PNG);


Component2D::Component2D()
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

	shapeVAO = std::make_unique<VertexArrayObject>(GL_TRIANGLES, false, false);
	borderVAO = std::make_unique<VertexArrayObject>(GL_LINES, false, false);
	textVAO = std::make_unique<VertexArrayObject>(GL_TRIANGLES, true, false);
	shapeColour = glm::vec4(0.5f, 0.5f, 0.9f, 0.9f);
	borderColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

	shape = std::make_unique<Polygon2D>(&vertices, shapeVAO.get());
	shape->setColor(&shapeColour);
	shape->setLayer(componentLayer);
	shape->update();
	border = std::make_unique<Polygon2D>(&vertices, borderVAO.get());
	border->setColor(&borderColour);
	border->setLayer(componentLayer + borderLayerOffset);
	border->update();
	glm::vec3 texPos1(centre, componentLayer + borderLayerOffset);
	glm::vec4 texCol1(0.f, 0.f, 1.f, 1.f);
	title = std::make_unique<Text>("Generic Component", &texPos1, &texCol1, 1.f, 0, textVAO.get(), &titleFont);
	title->update();
}

Component2D::Component2D(float centreCoords[2]):Component2D()
{
	moveTo(centreCoords);
}

Component2D::~Component2D() {
}

void Component2D::moveTo(float pointerPos[2])
{
	glm::vec3 translateDestination(pointerPos[0], pointerPos[1], 0.f);
	shape->translateTo(&translateDestination);
	border->translateTo(&translateDestination);
	shape->update();
	border->update();
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
}

void Component2D::place(float pos[2])
{	//ensure the component is at the desired position
	moveTo(pos);
	setLayer(0.0f);
	glm::vec4 color1(0.f, 0.f, 1.f, 0.5f);
	glm::vec4 color2(0.f, 0.f, 0.f, 1.f);
	shape->setColor(&color1);
	border->setColor(&color2);
	shape->update();
	border->update();
	//Move to placement layer
}

void Component2D::setLayer(float layer)
{
	shape->setLayer(layer);
	border->setLayer(layer + borderLayerOffset);
	componentLayer = layer;
}

void Component2D::destroy()
{
	shape->destroy();
	border->destroy();
}

void Component2D::render()
{
	shapeVAO->render();
	borderVAO->render();
}
