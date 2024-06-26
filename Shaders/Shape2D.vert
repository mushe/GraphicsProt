#version 450
#define PI 3.1415926538

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

struct InstancingParameter
{
    vec3 position;
    int shapeType;
    vec3 scale;
    float directionTheta;
    vec4 color;
    float ascii;
    float dummy1;
    float dummy2;
    float dummy3;
};

layout(std140, binding = 2)
buffer InstancingShaderStorageBufferObject
{
    InstancingParameter params[1];
} instancingSSBO;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBiTangent;
layout(location = 5) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec4 outColor;
layout(location = 2) out int outShapeType;
layout(location = 3) out float outAscii;


void main()
{
    int id = gl_InstanceIndex;
    vec2 pos = instancingSSBO.params[id].position.xy;
    vec2 scale = instancingSSBO.params[id].scale.xy;
    vec2 texCoord = inTexCoord;
    
    // rect, circle
    if(instancingSSBO.params[id].shapeType == 0 || instancingSSBO.params[id].shapeType == 1)
    {
        // 0 -> 1 to -1 -> 1
        pos = pos * 2.0 - 1.0;
        gl_Position = vec4(vec3(inPosition.x * scale.x, inPosition.y * scale.y, 0) + vec3(pos.x, pos.y ,0), 1.0);
    }
    // line
    else if (instancingSSBO.params[id].shapeType == 2)
    {
        vec2 startPos = pos;
        vec2 endPos = scale; // for memory saving
        vec2 centerPos = (startPos + endPos) - 1.0;

        float width = instancingSSBO.params[id].scale.z; // for memory saving
        vec2 diff = endPos - startPos;
        float height = length(diff);

        float theta = atan(diff.y, diff.x) - PI/2.0;
        float x = inPosition.x * width;
        float y = inPosition.y * height;
        float xRotated = x * cos(theta) - y * sin(theta);
        float yRotated = x * sin(theta) + y * cos(theta);

        gl_Position = vec4(vec3(xRotated, yRotated, 0) + vec3(centerPos.x, centerPos.y ,0), 1.0);
    }
    // triangle
    else if(instancingSSBO.params[id].shapeType == 3)
    {
        vec3 vertexPos = inPosition;
        if(gl_VertexIndex == 0) vertexPos = vec3(-0.6, -1.0, 0.0);
        if(gl_VertexIndex == 1) vertexPos = vec3(0.6, -1.0, 0.0);
        if(gl_VertexIndex == 2) vertexPos = vec3(0.0, 1.0, 0.0);
        if(gl_VertexIndex == 3) vertexPos = vec3(0.0, 1.0, 0.0);
        float x = vertexPos.x * scale.x;
        float y = vertexPos.y * scale.y;
        float xRotated = x * cos(instancingSSBO.params[id].directionTheta) - y * sin(instancingSSBO.params[id].directionTheta);
        float yRotated = x * sin(instancingSSBO.params[id].directionTheta) + y * cos(instancingSSBO.params[id].directionTheta);

        // 0 -> 1 to -1 -> 1
        pos = pos * 2.0 - 1.0;
        gl_Position = vec4(vec3(xRotated, yRotated, 0) + vec3(pos.x, pos.y ,0), 1.0);
    }
    // text
    else if(instancingSSBO.params[id].shapeType == 4)
    {
        // 0 -> 1 to -1 -> 1
        pos = pos * 2.0 - 1.0;
        gl_Position = vec4(vec3(inPosition.x * scale.x, inPosition.y * scale.y, 0) + vec3(pos.x, pos.y ,0), 1.0);
    }

    outColor = instancingSSBO.params[id].color;
    outTexCoord = texCoord;
    outShapeType = instancingSSBO.params[id].shapeType;
    outAscii = instancingSSBO.params[id].ascii;
}
