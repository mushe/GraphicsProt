#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    float colorLerp;
} ubo;

layout(binding = 1)
uniform CommonUniformBufferObject
{
    mat4 WVP;
    mat4 VP;
    mat4 W;
    mat4 V;
    mat4 P;
    float time;
} commonUBO;

layout(binding = 3) uniform sampler2D inSampler;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inPosition;
layout(location = 3) in vec4 inWorldPosition;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBiTangent;
layout(location = 6) in vec3 inLightDirection;
layout(location = 7) in vec3 inCameraPos;

layout(location = 0) out vec4 outColor;


void main()
{
    vec4 tex = texture(inSampler, vec2(inTexCoord.x, 1.0 - inTexCoord.y)) * 2.0 - 1.0;

    vec4 col = vec4(0.0, 0.0, 0.0, 1.0);

    float gray = tex.r;

    float thresh = 0.25;

    // r
    if(gray < 0.5) col.r = 0.0;
    else if(gray < 1.0 - thresh) col.r = mix(0.0, 1.0, (gray - 0.5) / (0.5 - thresh));
    else col.r = 1.0;

    // g
    if (gray < thresh) col.g = mix(0.0, 1.0, gray / thresh);
    else if(gray < 1.0 - thresh) col.g = 1.0;
    else col.g = mix(1.0, 0.0, (gray - (1.0 - thresh)) / thresh);

    // b
    if(gray < thresh) col.b = 1.0;
    else if(gray < 0.5) col.b = mix(1.0, 0.0, (gray - thresh) / thresh); 
    else col.b = 0.0;

    outColor = mix(tex, col, ubo.colorLerp);
}
