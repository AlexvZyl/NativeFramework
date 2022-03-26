#shader vertex
#version 450 core

// Input.
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_localCoords;
layout(location = 3) in float v_outline;
layout(location = 4) in float v_thickness;
layout(location = 5) in float v_fade;
layout(location = 6) in uint v_entityID;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

struct VertexOutput
{
    vec2 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

// Output.
flat out uint f_entityID;
out float f_outline;
layout(location = 2) out VertexOutput Output;

void main()
{
    f_outline = v_outline;
    if(v_outline == 0.0f)
        return;

    Output.LocalPosition = v_localCoords;
    Output.Color         = v_color;
    Output.Thickness     = v_thickness;
    Output.Fade          = v_fade;
    gl_Position = projectionMatrix * viewMatrix * vec4(v_pos, 1.0);
    f_entityID = v_entityID;
}

#shader fragment
#version 450 core

struct VertexOutput
{
    vec2 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

// Input.
flat in uint f_entityID;
in float f_outline;
layout(location = 2) in VertexOutput Input;

// Output.
layout(location = 2) out vec4 o_outline;

void main()
{
    if(f_outline == 0.0f)
        discard;

    vec4 color;

    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
    float circleAlpha = smoothstep(0.0, Input.Fade, distance);
    if (circleAlpha < 0.5)  //  The 0.5 value should change based on the fade value of the circle.
        discard;            //  If not the fade does not work properly.
    circleAlpha *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

    // Set output color
    color = Input.Color;
    color.a *= circleAlpha;

    if(color.a == 0.0f)
        discard;

    o_outline = vec4(vec3(color), f_outline);
};
