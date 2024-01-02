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
    float time;
} commonUBO;

layout(binding = 2) uniform sampler2D inSampler;
layout(binding = 3) uniform sampler2D inSampler2;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inPosition;
layout(location = 3) in vec4 inWorldPosition;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 tex = texture(inSampler, vec2(inTexCoord.x, 1.0 - inTexCoord.y));
    if(tex.a < 0.001) discard;
    outColor = tex;
}