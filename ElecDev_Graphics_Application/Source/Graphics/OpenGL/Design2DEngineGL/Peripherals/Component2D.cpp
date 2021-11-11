
#include "Component2D.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/FontLoader.h"
#include "CoreGL/Entities/Text.h"
#include "Resources/ResourceHandler.h"

//Add font for component titles
Font Component2D::titleFont = loadFont(ARIAL_SDF_FNT, ARIAL_SDF_PNG);


Component2D::Component2D(VertexArrayObject* trianglesVAO, VertexArrayObject* linesVAO, VertexArrayObject* texturedTrianglesVAO)
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

	engine_trianglesVAO = trianglesVAO;
	engine_linesVAO = linesVAO;
	engine_texturedTrianglesVAO = texturedTrianglesVAO;
	shapeColour = glm::vec4(0.5f, 0.5f, 0.9f, 0.9f);
	borderColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

	//VertexData shapeVertices[4];
	std::shared_ptr<VertexData> edgeVertices[4];

	shape = std::make_shared<Polygon2D>(vertices, engine_trianglesVAO);
	shape->setColor(shapeColour);
	shape->setLayer(componentLayer);
	shape->update();
	border = std::make_shared<Polygon2D>(vertices, engine_linesVAO);
	border->setColor(borderColour);
	border->setLayer(componentLayer + borderLayerOffset);
	border->update();
	title = std::make_shared<Text>("Generic Component", glm::vec3(centre, componentLayer + borderLayerOffset), glm::vec4(0.f, 0.f, 1.f, .5f), 1.f, 0, engine_texturedTrianglesVAO, &titleFont);
	title->update();

}

Component2D::Component2D(float centreCoords[2], VertexArrayObject* trianglesVAO, VertexArrayObject* linesVAO, VertexArrayObject* texturedTrianglesVAO):Component2D(trianglesVAO, linesVAO, texturedTrianglesVAO)
{
	moveTo(centreCoords);
}

Component2D::~Component2D() {
}

void Component2D::moveTo(float pointerPos[2])
{
	shape->translateTo(glm::vec3(pointerPos[0], pointerPos[1], 0.f));
	border->translateTo(glm::vec3(pointerPos[0], pointerPos[1], 0.f));
	shape->update();
	border->update();
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
}

void Component2D::place(float pos[2])
{	//ensure the component is at the desired position
	moveTo(pos);
	setLayer(0.0f);
	shape->setColor(glm::vec4(0.f, 0.f, 1.f, 0.5f));
	border->setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
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
	//deprecated
	//rendering of VAOs moved to engine
}
