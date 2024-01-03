#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    vec3 lightDirection;
    float diffuseIntensity;

    vec3 baseColor;
    float diffusePower;
    
    vec3 cameraPos;
    float specularIntensity;
    
    float specularPower;
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
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inPosition;
layout(location = 3) in vec4 inWorldPosition;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBiTangent;
layout(location = 6) in vec3 inLightDirection;
layout(location = 7) in vec3 inCameraPos;

layout(location = 0) out vec4 outColor;


void main()
{
    vec4 tex = texture(inSampler, vec2(inTexCoord.x, 1.0 - inTexCoord.y)) * 2.0 - 1.0;
    vec3 normal = normalize(inTangent * tex.r + inBiTangent * tex.g + inNormal * tex.b);

    // diffuse
    float diffuse = (dot(normal, normalize(ubo.lightDirection)) + 1.0) / 2.0;
    diffuse = pow(diffuse, ubo.diffusePower) * ubo.diffuseIntensity;

    // specular
    vec3 reflection = -normalize(ubo.lightDirection + 2.0 * dot(normal, ubo.lightDirection) * normal);
    vec3 viewDirection = normalize(inWorldPosition.xyz - ubo.cameraPos);
    float specular = (dot(reflection, viewDirection)+ 1.0 ) / 2.0;
    specular = pow(specular, ubo.specularPower) * ubo.specularIntensity;

    outColor = vec4(ubo.baseColor + vec3(diffuse + specular), 1.0);
}
