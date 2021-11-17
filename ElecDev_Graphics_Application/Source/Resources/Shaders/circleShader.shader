#shader vertex
#version 450 core

// Input.
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_localCoords;
layout(location = 3) in float v_thickness;
layout(location = 4) in float v_fade;
layout(location = 5) in uint v_entityID;

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
layout(location = 1) out VertexOutput Output;

void main()
{
    // Pass circle shader data.
    Output.LocalPosition = v_localCoords;
    Output.Color         = v_color;
    Output.Thickness     = v_thickness;
    Output.Fade          = v_fade;

    // Pass position.
    gl_Position = projectionMatrix * viewMatrix * vec4(v_pos, 1.0);
    // Pass colour.
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
layout(location = 1) in VertexOutput Input;
// Output.
layout(location = 0) out vec4 o_color;
layout(location = 1) out uint o_entityID;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
    float circleAlpha = smoothstep(0.0, Input.Fade, distance);
    if (circleAlpha < 0.9)
        discard;
    circleAlpha *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

    // Set output color
    o_color = Input.Color;
    o_color.a *= circleAlpha;
    o_entityID = f_entityID;
};
