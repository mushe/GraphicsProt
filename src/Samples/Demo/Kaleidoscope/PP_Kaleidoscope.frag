#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    float division;
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


float PI = 3.14159274;

vec2 kaleidoUV(vec2 uv)
{
    float radScale = 2.0 * PI / ubo.division;
    // 0,0  1,0
    // |     |
    // 0,1  1,1
    // to
    // 1,1  1,1
    // |     |
    // 1,1  1,1
    vec2 normalizedUV = (uv - vec2(0.5, 0.5)) * 2.0;
    
    // polar conversion
    float r = distance(normalizedUV, vec2(0,0));
    float theta = atan(normalizedUV.y, normalizedUV.x) + PI;
    theta = mod(theta, radScale);

    vec2 uvFromRTheta = vec2(r * cos(theta), r * sin(theta));
    return uvFromRTheta;
}

void main()
{
    vec2 kaleidoCoord = kaleidoUV(inTexCoord);
    vec4 tex = texture(inSampler, vec2(1.0 - kaleidoCoord.x, kaleidoCoord.y));
    outColor = tex;
}
