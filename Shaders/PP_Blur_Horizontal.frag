#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    vec2 resolution;
    float scale;
} customUBO;

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
layout(binding = 3) uniform sampler2D inSamplerDepth;

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;


float remap(float x, float in0, float in1, float out0, float out1) {
    return out0 + (out1 - out0) * (x - in0) / (in1 - in0);
}

void main()
{
    vec2 pixelOffset = vec2(1.0 / customUBO.resolution.x, 1.0 / customUBO.resolution.y);

    vec2 uv_rr = inTexCoord + vec2(pixelOffset.x * 2.0 * customUBO.scale, 0);
    vec2 uv_r = inTexCoord + vec2(pixelOffset.x * customUBO.scale, 0);
    vec2 uv_l = inTexCoord + vec2(-pixelOffset.x * customUBO.scale, 0);
    vec2 uv_ll = inTexCoord + vec2(-pixelOffset.x * 2.0 * customUBO.scale, 0);

    // 0.0219 + 0.2285 + 0.4992 + 0.2285 + 0.0219
    vec4 rrc = texture(inSampler, vec2 (1.0 - uv_rr.x, uv_rr.y));
    vec4 rc = texture(inSampler, vec2 (1.0 - uv_r.x, uv_r.y));
    vec4 c = texture(inSampler, vec2(1.0 - inTexCoord.x, inTexCoord.y));
    vec4 lc = texture(inSampler, vec2 (1.0 - uv_l.x, uv_l.y));
    vec4 llc = texture(inSampler, vec2 (1.0 - uv_ll.x, uv_ll.y));

    vec4 final = rrc * 0.0219 + rc * 0.2285 + c * 0.4992 + lc * 0.2285 + llc * 0.0219;
    //vec4 final = rrc * 0.0219 + rc * 0.2285 + lc * 0.2285 + llc * 0.0219;

    outColor = final;
}
