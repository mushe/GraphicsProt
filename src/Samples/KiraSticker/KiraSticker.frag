#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    float division;
    float scrollSpeed;
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

float PI = 3.14159274;

void main()
{
    float scale = 1.0 / ubo.division;

    vec2 modCoord = mod(inTexCoord , scale) * ubo.division;
    vec2 normalizedCoord = (modCoord - vec2(0.5,0.5)) * 2.0;
    float rotatedCoordX = (atan(normalizedCoord.y, normalizedCoord.x) + PI) / ( 2.0 * PI );

    float ux = mod(rotatedCoordX + commonUBO.time * ubo.scrollSpeed, 1.0);
    ux = clamp(ux, 0.01, 0.99);
    vec4 tex = texture(inSampler, vec2(ux, 0.5));

    outColor = tex;
}
