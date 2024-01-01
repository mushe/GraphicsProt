#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    mat4 world;
    mat4 view;
    mat4 proj;
    vec3 lightDirection;
    vec3 cameraPos;
    
    float diffusePower;
    float diffuseMultiply;
    float specularPower;
    float specularMultiply;
    
    vec3 waveColor;
    vec3 lightColor;
    
    float time;
    float waveHeight_1;
    float waveHeight_2;
    float waveHeight_3;
    
    float waveHeight_4;
    float waveHeight_5;
    float wavePosScale_1;
    float wavePosScale_2;
    
    float wavePosScale_3;
    float wavePosScale_4;
    float wavePosScale_5;
    float waveSpeed_1;
    
    float waveSpeed_2;
    float waveSpeed_3;
    float waveSpeed_4;
    float waveSpeed_5;
    
    vec2 waveDirection_1;
    vec2 waveDirection_2;
    
    vec2 waveDirection_3;
    vec2 waveDirection_4;
    
    vec2 waveDirection_5;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outFragColor;
layout(location = 1) out vec2 outFragTexCoord;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outLightDirection;
layout(location = 4) out vec3 outCameraPos;
layout(location = 5) out vec3 outPosition;

layout(location = 6) out float outDiffusePower;
layout(location = 7) out float outDiffuseMultiply;

layout(location = 8) out float outSpecularPower;
layout(location = 9) out float outSpecularMultiply;

layout(location = 10) out vec3 outWaveColor;
layout(location = 11) out vec3 outLightColor;

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


vec3 gerstnerWave(vec3 p)
{
    float d = dot(vec2(p.x, p.z), ubo.waveDirection_1);
    vec3 posOffs = vec3(0.0, sin(ubo.time.x * ubo.waveSpeed_1 + d * ubo.wavePosScale_1) * ubo.waveHeight_1, 0.0) ;
     
    d = d = dot(vec2(p.x, p.z), ubo.waveDirection_2);
    posOffs += vec3(0.0, sin(ubo.time.x * ubo.waveSpeed_2 + d * ubo.wavePosScale_2) * ubo.waveHeight_2, 0.0) ;
    
    
    d = d = dot(vec2(p.x, p.z), ubo.waveDirection_3);
    posOffs += vec3(0.0, sin(ubo.time.x * ubo.waveSpeed_3 + d * ubo.wavePosScale_3) * ubo.waveHeight_3, 0.0) ;
    
    d = d = dot(vec2(p.x, p.z), ubo.waveDirection_4);
    posOffs += vec3(0.0, sin(ubo.time.x * ubo.waveSpeed_4 + d * ubo.wavePosScale_4) * ubo.waveHeight_4, 0.0) ;
    
    d = d = dot(vec2(p.x, p.z), ubo.waveDirection_5);
    posOffs += vec3(0.0, sin(ubo.time.x * ubo.waveSpeed_5 + d * ubo.wavePosScale_5) * ubo.waveHeight_5, 0.0) ;
     
    
    return posOffs;
}


void main()
{
    vec3 p = inPosition;
    vec3 posOffs = gerstnerWave(p);
    
    float delta = 0.001;
    vec3 dx = gerstnerWave(p + vec3(delta, 0.0, 0.0)) + vec3(delta, 0.0, 0.0);
    vec3 dz = gerstnerWave(p + vec3(0.0, 0.0, delta)) + vec3(0.0, 0.0, delta);
    vec3 normal = normalize(cross(normalize(dx-posOffs), normalize(dz-posOffs)));
    
    vec4 posw = ubo.world * vec4(inPosition + posOffs, 1.0);
    gl_Position = ubo.proj * ubo.view * posw;
    //gl_Position = ubo.proj * ubo.view * ubo.world * vec4(inPosition, 1.0);
    
    outNormal = normal;
    outFragColor = vec3(1,0,1);
    outFragTexCoord = inTexCoord;
    outLightDirection = ubo.lightDirection;
    outCameraPos = (ubo.world * vec4(ubo.cameraPos, 1.0)).xyz;
    outPosition = posw.xyz;
    
    outDiffusePower = ubo.diffusePower;
    outDiffuseMultiply = ubo.diffuseMultiply;
    outSpecularPower = ubo.specularPower;
    outSpecularMultiply = ubo.specularMultiply;
    outWaveColor = ubo.waveColor;
    outLightColor = ubo.lightColor;
}
