#include "MeshManager.h"
#include <fstream>
#include <functional>
#include "Globals.h"

using namespace std;
using namespace MeshManager;
using namespace DirectX;
using Globals::device;

typedef function<XMFLOAT3(XMFLOAT3)> vectorTransformer;

struct meshAndCollider
{
	Mesh *mesh;
	CylinderCollider *collider;
};

unordered_map<string, meshAndCollider> regularModels;
unordered_map<string, meshAndCollider> invertedModels;
unordered_map<Mesh*, CylinderCollider*> meshToCollider;

Mesh *MeshManager::LoadModel(std::string path, bool invertNormals)
{
	auto &modelCollection = (invertNormals) ? invertedModels : regularModels;
	auto meshAndCol = modelCollection.find(path);

	// found
	if (meshAndCol != modelCollection.end())
		return meshAndCol->second.mesh;

	auto vertsAndIndices = LoadVertsAndIndices(path.data(), invertNormals);

	auto *mesh = new Mesh(vertsAndIndices, device);

	auto *cylinderCol = new CylinderCollider(CylinderCollider::Make(vertsAndIndices.vertices));

	meshToCollider.emplace(mesh, cylinderCol);

	auto emplaceResult = modelCollection.emplace(path, meshAndCollider{ mesh, cylinderCol });

	return mesh;
}

CylinderCollider *MeshManager::GetColliderForMesh(Mesh *mesh)
{
	auto collider = meshToCollider.find(mesh);

	if (collider == meshToCollider.end()) return nullptr;

	return collider->second;
}

VerticesAndIndices MeshManager::LoadVertsAndIndices(const char *filename, bool invertNormals)
{
	vectorTransformer normalTrans;
	if (invertNormals) {
		normalTrans = [](XMFLOAT3 normal) -> XMFLOAT3 { return XMFLOAT3(-normal.x, -normal.y, -normal.z); };
	} else {
		normalTrans = [](XMFLOAT3 normal) -> XMFLOAT3 { return normal; };
	};
	
	std::ifstream obj(filename);

	if (!obj.is_open()) {
		std::string msg("Couldn't open file ");
		msg += filename;
		throw std::exception(msg.c_str());
	}

	// Variables used while reading the file
	std::vector<XMFLOAT3> positions;     // Positions from the file
	std::vector<XMFLOAT3> normals;       // Normals from the file
	std::vector<XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int triangleCounter = 0;    // Count of triangles/indices
	char chars[100];                     // String for line reading

	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(normalTrans(norm));
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uv.y = 1.0f - uv.y;
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the 9 face indices into an array
			unsigned int i[9];
			sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.Uv = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.Uv = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.Uv = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// Add the verts to the vector
			if (invertNormals)
			{
				verts.push_back(v2);
				verts.push_back(v1);
				verts.push_back(v3);
			}
			else
			{
				verts.push_back(v1);
				verts.push_back(v2);
				verts.push_back(v3);
			}

			// Add three more indices
			indices.push_back(triangleCounter++);
			indices.push_back(triangleCounter++);
			indices.push_back(triangleCounter++);
		}
	}

	// Close (shouldn't be necessary?)
	obj.close();

	return{ verts, indices };
}

void MeshManager::DestroyAllMeshes()
{
	for (auto &mnc : regularModels) {
		delete mnc.second.collider;
		delete mnc.second.mesh;
	}
	for (auto &mnc : invertedModels) {
		delete mnc.second.collider;
		delete mnc.second.mesh;
	}

	regularModels.clear();
	invertedModels.clear();
	meshToCollider.clear();
}
