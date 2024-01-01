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
    mat4 W;
    mat4 V;
    mat4 P;
    float time;
} commonUBO;

layout(binding = 2) uniform sampler2D inSampler2;
layout(binding = 3) uniform sampler2D inSampler4;
layout(binding = 4) uniform sampler2D inSampler6;
layout(binding = 5) uniform sampler2D inSampler8;

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;


float remap(float x, float in0, float in1, float out0, float out1) {
    return out0 + (out1 - out0) * (x - in0) / (in1 - in0);
}

void main()
{
    vec4 tex2 = texture(inSampler2, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    vec4 tex4 = texture(inSampler4, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    vec4 tex6 = texture(inSampler6, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    vec4 tex8 = texture(inSampler8, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    outColor = (tex2 + tex4 + tex6 + tex8) / 4.0;
    outColor.a = 1.0;

    //float depth = texture(inSamplerDepth, vec2(1.0 - inTexCoord.x, inTexCoord.y)).r;
    //depth = remap(depth, 0.999, 1.0, 0.0, 1.0);
    //outColor = vec4(depth, depth, depth, 1.0);
}
