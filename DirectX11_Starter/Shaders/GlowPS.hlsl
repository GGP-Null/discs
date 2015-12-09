
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 positionInWorld : TEXCOORD1;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct DirectionalLight
{
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float4 DiffuseColor;
	float3 Location;
};

cbuffer lightBuffer : register(b0)
{
	float4 ambientColor;
	DirectionalLight dLight0;
	DirectionalLight dLight1;
	DirectionalLight dLight2;
	DirectionalLight dLight3;
	DirectionalLight dLight4;
	DirectionalLight dLight5;
	DirectionalLight dLight6;
	DirectionalLight dLight7;
	PointLight pLight0;
	PointLight pLight1;
	PointLight pLight2;
	PointLight pLight3;
	PointLight pLight4;
	PointLight pLight5;
	PointLight pLight6;
	PointLight pLight7;
}

cbuffer materialData : register(b1)
{
	float1 alpha;
}

Texture2D diffuseTexture : register(t0);
Texture2D glowMap : register(t1);
SamplerState basicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	float glowIntensity = glowMap.Sample(basicSampler, input.uv).x;

	float4 output = surfaceColor * ambientColor;
	float3 directionToLight;
	float diffuseAmount;
 
	//START DIRECTIONAL LIGHTS
	directionToLight = normalize(-dLight0.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight0.DiffuseColor * diffuseAmount;

	directionToLight = normalize(-dLight1.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight1.DiffuseColor * diffuseAmount;

	directionToLight = normalize(-dLight2.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight2.DiffuseColor * diffuseAmount;

	directionToLight = normalize(-dLight3.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight3.DiffuseColor * diffuseAmount;

	directionToLight = normalize(-dLight4.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight4.DiffuseColor * diffuseAmount;

	directionToLight = normalize(-dLight5.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight5.DiffuseColor * diffuseAmount;

	directionToLight = normalize(-dLight6.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight6.DiffuseColor * diffuseAmount;

	directionToLight = normalize(-dLight7.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * dLight7.DiffuseColor * diffuseAmount;

	//START POINT LIGHTS
	directionToLight = normalize(pLight0.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight0.DiffuseColor * diffuseAmount;

	directionToLight = normalize(pLight1.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight1.DiffuseColor * diffuseAmount;

	directionToLight = normalize(pLight2.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight2.DiffuseColor * diffuseAmount;

	directionToLight = normalize(pLight3.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight3.DiffuseColor * diffuseAmount;

	directionToLight = normalize(pLight4.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight4.DiffuseColor * diffuseAmount;

	directionToLight = normalize(pLight5.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight5.DiffuseColor * diffuseAmount;

	directionToLight = normalize(pLight6.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight6.DiffuseColor * diffuseAmount;

	directionToLight = normalize(pLight7.Location - input.positionInWorld);
	diffuseAmount = saturate(dot(directionToLight, input.normal));
	output += surfaceColor * pLight7.DiffuseColor * diffuseAmount;

	//add glow map result
	output += surfaceColor * glowIntensity;

	return float4(output.rgb, alpha);
}