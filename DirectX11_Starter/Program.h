#pragma once
#include "GameObject.h"
#include "CylinderColliderComponent.h"

class Program : public GameObject
{
public:
	Program(Mesh* m, Material* mat);
	~Program();

	CylinderColliderComponent colliderComp;
};