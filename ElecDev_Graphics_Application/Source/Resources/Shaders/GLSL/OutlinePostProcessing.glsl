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
#version 330 core

layout(location = 0) out vec4 o_color;

uniform sampler2D f_textures[1];
uniform float width;
uniform float height;

in vec2 f_texCoord;
in float f_texID;

float xIncrements = 1.0f / width;  
float yIncrements = 1.0f / height;  

const float sobelMin = 0.75f;

vec2 offsets[9] = vec2[9]
(
    vec2(-xIncrements,  yIncrements),    vec2( 0.0f,  yIncrements),     vec2( xIncrements,  yIncrements),
    vec2(-xIncrements,  0.0f       ),    vec2( 0.0f,  0.0f       ),     vec2( xIncrements,  0.0f       ),
    vec2(-xIncrements, -yIncrements),    vec2( 0.0f, -yIncrements),     vec2( xIncrements, -yIncrements) 
);

float xKernel[9] = float[9]
(
    -1,  0,  1,
    -2,  0,  2,
    -1,  0,  1
);

float yKernel[9] = float[9]
(
    -1, -2, -1,
     0,  0,  0,
     1,  2,  1
);

float greyScale(vec3 color)
{
    return (color.r + color.g + color.b ) / 3.f;
}

void main()
{
    float xValue = 0;
    float yValue = 0;
    for(int i=0; i<9; i++)
    {
        vec4 fragColor = texture(f_textures[int(f_texID)], f_texCoord.st + offsets[i]);
        xValue += greyScale(vec3(fragColor) * xKernel[i]);
        yValue += greyScale(vec3(fragColor) * yKernel[i]);
    }

    float sobelValue = sqrt( (xValue*xValue) + (yValue*yValue));

    if(sobelValue == 0.f)
    {
        o_color = vec4(0.f, 0.f, 0.f, 0.f);
        return;
    }
    else if(sobelValue <= sobelMin)
    {
        o_color = vec4(sobelMin, sobelMin, sobelMin, 1.f);
    }
    else
    {
        o_color = vec4(sobelValue, sobelValue, sobelValue, 1.f);
    }
}