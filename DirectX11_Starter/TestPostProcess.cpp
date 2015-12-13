#include "TestPostProcess.h"
#include "hr.h"


TestPostProcess::TestPostProcess(ID3D11Device* dev, ID3D11DeviceContext* con)
{
	device = dev;
	context = con;

	vs = new SimpleVertexShader(dev, con);
	vs->LoadShaderFile(L"PostVS.cso");

	ps = new SimplePixelShader(dev, con);
	ps->LoadShaderFile(L"TestPostPS.cso");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &sampler));
}

void TestPostProcess::Do(ID3D11ShaderResourceView * render, ID3D11RenderTargetView * backbuffer)
{
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	context->OMSetRenderTargets(1, &backbuffer, nullptr);
	context->ClearRenderTargetView(backbuffer, color);

	vs->SetShader();

	ps->SetShaderResourceView("render", render);
	ps->SetSamplerState("trilinear", sampler);
	ps->SetShader();

	context->Draw(3, 0);
}


TestPostProcess::~TestPostProcess()
{
	if (vs) delete vs;
	if (ps) delete ps;
	if (sampler) sampler->Release();
}
