#version 450

float neighborOffs = 0.02;
vec3 lightDir = normalize(vec3(0.9, -0.8, 0.3));

layout(binding = 0)
uniform CustomUniformBuffer
{
    vec4 ColorA;
    vec4 ColorB;
    float diffusePower;
    float diffuseMultiply;
    float specularPower;
    float specularMultiply;
    float floorColorBlendPower;
    float floorColorBlendMultiply;
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
layout(binding = 4) uniform sampler2D inSamplerFloor;
layout(binding = 5) uniform sampler2D inSamplerAdditiveFluid;

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;


vec3 depthToNormal(float depth)
{
		float dr = texture(inSampler, inTexCoord + vec2(neighborOffs, 0.0)).r;
		float dl = texture(inSampler, inTexCoord + vec2(-neighborOffs, 0.0)).r;
		float du = texture(inSampler, inTexCoord + vec2(0.0, neighborOffs)).r;
		float dd = texture(inSampler, inTexCoord + vec2(0.0, -neighborOffs)).r;

		float drdl = (dr - dl) * 1000.0;
		float dudd = (du - dd) * 1000.0;

		vec3 dir = vec3(-drdl, -dudd, 1.0);
		vec3 normal = normalize(dir);
		return normal;
}


void main()
{
		// diffuse and specular
		float depth = texture(inSampler, vec2(inTexCoord.x, inTexCoord.y)).r;
		vec4 fluidColor = vec4(0.0);
		if(depth > 0.0001)
		{
			// estimate normal from depth
			vec3 normal = depthToNormal(depth);

			// diffuse
			float diff = dot(normal, lightDir);
			diff = max(diff, 0.0);
			diff = pow(diff, ubo.diffusePower) * ubo.diffuseMultiply;

			// specular(fixed view)
			vec3 viewDir = normalize(vec3(-0.3, -2.0, 0.5));
			vec3 R = lightDir + 2.0 * dot(normal, lightDir) * normal;
			float spec = max(dot(R, viewDir), 0.0);
			spec = pow(spec, ubo.specularPower) * ubo.specularMultiply;

			float l = diff + spec;
			fluidColor = vec4(l, l, l, 1.0);

		}
		outColor = fluidColor;


		// additive fluid color
		float additive = texture(inSamplerAdditiveFluid, vec2(1.0-inTexCoord.x, inTexCoord.y)).b;
		vec4 waterColor = mix(ubo.ColorA, ubo.ColorB, additive) * additive;
		outColor = waterColor + outColor;


		// floor
		vec4 floorTex = texture(inSamplerFloor, vec2(1.0-inTexCoord.x, inTexCoord.y));
		float blend = pow(additive, ubo.floorColorBlendPower) * ubo.floorColorBlendMultiply;
		blend = clamp(blend, 0.0, 1.0);
		outColor = mix(floorTex, outColor,blend);

}