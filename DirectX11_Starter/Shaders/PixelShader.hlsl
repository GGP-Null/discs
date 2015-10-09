
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
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer lightBuffer : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
}

Texture2D diffuseTexture : register(t0);
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

	float4 output = float4(0, 0, 0, 0);
	float3 directionToLight = normalize(-light.Direction);
	float diffuseAmount = saturate(dot(directionToLight, input.normal));
	
	output += surfaceColor * (light.DiffuseColor * diffuseAmount + light.AmbientColor);

	directionToLight = normalize(-light2.Direction);
	diffuseAmount = saturate(dot(directionToLight, input.normal));

	
	output += surfaceColor * (light2.DiffuseColor * diffuseAmount + light2.AmbientColor);

	return output;
}