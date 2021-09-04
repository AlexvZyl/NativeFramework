#shader vertex
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 colorData;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec4 colorFragment;

void main()
{
	colorFragment = colorData;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(pos, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 colorFragment;

void main()
{
	color = colorFragment;
};