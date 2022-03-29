#shader vertex
#version 450 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in float v_outline;
layout(location = 3) in uint v_entityID;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out float f_outline;
out vec4 f_color;

void main()
{
	f_outline = v_outline;
	if(v_outline == 0.0f)
		return;

	f_color = v_color;
    gl_Position = projectionMatrix * viewMatrix * vec4(v_pos, 1.0f);
}

#shader fragment
#version 450 core

layout(location = 2) out vec4 o_outline;

in float f_outline;
in vec4 f_color;

void main()
{
	if(f_outline == 0.0f)
		discard;

	o_outline = vec4(vec3(f_color), f_outline);
};