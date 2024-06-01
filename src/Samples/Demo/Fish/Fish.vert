#version 450

float FISH_SCALE = 0.015;

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

struct InstancingParameter
{
    vec3 pos;
    vec3 vel;
    float density;
    float pressure;
};

layout(binding = 2)
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
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outPosition;
layout(location = 3) out vec4 outWorldPosition;
layout(location = 4) out vec3 outTangent;
layout(location = 5) out vec3 outBiTangent;


// q_1q_2  =  (w_1w_2 - v_1・v_2, w_1v_2 + w_2v_1  + v_1×v_2)
vec4 multiplyQuaternion(vec4 a, vec4 b)
{
    float w1 = a.x;
    float w2 = b.x;
    vec3 v1 = a.yzw;
    vec3 v2 = b.yzw;

    return vec4
    (
        w1*w2 - dot(v1, v2),
        w1*v2 + w2*v1 + cross(v1, v2)
    );
}

// q = (v, v)
// q^-1 = (w,-v)
vec4 invertQuaternion(vec4 q)
{
    return vec4(q.x, -q.y, -q.z, -q.w);
}
// p´ = qpq^{-1}
vec3 rotate(vec3 pos, vec4 q)
{
    vec4 qp = multiplyQuaternion(q, vec4(0, pos));
    vec4 q_inv = invertQuaternion(q);
    vec4 qpq_inv = multiplyQuaternion(qp, q_inv);
    return qpq_inv.yzw;
}

vec4 fromToQuaternion(vec3 p1, vec3 p2)
{
    p1 = normalize(p1);
    p2 = normalize(p2);
    vec3 n = cross(p1, p2);
    float co = dot(p1, p2);
    return normalize( vec4(1+co, n) );
}

vec4 lookAtQuaternion(vec3 eyePos, vec3 targetPos)
{
    return fromToQuaternion(vec3(0,1,0), targetPos - eyePos);
}


void main()
{
    int id = gl_InstanceIndex;
    vec4 q = lookAtQuaternion(vec3(0,0,0), instancingSSBO.params[id].vel);
    vec3 vertPos = vec3(inPosition.y, inPosition.x, inPosition.z * 0.25) * FISH_SCALE;
    vec3 objPos = instancingSSBO.params[id].pos + vec3(-0.5);
    gl_Position = commonUBO.P * commonUBO.V  * vec4(rotate(vertPos, q) + objPos, 1.0);
    
    outPosition = gl_Position;

    vec4 world = (commonUBO.W * vec4(gl_Position.xyz, 1.0));
    outWorldPosition = world / world.w;

    outTexCoord = inTexCoord;

    vec4 normal = commonUBO.W * vec4(inNormal, 1.0);
    normal = normalize(normal / normal.w);
    outNormal = normal.xyz;

    vec4 tangent = commonUBO.W * vec4(inTangent, 1.0);
    tangent = normalize(tangent / tangent.w);
    outTangent = tangent.xyz;

    vec4 bitangent = commonUBO.W * vec4(inBiTangent, 1.0);
    bitangent = normalize(bitangent / bitangent.w);
    outBiTangent = bitangent.xyz;
}
