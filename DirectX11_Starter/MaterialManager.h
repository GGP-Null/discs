#pragma once
#include "SimpleShader.h"
#include "Material.h"

namespace MaterialManager
{
	void SetStandardVertexShader(SimpleVertexShader *shader);
	void SetStandardPixelShader(SimplePixelShader *shader);

	Material *CloneStandardMaterial();

	void DestroyAllMaterials();
};