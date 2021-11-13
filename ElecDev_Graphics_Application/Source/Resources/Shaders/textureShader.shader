#shader vertex
#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_texCoord;
layout(location = 3) in float v_texID;
layout(location = 4) in uint v_entityID;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec4 f_color;
out vec2 f_texCoord;
out float f_texID;
flat out uint f_entityID;

void main()
{
	f_color =v_color;
	f_texCoord = v_texCoord;
	f_texID = v_texID;
	gl_Position = projectionMatrix * viewMatrix * vec4(pos, 1.0);
	f_entityID = v_entityID;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out uint o_entityID;

in vec4 f_color;
in vec2 f_texCoord;
in float f_texID;
flat in uint f_entityID;

const float width = 0.4;
const float edge = 0.2;

uniform sampler2D f_textures[3];

void main()
{
	int index = int(f_texID);
	if (index == 0)
	{
		o_color = f_color;
	}
	else if (index == 1)
	{
		float distance = (1.0 - texture(f_textures[index], f_texCoord).a);
		float alpha = (1.0 - smoothstep(width, width + edge, distance));
		o_color = vec4(f_color[0], f_color[1], f_color[2], alpha);
	}
	else 
	{
		o_color = texture(f_textures[index], f_texCoord);
	}
	o_entityID = f_entityID;
}