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
	ID3D11ShaderResourceView* GlowResourceView;
	ID3D11SamplerState* SamplerState;
	ID3D11RasterizerState* RasterizerState;
	ID3D11BlendState* BlendState;
	float transparency;

	Material() :
		VertexShader(nullptr), PixelShader(nullptr),
		ResourceView(nullptr), SamplerState(nullptr),
		RasterizerState(nullptr), BlendState(nullptr)
	{}

	Material(SimpleVertexShader *vertShader, SimplePixelShader *pixShader) :
		VertexShader(vertShader), PixelShader(pixShader),
		ResourceView(nullptr), SamplerState(nullptr),
		RasterizerState(nullptr), BlendState(nullptr)
	{}

	bool IsTransparent()
	{
		return BlendState;
	}
};