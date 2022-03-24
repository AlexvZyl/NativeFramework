#shader vertex
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

layout(location = 0) out vec4 o_color;
layout(location = 1) out uint o_entityID;

in vec4 f_color;

void main()
{
	o_color = f_color;
	o_entityID = -1;
};