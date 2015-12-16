#include "Skybox.h"



Skybox::Skybox(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	skyMesh = MeshManager::LoadModel("../Resources/cube.obj");

	skyVS = new SimpleVertexShader(device, deviceContext);
	skyVS->LoadShaderFile(L"SkyVS.cso");

	skyPS = new SimplePixelShader(device, deviceContext);
	skyPS->LoadShaderFile(L"SkyPS.cso");
	skyTexture = MaterialManager::LoadDDSTextureFromFile(L"../Resources/SunnyCubeMap.dds");

	rsSky = MaterialManager::GetSkyboxRasterizerState();

	// A depth state for the sky rendering
	dsSky = MaterialManager::GetSkyboxDepthStencilState();
}


Skybox::~Skybox()
{
	delete skyVS;
	delete skyPS;
}
