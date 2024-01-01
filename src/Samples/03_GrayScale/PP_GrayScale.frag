#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    float postProcessLerp;
} ubo;

layout(binding = 1)
uniform CommonUniformBuffer
{
    mat4 WVP;
    mat4 W;
    mat4 V;
    mat4 P;
    float time;
} commonUBO;

layout(binding = 2) uniform sampler2D inSampler;

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 tex = texture(inSampler, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    float luminance = tex.r * 0.2126 + tex.g * 0.7152 + tex.b * 0.0722;
    vec4 grayScale = vec4(luminance, luminance, luminance, 1.0);
    outColor = mix(tex, grayScale, ubo.postProcessLerp);
}
