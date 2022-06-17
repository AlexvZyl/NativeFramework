#shader vertex
#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_texCoord;
layout(location = 3) in float v_outline;
layout(location = 4) in float v_texID;
layout(location = 5) in uint v_entityID;

out vec4 f_color;
out vec2 f_texCoord;
out float f_texID;

void main()
{
	f_color = v_color;
	f_texCoord = v_texCoord;
	f_texID = v_texID;
	gl_Position = vec4(pos, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_color;

in vec4 f_color;
in vec2 f_texCoord;
in float f_texID;

uniform int msaaSamples;
uniform sampler2DMS msaaTexture[1];
uniform int textureSize[2];

void main()
{
	ivec2 msaaTexCoord = { int(f_texCoord.x * textureSize[0]) , int(f_texCoord.y * textureSize[1]) };
	vec4 color = vec4(0.0);
	for (int i = 0; i < msaaSamples; i++)
		color += texelFetch(msaaTexture[int(f_texID)], msaaTexCoord, i);
	color /= float(msaaSamples);
	o_color = color;
}