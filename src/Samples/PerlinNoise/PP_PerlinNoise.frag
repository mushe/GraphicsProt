#version 450

layout(binding = 0)
uniform CustomUniformBuffer
{
    float scale;
    float speed;
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

layout(location = 0) out vec4 outColor;

float rand(vec2 p)
{
    return fract(sin(dot(p.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 rand3D(vec3 p) 
{
    return fract
	(
		sin
		(
			vec3
			(
				dot(p, vec3(127.1, 311.7, 542.3)),
				dot(p, vec3(269.5, 183.3, 461.7)), 
				dot(p, vec3(732.1, 845.3, 231.7))
			)
		)
		* 43758.5453123
	);
}

float PerlinNoise3D(vec3 p)
{
	vec3 i = floor(p);
	vec3 f = fract(p);

	vec3 p000 = i + vec3(0.0, 0.0, 0.0);
	vec3 p001 = i + vec3(0.0, 0.0, 1.0);
	vec3 p010 = i + vec3(0.0, 1.0, 0.0);
	vec3 p011 = i + vec3(0.0, 1.0, 1.0);
	vec3 p100 = i + vec3(1.0, 0.0, 0.0);
	vec3 p101 = i + vec3(1.0, 0.0, 1.0);
	vec3 p110 = i + vec3(1.0, 1.0, 0.0);
	vec3 p111 = i + vec3(1.0, 1.0, 1.0);

	vec3 f000 = f - vec3(0.0, 0.0, 0.0);
	vec3 f001 = f - vec3(0.0, 0.0, 1.0);
	vec3 f010 = f - vec3(0.0, 1.0, 0.0);
	vec3 f011 = f - vec3(0.0, 1.0, 1.0);
	vec3 f100 = f - vec3(1.0, 0.0, 0.0);
	vec3 f101 = f - vec3(1.0, 0.0, 1.0);
	vec3 f110 = f - vec3(1.0, 1.0, 0.0);
	vec3 f111 = f - vec3(1.0, 1.0, 1.0);

	vec3 g000 = normalize(rand3D(p000) * 2 - 1);
	vec3 g001 = normalize(rand3D(p001) * 2 - 1);
	vec3 g010 = normalize(rand3D(p010) * 2 - 1);
	vec3 g011 = normalize(rand3D(p011) * 2 - 1);
	vec3 g100 = normalize(rand3D(p100) * 2 - 1);
	vec3 g101 = normalize(rand3D(p101) * 2 - 1);
	vec3 g110 = normalize(rand3D(p110) * 2 - 1);
	vec3 g111 = normalize(rand3D(p111) * 2 - 1);

	float d000 = dot(g000, f000);
	float d001 = dot(g001, f001);
	float d010 = dot(g010, f010);
	float d011 = dot(g011, f011);
	float d100 = dot(g100, f100);
	float d101 = dot(g101, f101);
	float d110 = dot(g110, f110);
	float d111 = dot(g111, f111);

	vec3 u = smoothstep(0.0, 1.0, f); 

	float d000_100 = mix(d000, d100, u.x);
	float d010_110 = mix(d010, d110, u.x);
	float d001_101 = mix(d001, d101, u.x);
	float d011_111 = mix(d011, d111, u.x);

	float d000_100_010_110 = mix(d000_100, d010_110, u.y);
	float d001_101_011_111 = mix(d001_101, d011_111, u.y);

	return mix(d000_100_010_110, d001_101_011_111, u.z) * 0.5 + 0.5;
}


void main()
{
    vec4 tex = texture(inSampler, vec2(1.0 - inTexCoord.x, inTexCoord.y));

    vec3 pIn = vec3(inTexCoord.xy * ubo.scale, commonUBO.time * ubo.speed);
    float p = PerlinNoise3D(pIn);
    
    outColor = vec4(p, p, p, 1.0);
}
