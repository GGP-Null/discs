#pragma once

#include <DirectXMath.h>
#include "Mesh.h"
#include "SimpleShader.h"

using namespace DirectX;

//temporary struct; just for storing shaders until we have a 
//real material class
//TODO:  More full material class
struct Material
{
	SimpleVertexShader* VertexShader;
	SimplePixelShader* PixelShader;
	ID3D11ShaderResourceView* ResourceView;
	ID3D11SamplerState* SamplerState;
};

class GameObject
{
protected:
	XMFLOAT4X4 worldMat;

	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	
	bool worldMatIsDirty;

	Mesh* mesh;
	//TODO: Make this a pointer to a material, for easier use in Renderer(?)
	Material material;

public:
	GameObject(Mesh* mesh, Material material);

	//getters for worldmat elements
	XMFLOAT3 GetTranslation();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();

	//setters for worldmat elements
	void SetTranslation(XMFLOAT3 translation);
	void SetRotation(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);

	//helpers
	void Translate(XMFLOAT3 translation);
	void Scale(XMFLOAT3 scale);

	Material GetMaterial();

	void Draw(ID3D11DeviceContext* context);

	~GameObject();
};

