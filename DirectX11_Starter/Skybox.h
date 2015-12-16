#pragma once
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "MaterialManager.h"
class Skybox
{
public:
	Skybox(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Skybox();

	Mesh* skyMesh;

	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;

	// Render states for the sky
	ID3D11RasterizerState* rsSky;
	ID3D11DepthStencilState* dsSky;

	ID3D11ShaderResourceView* skyTexture;
};

