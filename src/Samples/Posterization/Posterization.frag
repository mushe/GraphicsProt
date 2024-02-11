#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    float resolution;
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
    vec4 tex = texture(inSampler, vec2(inTexCoord.x, 1.0 - inTexCoord.y));
    vec3 col = tex.rgb - mod(tex.rgb, ubo.resolution);
    outColor = vec4(col.rgb, 1.0);
}
