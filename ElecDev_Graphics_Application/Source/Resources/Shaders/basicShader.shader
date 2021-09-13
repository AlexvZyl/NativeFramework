#shader vertex
#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 v_color;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec4 f_color;

void main()
{
	f_color = v_color;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(pos, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_color;

in vec4 f_color;

void main()
{
	o_color = f_color;
};