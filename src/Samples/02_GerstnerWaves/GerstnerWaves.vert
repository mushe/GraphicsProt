#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    vec3 waveColor;
    float diffusePower;

    vec3 lightDirection;
    float diffuseMultiply;

    vec3 lightColor;
    float specularPower;

    vec3 cameraPos;
    float specularMultiply;

    float sunSpread;
    float sunRefrectionThreshold;
    float dummy1;
    float dummy2;

    vec3 sunColor;
    float sunIntensity;

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
    float dummy;

    vec2 waveDirection_1;
    vec2 waveDirection_2;
    
    vec2 waveDirection_3;
    vec2 waveDirection_4;
    
    vec2 waveDirection_5;
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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outPosition;


vec3 gerstnerWave(vec3 p)
{
    float d = dot(vec2(p.x, p.z), ubo.waveDirection_1);
    vec3 posOffs = vec3(0.0, sin(commonUBO.time.x * ubo.waveSpeed_1 + d * ubo.wavePosScale_1) * ubo.waveHeight_1, 0.0) ;
     
    d = dot(vec2(p.x, p.z), ubo.waveDirection_2);
    posOffs += vec3(0.0, sin(commonUBO.time.x * ubo.waveSpeed_2 + d * ubo.wavePosScale_2) * ubo.waveHeight_2, 0.0) ;
    
    d = dot(vec2(p.x, p.z), ubo.waveDirection_3);
    posOffs += vec3(0.0, sin(commonUBO.time.x * ubo.waveSpeed_3 + d * ubo.wavePosScale_3) * ubo.waveHeight_3, 0.0) ;
    
    d = dot(vec2(p.x, p.z), ubo.waveDirection_4);
    posOffs += vec3(0.0, sin(commonUBO.time.x * ubo.waveSpeed_4 + d * ubo.wavePosScale_4) * ubo.waveHeight_4, 0.0) ;
    
    d = dot(vec2(p.x, p.z), ubo.waveDirection_5);
    posOffs += vec3(0.0, sin(commonUBO.time.x * ubo.waveSpeed_5 + d * ubo.wavePosScale_5) * ubo.waveHeight_5, 0.0) ;
    
    return posOffs;
}

void main()
{
    vec3 posOffs = gerstnerWave(inPosition);
    gl_Position = commonUBO.WVP * vec4(inPosition + posOffs, 1.0);
    outPosition = gl_Position.xyz;

    outTexCoord = inTexCoord;

    float delta = 0.001;
    vec3 dx = gerstnerWave(inPosition + vec3(delta, 0.0, 0.0)) + vec3(delta, 0.0, 0.0);
    vec3 dz = gerstnerWave(inPosition + vec3(0.0, 0.0, delta)) + vec3(0.0, 0.0, delta);
    vec3 normal = normalize(cross(normalize(dx-posOffs), normalize(dz-posOffs)));
    outNormal = normal;
}
