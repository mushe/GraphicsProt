#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    float luminancePower;
    float luminanceMultiply;
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

float rand(vec2 p)
{
    return fract(sin(dot(p.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec4 tex = texture(inSampler, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    float c = rand(inTexCoord + tex.rb);

    float luminance = tex.r * 0.2126 + tex.g * 0.7152 + tex.b * 0.0722;
    luminance = pow(luminance, ubo.luminancePower) * ubo.luminanceMultiply;

    if(c > luminance) c = 1.0;
    else c = 0.0;

    outColor = vec4(c, c, c, 1.0);
}
