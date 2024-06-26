#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    float scale;
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
    vec3 position;
    float scale;
};

layout(binding = 2)
buffer InstancingShaderStorageBufferObject
{
    InstancingParameter params[1];
} instancingSSBO;

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
    int id = gl_InstanceIndex;
    vec3 position = inPosition * instancingSSBO.params[id].scale * ubo.scale + instancingSSBO.params[id].position;
    
    gl_Position = commonUBO.WVP * vec4(position, 1.0);
    outPosition = gl_Position;

    vec4 world = (commonUBO.W * vec4(position, 1.0));
    outWorldPosition = world / world.w;

    outTexCoord = inTexCoord;

    vec4 normal = commonUBO.W * vec4(inNormal, 1.0);
    normal = normalize(normal / normal.w);
    outNormal = normal.xyz;

    vec4 tangent = commonUBO.W * vec4(inTangent, 1.0);
    tangent = normalize(tangent / tangent.w);
    outTangent = tangent.xyz;

    vec4 bitangent = commonUBO.W * vec4(inBiTangent, 1.0);
    bitangent = normalize(bitangent / bitangent.w);
    outBiTangent = bitangent.xyz;
}
