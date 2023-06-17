#shader vertex
#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_texCoord;
layout(location = 3) in float v_texID;
layout(location = 4) in uint v_entityID;

out vec2 f_texCoord;
out float f_texID;

void main()
{
	f_texCoord = v_texCoord;
	f_texID = v_texID;
	gl_Position = vec4(pos, 1.0f);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_color;

in vec2 f_texCoord;
in float f_texID;

uniform sampler2D f_textures[1];

void main()
{
	o_color = texture(f_textures[int(f_texID)], f_texCoord);
}