#pragma once
#include <d3d11.h>
#include "PostProcess.h"
#include "SimpleShader.h"

class BloomPost :
	public PostProcess
{
public:
	BloomPost(ID3D11Device* dev, ID3D11DeviceContext* con, int windowWidth, int windowHeight);

	virtual void Do(ID3D11ShaderResourceView* render, ID3D11RenderTargetView* backbuffer);

	~BloomPost();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
	float pixelWidth;
	float pixelHeight;

	SimpleVertexShader* vs;
	SimplePixelShader* blurPixelShader;
	SimplePixelShader* bloomPixelShader;
	ID3D11SamplerState* sampler;
};

