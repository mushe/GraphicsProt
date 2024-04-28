#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    float distance;
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

void main()
{
    float r = texture(inSampler, vec2(1.0 - inTexCoord.x + ubo.distance, inTexCoord.y)).r;
    float g = texture(inSampler, vec2(1.0 - inTexCoord.x, inTexCoord.y)).g;
    float b = texture(inSampler, vec2(1.0 - inTexCoord.x - ubo.distance, inTexCoord.y)).b;
    vec4 col = vec4(r, g, b, 1.0);
    outColor = col;
}
