#include "MaterialManager.h"
#include <vector>

SimpleVertexShader *vertShader;
SimplePixelShader *pixShader;
ID3D11Device *device;

Material *transparentMaterial;

// TODO: there's probably a better way of storing these
// TODO: is there tho
std::vector<Material *> materials;

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

Material *makeStandardMaterial()
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

Material *MaterialManager::GetSingleTransparentMaterial()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	
}

void MaterialManager::DestroyAllMaterials()
{
	for (auto mat : materials) delete mat;

	materials.clear();
}