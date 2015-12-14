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

	for (auto tex : textures) tex.second->Release();

	if (standardSampler) standardSampler->Release();
	if (standardTransparency) standardTransparency->Release();

	materials.clear();
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
