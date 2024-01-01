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
    float time;
} commonUBO;

layout(binding = 2) uniform sampler2D inSampler;
layout(binding = 3) uniform sampler2D inSampler2;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inPosition;
layout(location = 3) in vec4 inWorldPosition;

layout(location = 0) out vec4 outColor;

float remap(float x, float in0, float in1, float out0, float out1) {
    return out0 + (out1 - out0) * (x - in0) / (in1 - in0);
}

void main()
{

    vec4 tex = texture(inSampler, inTexCoord);


    //float l = dot(normalize(inNormal), normalize(ubo.lightDir)) * 0.5 + 0.5;
    //l *= ubo.lightIntensity;
    //outColor = vec4(l,l,l, 1.0) + vec4(0.1,0.1,0.1,0.0);

    tex = texture(inSampler, vec2(inTexCoord.x, 1.0 - inTexCoord.y));
    outColor = tex;


    /*
    vec4 p1 = ubo.shadowVP * inWorldPosition;
    p1 /= p1.w;
    float d1 = p1.z;

    vec2 depthCoord = vec2(p1.x, p1.y) * 0.5 + vec2(0.5, 0.5);
    float d2 = texture(inSampler, depthCoord).r;
    if(d1 - d2 > 0.00001 && d2 > 0.0001)
	{
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
    */
}
