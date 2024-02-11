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

void main()
{
    vec4 tex = texture(inSampler, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    float r = 0.393 * tex.r + 0.769 * tex.g + 0.189 * tex.b;
    float g = 0.349 * tex.r + 0.686 * tex.g + 0.168 * tex.b;
    float b = 0.272 * tex.r + 0.534 * tex.g + 0.131 * tex.b;

    vec4 sepiaColor = vec4(r, g, b, 1.0);
    outColor = mix(tex, sepiaColor, ubo.postProcessLerp);
}
