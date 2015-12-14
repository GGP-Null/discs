#pragma once
#include <string>
#include "SimpleShader.h"
#include "Material.h"

namespace MaterialManager
{
	void SetStandardVertexShader(SimpleVertexShader *shader);
	void SetStandardPixelShader(SimplePixelShader *shader);

	ID3D11ShaderResourceView *LoadWICTextureFromFile(const std::wstring &path);
	ID3D11ShaderResourceView *LoadDDSTextureFromFile(const std::wstring &path);

	ID3D11SamplerState *GetStandardSamplerState();

	ID3D11RasterizerState *GetStandardRasterizerState();
	ID3D11RasterizerState *GetWireframeRasterizerState();
	ID3D11RasterizerState *GetSkyboxRasterizerState();

	ID3D11DepthStencilState *GetSkyboxDepthStencilState();

	Material *CloneStandardMaterial();

	Material *CloneStandardTransparentMaterial();

	void DestroyAllMaterials();
};