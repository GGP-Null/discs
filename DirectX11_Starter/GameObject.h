#pragma once

#include <DirectXMath.h>
#include <memory>
#include <string>
#include "Mesh.h"
#include "SimpleShader.h"
#include "FrameUpdateData.h"

using namespace DirectX;
using namespace std;

class GameObject
{
protected:
	XMFLOAT4X4 worldMat;

	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	float distFromCamera;
	
	bool worldMatIsDirty;

	Mesh *mesh;
	Material *material;

	string debugName;

	void updateWorldMatrix();
	void init();

public:
	GameObject(Mesh* mesh, Material* material);

	Material* GetMaterial();
	Mesh* GetMesh();

	//getters for worldmat elements
	XMFLOAT3 GetTranslation() const;
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();

	float GetDistFromCamera() const;

	string const &GetDebugName();

	//setters for worldmat elements
	void SetTranslation(XMFLOAT3 translation);
	void SetRotation(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);

	void SetDistFromCamera(float dist);

	void SetDebugName(string const &str);

	//helpers
	void Translate(XMFLOAT3 translation);
	void Rotate(XMFLOAT3 rotation);
	void Scale(XMFLOAT3 scale);

	virtual void Update(FrameUpdateData);

	virtual void Draw(ID3D11DeviceContext* context);
};
