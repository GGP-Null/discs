#pragma once
#include <string>
#include "SimpleShader.h"
#include "Material.h"

namespace MaterialManager
{
	void SetStandardVertexShader(SimpleVertexShader *shader);
	void SetStandardPixelShader(SimplePixelShader *shader);

	ID3D11ShaderResourceView *LoadTextureFromFile(const std::wstring &path);

	ID3D11SamplerState *GetStandardSamplerState();

	Material *CloneStandardMaterial();

	Material *CloneStandardTransparentMaterial();

	void DestroyAllMaterials();
};