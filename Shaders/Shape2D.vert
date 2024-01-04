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

struct InstancingParameter
{
    vec2 position;
    vec2 scale;
};

layout(binding = 2)
uniform InstancingUniformBufferObject
{
    InstancingParameter params[1];
} instancingUBO;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBiTangent;
layout(location = 5) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec4 outColor;
layout(location = 2) out int outShapeType;


void main()
{
    int id = gl_InstanceIndex;
    int x = id % 10;
    int y = id / 10;

    vec2 pos = vec2(0.1 * x + 0.05, 0.1 * y + 0.05);
    vec2 scale = vec2(1.0, 1.0) * 0.09;

    pos = instancingUBO.params[id].position;
    scale = instancingUBO.params[id].scale;


    // 0 -> 1 to -1 -> 1
    pos = pos * 2.0 - 1.0;

    gl_Position = vec4(vec3(inPosition.x * scale.x, inPosition.y * scale.y, 0) + vec3(pos.x, pos.y ,0), 1.0);

    outColor = vec4(0.0, 0.3, 0.4, 1);
    outTexCoord = inTexCoord;
    outShapeType = 0;
}
