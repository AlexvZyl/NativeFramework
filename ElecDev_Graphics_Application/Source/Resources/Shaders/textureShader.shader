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

uniform float screenPxRange;

uniform sampler2D f_textures[32];

// Utility function for MSDF rendering.
float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	// Get the texture ID.
	int index = int(f_texID);

	// Render object without texture.
	if (index == 0)
	{
		o_color = f_color;
	}
	// Render MSDF text.
	else if (index == 1)
	{
		float screenPxRangeTemp = 10;
		vec3 msd = texture(f_textures[1], f_texCoord).rgb;
		float sd = median(msd.r, msd.g, msd.b);
		float screenPxDistance = screenPxRangeTemp * (sd - 0.5);
		float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
		if (opacity == 0)
			discard;
		o_color.rgb = f_color.rgb;
		o_color.a = opacity;
	}
	// Render normal textured entities.
	else 
	{
		o_color = texture(f_textures[index], f_texCoord);
	}

	// Output the entity ID.
	o_entityID = f_entityID;
}