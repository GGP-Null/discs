#pragma once
#include <d3d11.h>
#include "PostProcess.h"
#include "SimpleShader.h"

class TestPostProcess : public PostProcess
{
public:
	TestPostProcess(ID3D11Device* device, ID3D11DeviceContext* context);

	virtual void Do(ID3D11ShaderResourceView* render, ID3D11RenderTargetView* backbuffer);

	~TestPostProcess();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	SimpleVertexShader* vs;
	SimplePixelShader* ps;
	ID3D11SamplerState* sampler;
};

