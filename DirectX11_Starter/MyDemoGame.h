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
#include "TestPostProcess.h"
#include "Console.h"
#include "Skybox.h"
#include "BloomPost.h"
#include "UIDraw.h"

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
	enum gameState {MAIN, GAME, INSTRUCT, CREDITS };
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
	Disc* DiscToLaunch(int i);
private:
	int menuChoice;
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
	void Menu();
	void Instruct();
	void Credit();

	Console console;

	Mesh *mesh;
	Mesh* p2Mesh;
	Mesh* discMesh;
	Mesh* platformMesh;
	Mesh *arenaMesh;
	Material *p1mat;
	Material *p2mat;
	Material *mat;
	Material* matWireframe;
	Material* matTrans;
	Material* matTransWhite;
	Material* discMat;
	Material* platformMat;
	UIDraw* menuDrawer;

	CylinderCollider cyl_col;

	Player* player;
	Player* player2;
	std::array<Disc*, 3> discs;
	std::array<Disc*, 3> p2discs;
	std::array<bool, 3> discIsColliding;
	GameObject* arena;
	GameObject* p1Platform;
	GameObject* p2Platform;

	Skybox* sky;

	DebugCamera* debugCamera;
	TrackingCamera* trackingCamera;
	bool useDebugCamera;

	Input::GamePad gamePad;

	Renderer* renderer;
	gameState gState;
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* glowPixelShader;

	ID3D11RasterizerState* wireframeRS;
	ID3D11RasterizerState* solidRS;
	ID3D11RasterizerState* transRS;

	BloomPost* post;
};
