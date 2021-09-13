#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colorData;

out vec4 colorFragment;

void main()
{
	colorFragment = colorData;
	gl_Position = position;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 colorFragment;

void main()
{
	color = colorFragment;
};