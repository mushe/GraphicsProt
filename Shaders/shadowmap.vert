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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outPosition;
layout(location = 3) out vec4 outWorldPosition;


void main()
{
    gl_Position = ubo.WVP * vec4(inPosition, 1.0);
    outPosition = gl_Position;
    outWorldPosition = ubo.W * vec4(inPosition, 1.0);
    outTexCoord = inTexCoord;
    outNormal = inNormal;
}
