#include "BloomPost.h"
#include "hr.h"



BloomPost::BloomPost(ID3D11Device* dev, ID3D11DeviceContext* con, int windowWidth, int windowHeight)
{
	device = dev;
	context = con;

	pixelHeight = 1.0f / windowHeight;
	pixelWidth = 1.0f / windowWidth;

	vs = new SimpleVertexShader(dev, con);
	vs->LoadShaderFile(L"PostVS.cso");

	blurPixelShader = new SimplePixelShader(dev, con);
	blurPixelShader ->LoadShaderFile(L"BlurPS.cso");

	bloomPixelShader = new SimplePixelShader(dev, con);
	bloomPixelShader->LoadShaderFile(L"BloomPS.cso");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &sampler));

	// Create post-process resources
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = windowWidth;
	textureDesc.Height = windowHeight;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D* ppTexture;
	device->CreateTexture2D(&textureDesc, 0, &ppTexture);

	// Create the Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(ppTexture, &rtvDesc, &rtv);

	// Create the Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	device->CreateShaderResourceView(ppTexture, &srvDesc, &srv);

	// We don't need the texture reference no mo'
	ppTexture->Release();
}

void BloomPost::Do(ID3D11ShaderResourceView * render, ID3D11RenderTargetView * backbuffer)
{
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	//Blur PS
	context->OMSetRenderTargets(1, &rtv, nullptr);
	context->ClearRenderTargetView(rtv, color);

	vs->SetShader();

	blurPixelShader->SetShaderResourceView("render", render);
	blurPixelShader->SetSamplerState("trilinear", sampler);

	blurPixelShader->SetFloat("pixelWidth", pixelWidth);
	blurPixelShader->SetFloat("pixelHeight", pixelHeight);
	blurPixelShader->SetInt("blurAmount", 10);

	blurPixelShader->SetShader();

	context->Draw(3, 0);

	blurPixelShader->SetShaderResourceView("render", nullptr);


	//Bloom PS
	
	context->OMSetRenderTargets(1, &backbuffer, nullptr);
	context->ClearRenderTargetView(backbuffer, color);

	vs->SetShader();

	bloomPixelShader->SetShaderResourceView("render", render);
	bloomPixelShader->SetShaderResourceView("blurRender", srv);

	bloomPixelShader->SetSamplerState("trilinear", sampler);
	bloomPixelShader->SetShader();

	context->Draw(3, 0);



}


BloomPost::~BloomPost()
{
}
