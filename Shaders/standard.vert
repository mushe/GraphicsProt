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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBiTangent;
layout(location = 5) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outPosition;
layout(location = 3) out vec4 outWorldPosition;
layout(location = 4) out vec3 outTangent;
layout(location = 5) out vec3 outBiTangent;


void main()
{
    gl_Position = commonUBO.WVP * vec4(inPosition, 1.0);
    outPosition = gl_Position;

    outTexCoord = inTexCoord;
    outNormal = inNormal;
    outTangent = inTangent;
    outBiTangent = inBiTangent;
}
