#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    float time;
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

layout(location = 0) out vec4 outColor;


void main()
{
    vec4 fishTex = texture(inSampler, vec2(inTexCoord.y, inTexCoord.x));
    if(fishTex.a < 0.01) discard;
    outColor = fishTex;
}
