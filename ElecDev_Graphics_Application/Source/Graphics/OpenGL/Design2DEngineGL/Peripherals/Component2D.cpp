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
	edges = std::make_shared<VertexArrayObject>(GL_LINES);
	shapeColour = Colour(1.0f, 0.0f, 0.0f, 0.5f);
	edgeColour = Colour(1.0f, 0.0f, 0.0f, 1.0f);

	// Set static eID for now.
	unsigned shapeID = 1;
	unsigned edgeID = 2;
	//VertexData shapeVertices[4];
	VertexData edgeVertices[4];

	// Populate VertexData structures.
	for (int i = 0; i < 4; i++) {
		//shapeVertices[i] = VertexData(vertices[i], shapeColour, shapeID);
		edgeVertices[i] = VertexData(vertices[i], edgeColour, edgeID++);
	}
	//std::vector<VertexData> shapeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[0] };
	shape = Polygon2D(vertices, shapeVAO.get());
	shape.setColour(shapeColour);
	shape.setEID(shapeID);
	shape.update();
	std::vector<VertexData> edgeVerticesVec = { edgeVertices[0], edgeVertices[1], edgeVertices[1], edgeVertices[2], edgeVertices[2], edgeVertices[3], edgeVertices[3], edgeVertices[0] };

	// Write to CPU side buffer.
	//shape->assignDataCPU(shapeVerticesVec, 0);
	//shape->updateGPU();
	edges->assignDataCPU(edgeVerticesVec, 0);
	edges->updateGPU();
}

Component2D::Component2D(float centreCoords[2]):Component2D()
{
	moveTo(centreCoords);
}

Component2D::~Component2D() {
}

void Component2D::draw()
{
	// Set static eID for now.
	unsigned shapeID = 1;
	unsigned edgeID = 2;
	VertexData shapeVertices[4];
	VertexData edgeVertices[4];

	// Populate VertexData structures.
	for (int i = 0; i < 4; i++) {
		shapeVertices[i] = VertexData(vertices[i], shapeColour, shapeID);
		edgeVertices[i] = VertexData(vertices[i], shapeColour, edgeID++);
	}

	//It may be faster to edit the buffers rather than recreate them each time -- This implementation prioritises memory usage over runtime
	std::vector<VertexData> shapeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[0] };
	std::vector<VertexData> edgeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[3], shapeVertices[0] };
	// Write to GPU side buffer.

	//shape->assignDataCPU(shapeVerticesVec, 0);
	//shape->updateGPU();
	edges->assignDataCPU(edgeVerticesVec, 0);
	edges->updateGPU();
}

void Component2D::moveTo(float pointerPos[2])
{
	shape.translateTo(glm::vec2(pointerPos[0], pointerPos[1]));
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
	vertices[0] = glm::vec3(centre.x - width, centre.y - height, 0.0f);
	vertices[1] = glm::vec3(centre.x + width, centre.y - height, 0.0f);
	vertices[2] = glm::vec3(centre.x + width, centre.y + height, 0.0f);
	vertices[3] = glm::vec3(centre.x - width, centre.y + height, 0.0f);
	draw();
}

void Component2D::place(float pos[2])
{	//ensure the component is at the desired position
	this->moveTo(pos);

	//Move to placement layer
}

void Component2D::render()
{
	shapeVAO->render();
	edges->render();
}
