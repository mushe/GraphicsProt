#version 450

layout(binding = 1) uniform sampler2D inSampler;

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inLightDirection;
layout(location = 4) in vec3 inCameraPos;
layout(location = 5) in vec3 inPosition;

layout(location = 6) in float inDiffusePower;
layout(location = 7) in float inDiffuseMultiply;

layout(location = 8) in float inSpecularPower;
layout(location = 9) in float inSpecularMultiply;

layout(location = 10) in vec3 inWaveColor;
layout(location = 11) in vec3 inLightColor;


layout(location = 0) out vec4 outColor;

void main()
{
    //outColor = texture(inSampler, inTexCoord);
    //if(outColor.a < 0.01) discard;
    
    float diffuse = (dot(normalize(inNormal), normalize(inLightDirection)) + 1.0) / 2.0;
    diffuse = ( diffuse + 1.0 ) / 2.0;
    
    vec3 reflection = -normalize(inLightDirection + 2.0 * dot(inNormal, inLightDirection) * inNormal);
    vec3 viewDirection = normalize(inPosition - inCameraPos);
    float specular = dot(reflection, viewDirection);
    specular = ( specular + 1.0 ) / 2.0;
    
    
    float c = pow(diffuse, inDiffusePower) * inDiffuseMultiply + pow(specular,inSpecularPower) * inSpecularMultiply;
    vec3 col = inWaveColor + inLightColor * c;

    
    float alpha = 0.88;
    
    //float l = clamp((-inPosition.z)/2.3, 0.0, 1.0);
    
    float l = -inPosition.z + 1.65;
    l = pow(l, 1.5);
    
    
    float xThresh = 0.18;
    if(reflection.b > 0.06 && inPosition.x > -xThresh && inPosition.x < xThresh )
    {
        float r = reflection.b * (xThresh-abs(inPosition.x));
        col += vec3(8.5, 14.5, 0.0) * r;
    }
    
    outColor = vec4(col, l);
    //outColor = vec4(1,1,1,1);
    
}
