#shader vertex
#version 450 core

// Input.
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in float v_radius;
layout(location = 3) in vec2 v_localCoords;
layout(location = 4) in float v_outline;
layout(location = 5) in float v_thickness;
layout(location = 6) in float v_fade;
layout(location = 7) in uint v_entityID;

uniform mat4 viewProjMatrix;

struct VertexOutput
{
    float Radius;
    vec2 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

// Output.
flat out uint f_entityID;
layout(location = 2) out VertexOutput Output;

void main()
{
    Output.Radius        = v_radius;
    Output.LocalPosition = v_localCoords;
    Output.Color         = v_color;
    Output.Thickness     = v_thickness;
    Output.Fade          = v_fade;
    gl_Position          = viewProjMatrix * vec4(v_pos, 1.0);
    f_entityID           = v_entityID;
}

#shader fragment
#version 450 core

struct VertexOutput
{
    float Radius;
    vec2 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

// Input.
flat in uint f_entityID;
layout(location = 2) in VertexOutput Input;

// Output.
layout(location = 0) out vec4 o_color;
layout(location = 1) out uint o_entityID;

void main()
{
    // Calculate distance and fill circle with white
    float normalisedThickness = Input.Thickness / Input.Radius;
    if(normalisedThickness > 1)
        normalisedThickness = 1;
    float distance = 1.0 - length(Input.LocalPosition);
    float circleAlpha = smoothstep(0.0, Input.Fade, distance);
    circleAlpha *= smoothstep(normalisedThickness + Input.Fade, normalisedThickness, distance);
    if (circleAlpha < 0.5)  //  The 0.5 value should change based on the fade value of the circle.
        discard;            //  If not the fade does not work properly.

    // Set output color
    o_color = Input.Color;
    o_color.a *= circleAlpha;
    o_entityID = f_entityID;
};
