#include "MaterialManager.h"
#include <vector>
#include <unordered_map>
#include "WICTextureLoader.h"
#include "hr.h"
#include "Globals.h"

using namespace std;
using namespace DirectX;
using Globals::device;
using Globals::deviceContext;

namespace {
	SimpleVertexShader *vertShader;
	SimplePixelShader *pixShader;

	ID3D11BlendState *standardTransparency = nullptr;
	ID3D11SamplerState *standardSampler = nullptr;

	ID3D11RasterizerState 
		*standardRastState = nullptr,
		*wireframeRastState = nullptr,
		*skyboxRastState = nullptr
		;

	ID3D11DepthStencilState *skyboxDepthStencilState = nullptr;

	// TODO: there's probably a better way of storing these
	// TODO: is there tho
	std::vector<Material *> materials;

	unordered_map<wstring, ID3D11ShaderResourceView*> textures;
}


void MaterialManager::SetStandardVertexShader(SimpleVertexShader *shader)
{
	vertShader = shader;
}

void MaterialManager::SetStandardPixelShader(SimplePixelShader *shader)
{
	pixShader = shader;
}

static Material *makeStandardMaterial()
{
	auto mat = new Material;

	mat->VertexShader = vertShader;
	mat->PixelShader = pixShader;

	return mat;
}

Material *MaterialManager::CloneStandardMaterial()
{
	auto mat = makeStandardMaterial();
	materials.push_back(mat);

	return mat;
}

static void makeStandardTransparentBlendState()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = true;

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&blendDesc, &standardTransparency);
}

Material *MaterialManager::CloneStandardTransparentMaterial()
{
	auto transparentMaterial = CloneStandardMaterial();

	if (!standardTransparency) makeStandardTransparentBlendState();

	transparentMaterial->BlendState = standardTransparency;
	
	return transparentMaterial;
}

void MaterialManager::DestroyAllMaterials()
{
	for (auto mat : materials) delete mat;
	materials.clear();

	for (auto tex : textures) tex.second->Release();
	textures.clear();

	if (standardSampler) standardSampler->Release();
	if (standardTransparency) standardTransparency->Release();
	if (standardRastState) standardRastState->Release();
	if (wireframeRastState) wireframeRastState->Release();
	if (skyboxRastState) skyboxRastState->Release();
	if (skyboxDepthStencilState) skyboxDepthStencilState->Release();
}

ID3D11ShaderResourceView *MaterialManager::LoadTextureFromFile(const wstring &path)
{
	auto srv = textures.find(path);
	if (srv != textures.end()) return srv->second;

	ID3D11ShaderResourceView *ptr;
	HR(CreateWICTextureFromFile(device, deviceContext, path.data(), nullptr, &ptr));
	textures.emplace(path, ptr);

	return ptr;
}

ID3D11SamplerState *MaterialManager::GetStandardSamplerState()
{
	if (standardSampler) return standardSampler;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &standardSampler));

	return standardSampler;
}

ID3D11RasterizerState *MaterialManager::GetStandardRasterizerState()
{
	if (standardRastState) return standardRastState;

	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&solidDesc, &standardRastState));

	return standardRastState;
}

ID3D11RasterizerState *MaterialManager::GetWireframeRasterizerState()
{
	if (wireframeRastState) return wireframeRastState;

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&wireframeDesc, &wireframeRastState);

	return wireframeRastState;
}

ID3D11RasterizerState *MaterialManager::GetSkyboxRasterizerState()
{
	if (skyboxRastState) return skyboxRastState;

	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rastDesc, &skyboxRastState);

	return skyboxRastState;
}

ID3D11DepthStencilState *MaterialManager::GetSkyboxDepthStencilState()
{
	if (skyboxDepthStencilState) return skyboxDepthStencilState;

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&dsDesc, &skyboxDepthStencilState);

	return skyboxDepthStencilState;
}
