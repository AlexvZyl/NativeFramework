#include "Component2D.h"

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

	shapeVAO = std::make_shared<VertexArrayObject>(GL_TRIANGLES);
	borderVAO = std::make_shared<VertexArrayObject>(GL_LINES);
	shapeColour = Colour(0.7f, 0.7f, 0.9f, 0.5f);
	borderColour = Colour(0.5f, 0.5f, 0.5f, 0.5f);

	//VertexData shapeVertices[4];
	VertexData edgeVertices[4];

	shape = std::make_shared<Polygon2D>(vertices, shapeVAO.get());
	shape->setColour(shapeColour);
	shape->setLayer(componentLayer);
	shape->update();
	border = std::make_shared<Polygon2D>(vertices, borderVAO.get());
	border->setColour(borderColour);
	border->setLayer(componentLayer + borderLayerOffset);
	border->update();
}

Component2D::Component2D(float centreCoords[2]):Component2D()
{
	moveTo(centreCoords);
}

Component2D::~Component2D() {
}

void Component2D::moveTo(float pointerPos[2])
{
	shape->translateTo(glm::vec2(pointerPos[0], pointerPos[1]));
	border->translateTo(glm::vec2(pointerPos[0], pointerPos[1]));
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
}

void Component2D::place(float pos[2])
{	//ensure the component is at the desired position
	moveTo(pos);
	setLayer(0.0f);
	shape->setColour(Colour(0.f, 0.f, 1.f, 0.5f));
	border->setColour(Colour(0.5f, 0.5f, 0.5f, 1.f));
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
