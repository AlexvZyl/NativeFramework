#include <iostream>
#include "shadersSource.h"

const std::string simpleVertexShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"
"uniform vec3 color;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(color * vertexColor, 1.0);\n"
"};\0";

const std::string simpleFragmentShader = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"uniform float rotation;\n"
"uniform vec2 translation;\n"
"void main()\n"
"{\n"
"	vec2 rotated_pos;\n"
"	rotated_pos.x = translation.x + position.x * cos(rotation) - position.y * sin(rotation);\n"
"	rotated_pos.y = translation.y + position.x * sin(rotation) + position.y * cos(rotation);\n"
"	gl_Position = vec4(rotated_pos.x, rotated_pos.y, position.z, 1.0);\n"
"	vertexColor = color;\n"
"};\0";