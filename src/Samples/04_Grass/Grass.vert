#version 450

layout(binding = 0)
uniform UniformBufferObject
{
    float noiseSpeed;
    float noiseScale;
	float noiseTimeOffset;
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
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBiTangent;
layout(location = 5) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outPosition;
layout(location = 3) out vec4 outWorldPosition;

vec3 Rand3D(vec3 p) 
{
    return fract(sin(vec3(dot(p, vec3(127.1, 311.7, 542.3)),dot(p, vec3(269.5, 183.3, 461.7)), dot(p, vec3(732.1, 845.3, 231.7)))) * 43758.5453123 );
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

	vec3 g000 = normalize(Rand3D(p000) * 2 - 1);
	vec3 g001 = normalize(Rand3D(p001) * 2 - 1);
	vec3 g010 = normalize(Rand3D(p010) * 2 - 1);
	vec3 g011 = normalize(Rand3D(p011) * 2 - 1);
	vec3 g100 = normalize(Rand3D(p100) * 2 - 1);
	vec3 g101 = normalize(Rand3D(p101) * 2 - 1);
	vec3 g110 = normalize(Rand3D(p110) * 2 - 1);
	vec3 g111 = normalize(Rand3D(p111) * 2 - 1);

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

vec3 PerlinNoise3Dto3D(vec3 p)
{
	float rx = PerlinNoise3D(p);
	float ry = PerlinNoise3D(p + vec3(3234.244, 1823.832, 8222.443));
	float rz = PerlinNoise3D(p + vec3(-999.951, 9560.669, 2322.241));
	return vec3(rx, ry, rz);
}


void main()
{
	vec3 noise = PerlinNoise3Dto3D(inPosition + vec3(commonUBO.time * ubo.noiseSpeed + ubo.noiseTimeOffset)) - vec3(0.5);
	vec3 posOffs = noise * ubo.noiseScale * inPosition.y;
	posOffs.y = 0.0;
    gl_Position = commonUBO.WVP * vec4(inPosition + posOffs, 1.0);
    outPosition = gl_Position;

    outTexCoord = inTexCoord;
    outNormal = inNormal;
}
