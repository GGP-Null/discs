#include "MaterialManager.h"
#include <vector>

namespace {

	SimpleVertexShader *vertShader;
	SimplePixelShader *pixShader;

	ID3D11Device *device;

	ID3D11BlendState *standardTransparency = nullptr;

	// TODO: there's probably a better way of storing these
	// TODO: is there tho
	std::vector<Material *> materials;
}


void MaterialManager::SetDevice(ID3D11Device *pdevice)
{
	::device = pdevice;
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
}