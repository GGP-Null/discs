#pragma once

#include "SimpleShader.h"

//temporary struct; just for storing shaders until we have a 
//real material class
//TODO:  More full material class
struct Material
{
	SimpleVertexShader* VertexShader;
	SimplePixelShader* PixelShader;
	ID3D11ShaderResourceView* ResourceView;
	ID3D11SamplerState* SamplerState;
	ID3D11RasterizerState* RasterizerState;

	~Material()
	{
		if (ResourceView) ResourceView->Release();
		if (SamplerState) SamplerState->Release();
	}
};