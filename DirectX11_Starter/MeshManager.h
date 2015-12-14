#pragma once
#include <unordered_map>
#include <string>
#include "Mesh.h"
#include "CylinderCollider.h"
#include "ModelLoading.h"

namespace MeshManager
{
	Mesh *LoadModel(std::string path, bool invertNormals = false);
	CylinderCollider *GetColliderForMesh(Mesh *);
	void DestroyAllMeshes();
	VerticesAndIndices LoadVertsAndIndices(const char *filename, bool invertNormals = false);
};