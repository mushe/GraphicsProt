#version 450

float neighborOffs = 0.001;

layout(binding = 0)
uniform CustomUniformBuffer
{
    float scale;
    float leapThreshold;
    float leapScale;
    float leapSpeed;
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


float getDepth(vec2 uv)
{
	float depth = 1.0 - texture(inSampler, uv).r;
	depth *= 400.0;
	return depth;
}

void main()
{
    float depth = getDepth(inTexCoord);
    outColor = vec4(depth);
}