#version 450

layout(binding = 0)
uniform UniformBufferObject
{
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
layout(binding = 4) uniform sampler2D inSampler2;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inPosition;
layout(location = 3) in vec4 inWorldPosition;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBiTangent;
layout(location = 6) in vec4 inColor;
layout(location = 7) in flat int inType;

layout(location = 0) out vec4 outColor;


void main()
{
    vec4 col = inColor;
    vec4 tex = texture(inSampler, inTexCoord);

    // wall
    if(inType == 0) col = tex;
    // floor
    else if(inType == 1) col = vec4(tex.rgb * 0.5, 1.0);
    
    outColor = col;
}
