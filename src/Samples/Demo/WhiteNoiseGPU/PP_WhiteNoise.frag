#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    float postProcessLerp;
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

layout(location = 0) out vec4 outColor;

float rand(vec2 p)
{
    return fract(sin(dot(p.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand(vec3 p)
{
    return fract(sin(dot(p, vec3(12.9898, 78.233, 31.2905))) * 43758.5453);
}

void main()
{
    float r = rand(vec3(inTexCoord.x, inTexCoord.y, commonUBO.time));
    outColor = vec4(r, r, r, 1.0);
}
