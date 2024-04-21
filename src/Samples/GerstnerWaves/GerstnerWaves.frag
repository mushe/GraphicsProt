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

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inPosition;

layout(location = 0) out vec4 outColor;

void main()
{
    // diffuse
    float diffuse = (dot(normalize(inNormal), normalize(ubo.lightDirection)) + 1.0) / 2.0;

    // specular
    vec3 reflection = -normalize(ubo.lightDirection + 2.0 * dot(inNormal, ubo.lightDirection) * inNormal);
    vec3 viewDirection = normalize(inPosition - ubo.cameraPos);
    float specular = dot(reflection, viewDirection);
    specular = ( specular + 1.0 ) / 2.0;
    
    float c = pow(diffuse, ubo.diffusePower) * ubo.diffuseMultiply + pow(specular, ubo.specularPower) * ubo.specularMultiply;
    vec3 col = ubo.waveColor + ubo.lightColor * c;

    // sun
    if(reflection.b > ubo.sunRefrectionThreshold && inPosition.x > -ubo.sunSpread && inPosition.x < ubo.sunSpread )
    {
        float r = reflection.b * (ubo.sunSpread-abs(inPosition.x));
        col += ubo.sunColor * ubo.sunIntensity * r;
    }

    outColor = vec4(col, 1);
}
