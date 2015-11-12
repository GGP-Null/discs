#pragma once

#include "GameObject.h"
#include "CylinderCollider.h"

class CylinderColliderComponent
{
public:
	CylinderColliderComponent() = default;
	CylinderColliderComponent(GameObject *go, CylinderCollider *col);

	void SetCollider(CylinderCollider *col);
	CylinderCollider *GetCollider();

	void SetGameObject(GameObject *go);
	GameObject *GetGameObject();

	operator CylinderCollider();
private:
	GameObject *go;
	CylinderCollider *collider;
};