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

    vec2 uv_ur = inTexCoord + vec2(pixelOffset.x * ubo.scale, pixelOffset.y * ubo.scale);
    vec2 uv_ul = inTexCoord + vec2(-pixelOffset.x * ubo.scale, pixelOffset.y * ubo.scale);
    vec2 uv_dr = inTexCoord + vec2(pixelOffset.x * ubo.scale, -pixelOffset.y * ubo.scale);
    vec2 uv_dl = inTexCoord + vec2(-pixelOffset.x * ubo.scale, -pixelOffset.y * ubo.scale);

    vec4 ur = texture(inSampler, uv_ur);
    vec4 ul = texture(inSampler, uv_ul);
    vec4 dr = texture(inSampler, uv_dr);
    vec4 dl = texture(inSampler, uv_dl);
    vec4 center = texture(inSampler, inTexCoord);

    vec4 final = (ur + ul + dr + dl) / 8.0 + center / 2.0;
    outColor = final;
}