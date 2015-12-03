#pragma once

#include <DirectXMath.h>
#include <memory>
#include <array>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Player.h"
#include "Disc.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "TrackingCamera.h"
#include "Renderer.h"
#include "DirectionalLight.h"
#include "WICTextureLoader.h"
#include "CylinderCollider.h"

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();
	enum gameState { START, MAIN, GAME };
	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime, float totalTime);
	void DrawScene(float deltaTime, float totalTime);

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	// Disc Testing
	Disc* DiscToLaunch();
private:
	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//-----------------------------------------------------------
	//NOTE:  Everything below is for testing/demo purposes only.  
	//This all should be removed/recycled when we move on to the 
	//full project.
	//-----------------------------------------------------------
	void CreateGeometry();
	void LoadShaders();
	void CreateObjects();
	void StartGame();
	void EndGame();

	Mesh *mesh;
	Mesh* p2Mesh;
	Mesh* discMesh;
	Mesh* platformMesh;
	Mesh *arenaMesh;
	Mesh* skyMesh;
	Material *p1mat;
	Material *p2mat;
	Material *mat;
	Material* matWireframe;
	Material* matTrans;
	Material* matTransWhite;
	Material* discMat;
	Material* platformMat;

	CylinderCollider cyl_col;

	Player* player;
	Player* player2;
	std::array<Disc*, 3> discs;
	std::array<Disc*, 3> p2discs;
	std::array<bool, 3> discIsColliding;
	GameObject* arena;
	GameObject* p1Platform;
	GameObject* p2Platform;

	DebugCamera* debugCamera;
	TrackingCamera* trackingCamera;
	bool useDebugCamera;

	Input::GamePad gamePad;

	Renderer* renderer;
	gameState gState;
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;

	ID3D11RasterizerState* wireframeRS;
	ID3D11RasterizerState* solidRS;
	ID3D11RasterizerState* transRS;

	// Render states for the sky
	ID3D11RasterizerState* rsSky;
	ID3D11DepthStencilState* dsSky;

	ID3D11ShaderResourceView* skyTexture;
};