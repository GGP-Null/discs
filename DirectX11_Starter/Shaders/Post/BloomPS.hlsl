
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D blurRender    : register(t1);
Texture2D render		: register(t0);
SamplerState trilinear	: register(s0);


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 blurSample = blurRender.Sample(trilinear, input.uv);
	blurSample = blurSample * blurSample;

	return render.Sample(trilinear, input.uv) + blurSample;
}