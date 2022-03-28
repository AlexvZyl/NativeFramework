A#shader vertex
#version 450 core

layout(location = 0) in vec4 v_position;
layout(location = 1) in vec4 v_colorData;

out vec4 f_color;

void main()
{
	f_color = v_colorData;
	gl_Position = vec4(v_position.x, v_position.y, 1.f, v_position.w);
};

#shader fragment
#version 450 core

layout(location = 2) out vec4 o_outline;

in vec4 f_color;

void main()
{
	o_outline = vec4(vec3(f_color), 0.0f);
};