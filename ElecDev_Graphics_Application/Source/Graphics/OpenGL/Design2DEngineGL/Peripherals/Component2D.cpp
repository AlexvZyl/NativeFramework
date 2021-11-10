
#include "Component2D.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/VertexArrayObjectGL.h"

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

	shapeVAO = std::make_shared<VertexArrayObject>(GL_TRIANGLES, false);
	edgesVAO = std::make_shared<VertexArrayObject>(GL_LINES, false);
	shapeColour = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
	edgeColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	// Set static eID for now.
	unsigned shapeID = 1;
	unsigned edgeID = 2;
	//VertexData shapeVertices[4];
	std::shared_ptr<VertexData> edgeVertices[4];

	// Populate VertexData structures.
	for (int i = 0; i < 4; i++) {
		//shapeVertices[i] = VertexData(vertices[i], shapeColour, shapeID);
		edgeVertices[i] = std::make_shared<VertexData>(vertices[i], edgeColour, edgeID++);
	}
	//std::vector<VertexData> shapeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[0] };
	shape = Polygon2D(vertices, shapeVAO.get());
	shape.setColor(shapeColour);
	shape.setEntityID(shapeID);
	shape.update();
	std::vector<std::shared_ptr<Vertex>> edgeVerticesVec = { edgeVertices[0], edgeVertices[1], edgeVertices[1], edgeVertices[2], edgeVertices[2], edgeVertices[3], edgeVertices[3], edgeVertices[0] };

	// Write to CPU side buffer.
	edgesVAO->assignDataCPU(edgeVerticesVec, 0);
	edgesVAO->updateGPU();
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
	std::shared_ptr<Vertex> shapeVertices[4];
	std::shared_ptr<Vertex> edgeVertices[4];

	// Populate VertexData structures.
	for (int i = 0; i < 4; i++) {
		shapeVertices[i] = std::make_shared<VertexData>(vertices[i], shapeColour, shapeID);
		edgeVertices[i] = std::make_shared<VertexData>(vertices[i], shapeColour, edgeID++);
	}

	//It may be faster to edit the buffers rather than recreate them each time -- This implementation prioritises memory usage over runtime
	std::vector<std::shared_ptr<Vertex>> shapeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[0] };
	std::vector<std::shared_ptr<Vertex>> edgeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[3], shapeVertices[0] };
	// Write to GPU side buffer.

	//shape->assignDataCPU(shapeVerticesVec, 0);
	//shape->updateGPU();
	edgesVAO->assignDataCPU(edgeVerticesVec, 0);
	edgesVAO->updateGPU();
}

void Component2D::moveTo(float pointerPos[2])
{
	shape.translateTo(glm::vec3(pointerPos[0], pointerPos[1], 0.f));
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
	vertices[0] = glm::vec3(centre.x - width, centre.y - height, 0.0f);
	vertices[1] = glm::vec3(centre.x + width, centre.y - height, 0.0f);
	vertices[2] = glm::vec3(centre.x + width, centre.y + height, 0.0f);
	vertices[3] = glm::vec3(centre.x - width, centre.y + height, 0.0f);
	draw();
}

void Component2D::render()
{
	shapeVAO->render();
	edgesVAO->render();
}
