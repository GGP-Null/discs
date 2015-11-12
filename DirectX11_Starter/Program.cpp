#include "Program.h"

#include "MeshManager.h"

Program::Program(Mesh *mesh, Material *mat)
	: GameObject(mesh, mat),
	colliderComp(this, MeshManager::GetColliderForMesh(mesh))
{

}


Program::~Program()
{
}
