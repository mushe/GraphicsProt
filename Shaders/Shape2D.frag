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

layout(binding = 3) uniform sampler2D inSampler;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec4 inColor;
layout(location = 2) flat in int inShapeType;
layout(location = 3) in float inAscii;

layout(location = 0) out vec4 outColor;


void main()
{
    vec4 color = vec4(0);
    
    // quad or triangle
    if(inShapeType == 0 || inShapeType == 3)
    {
        color = inColor;
    }
    // circle
	else if(inShapeType == 1)
	{
        float l = length(inTexCoord - vec2(0.5, 0.5));
        if(l < 0.5)
		{
			color = inColor;
		}
        else
			discard;
	}
    // line
	else if(inShapeType == 2)
	{
        color = inColor;
	}
     // text
	else if(inShapeType == 4)
	{
        float xScale = 0.030317;
        float yScale = 11.967 * 0.01;

        float xIndex = mod(inAscii, 32.0);
        float yIndex = int(inAscii / 32.0);
        float xOffs = 0.965 - xIndex * xScale;
        float yOffs = -0.089 + yIndex * yScale;

        vec4 tex = texture(inSampler, vec2(1.0 - inTexCoord.x * xScale - xOffs, inTexCoord.y * yScale + yOffs));

        if(tex.r > 0.5) discard;

        color = vec4(inColor.r, inColor.g, inColor.b,1);
	}
    
    outColor = color;
}
