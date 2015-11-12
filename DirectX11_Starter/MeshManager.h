#pragma once
#include <unordered_map>
#include <string>
#include "Mesh.h"
#include "CylinderCollider.h"
#include "ModelLoading.h"

namespace MeshManager
{
	void SetDevice(ID3D11Device *device);
	Mesh *LoadModel(std::string path);
	CylinderCollider *GetColliderForMesh(Mesh *);
};