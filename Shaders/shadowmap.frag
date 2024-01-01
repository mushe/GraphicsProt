#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    mat4 WVP;
    mat4 W;
    mat4 shadowVP;
    vec3 lightDir;
    float lightIntensity;
    float time;
} ubo;

layout(binding = 1) uniform sampler2D inSampler;
layout(binding = 2) uniform sampler2D inSampler2;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inPosition;
layout(location = 3) in vec4 inWorldPosition;

layout(location = 0) out vec4 outColor;

void main()
{
    float d = inPosition.z / inPosition.w;
    outColor = vec4(d, d, d, 1.0);
}
