#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "LightManager.h"

// This class is intended to improve the efficiency of using multiple different
// shaders, etc. without having the developers worry about the order in which things
// will be drawn.  Currently, it is just a wrapper that sets the correct
// shaders, sets camera data, and calls Draw on the object.  In the future, it will queue
// draw calls so that objects using the same set of shaders will all be drawn in one batch, 
// minimizing the number of times we need to switch shaders. 

class Renderer
{
public:
	Renderer(Camera* camera, ID3D11DeviceContext* context);
	~Renderer();

	LightManager* GetLightManager();
	void SetCamera(Camera* camera);

	void SetNumberOfBuckets(unsigned int numBuckets);
	void StartFrame();
	void DrawObject(GameObject* object, unsigned int bucket);
	void EndFrame();
private:
	Camera* camera;
	ID3D11DeviceContext* context;

	LightManager* lm;
	float lightsLastUpdated;

	float numBuckets;
	vector<vector<GameObject*>> buckets;

	void doDraw(GameObject* obj);
};

