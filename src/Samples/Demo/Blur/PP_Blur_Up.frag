#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    vec2 resolution;
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

layout(binding = 3) uniform sampler2D inSampler;

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;


void main()
{
    vec2 pixelOffset = vec2(1.0 / ubo.resolution.x, 1.0 / ubo.resolution.y);

    vec2 uv_r = inTexCoord + vec2(pixelOffset.x * ubo.scale * 2.0, 0.0);
    vec2 uv_l = inTexCoord + vec2(-pixelOffset.x * ubo.scale * 2.0, 0.0);
    vec2 uv_u = inTexCoord + vec2(0.0, pixelOffset.y * ubo.scale * 2.0);
    vec2 uv_d = inTexCoord + vec2(0.0, -pixelOffset.y * ubo.scale * 2.0);

    vec2 uv_ur = inTexCoord + vec2(pixelOffset.x * ubo.scale, pixelOffset.y * ubo.scale);
    vec2 uv_ul = inTexCoord + vec2(-pixelOffset.x * ubo.scale, pixelOffset.y * ubo.scale);
    vec2 uv_dr = inTexCoord + vec2(pixelOffset.x * ubo.scale, -pixelOffset.y * ubo.scale);
    vec2 uv_dl = inTexCoord + vec2(-pixelOffset.x * ubo.scale, -pixelOffset.y * ubo.scale);

    vec4 ur = texture(inSampler, uv_ur);
    vec4 ul = texture(inSampler, uv_ul);
    vec4 dr = texture(inSampler, uv_dr);
    vec4 dl = texture(inSampler, uv_dl);
    vec4 u = texture(inSampler, uv_u);
    vec4 d = texture(inSampler, uv_d);
    vec4 r = texture(inSampler, uv_r);
    vec4 l = texture(inSampler, uv_l);

    vec4 final = (u + d + r + l) / 6.0 + (ur + ul + dr + dl) / 12.0;
    outColor = final;
}