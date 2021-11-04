#include "Component2D.h"


Component2D::Component2D()
{
	centre = float2(0.0f, 0.0f);
	vertices[0] = float3(centre.x - width, centre.y - height, 0.0f);
	vertices[1] = float3(centre.x + width, centre.y - height, 0.0f);
	vertices[2] = float3(centre.x - width, centre.y + height, 0.0f);
	vertices[3] = float3(centre.x + width, centre.y + height, 0.0f);

	shape = new VertexArrayObject(GL_TRIANGLES);
	edges = new VertexArrayObject(GL_LINES);
	shapeColour = Colour(1.0f, 0.0f, 0.0f, 0.5f);
	edgeColour = Colour(1.0f, 0.0f, 0.0f, 1.0f);

}

Component2D::Component2D(float centreCoords[2])
{
	Component2D();
	centre = float2(centreCoords[0], centreCoords[1]);
}

Component2D::~Component2D() {
	delete shape;
	delete edges;
}

void Component2D::draw()
{
	//set static eID for now
	unsigned shapeID = 1;
	unsigned edgeID = 2;
	VertexData shapeVertices[4];
	VertexData edgeVertices[4];

	//populate VertexData structures
	for (int i = 0; i < 4; i++) {
		shapeVertices[i] = VertexData(vertices[i], shapeColour, shapeID);
		edgeVertices[i] = VertexData(vertices[i], shapeColour, edgeID++);
	}
	std::vector<VertexData> shapeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[0] };
	std::vector<VertexData> edgeVerticesVec = { shapeVertices[0], shapeVertices[1], shapeVertices[1], shapeVertices[2], shapeVertices[2], shapeVertices[3], shapeVertices[3], shapeVertices[0] };
	// Write to CPU side buffer.
	shape->assignData(shapeVerticesVec);
	shape->updateGPU();
	edges->assignData(edgeVerticesVec);
	edges->updateGPU();
}

void Component2D::moveTo(float pointerPos[2])
{
	//std::cout << pointerPos[0] << ',' << pointerPos[1] << std::endl;
	centre = float2(pointerPos[0], pointerPos[1]);
	vertices[0] = float3(centre.x - width, centre.y - height, 0.0f);
	vertices[1] = float3(centre.x + width, centre.y - height, 0.0f);
	vertices[2] = float3(centre.x + width, centre.y + height, 0.0f);
	vertices[3] = float3(centre.x - width, centre.y + height, 0.0f);
	draw();
}

void Component2D::render()
{
	shape->render();
	edges->render();
}