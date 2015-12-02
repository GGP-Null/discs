#include "MaterialManager.h"
#include <vector>

SimpleVertexShader *vertShader;
SimplePixelShader *pixShader;

// TODO: there's probably a better way of storing these
std::vector<Material *> materials;

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

void MaterialManager::DestroyAllMaterials()
{
	for (auto mat : materials) delete mat;

	materials.clear();
}