#pragma once
#include "SimpleShader.h"
#include "Material.h"

namespace MaterialManager
{
	void SetDevice(ID3D11Device *context);

	void SetStandardVertexShader(SimpleVertexShader *shader);
	void SetStandardPixelShader(SimplePixelShader *shader);

	Material *CloneStandardMaterial();

	Material *GetSingleTransparentMaterial();

	void DestroyAllMaterials();
};