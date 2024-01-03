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
    mat4 VP;
    mat4 W;
    mat4 V;
    mat4 P;
    float time;
} commonUBO;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec4 inColor;
layout(location = 2) flat in int inShapeType;

layout(location = 0) out vec4 outColor;


void main()
{
    vec4 color = vec4(0);
    
    // quad
    if(inShapeType == 0)
    {
        color = inColor;
    }
    // circle
	else if(inShapeType == 1)
	{
        float l = length(inTexCoord - vec2(0.5, 0.5));
        if(l < 0.5)
		{
			color = vec4(inTexCoord.x, inTexCoord.y, 0, 1);
		}
	}
    
    outColor = color;
}
