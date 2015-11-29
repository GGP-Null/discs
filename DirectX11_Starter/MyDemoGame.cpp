// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.h"
#include <iostream>
#include "ModelLoading.h"
#include "CylinderCollider.h"
#include "WICTextureLoader.h"
#include "MeshManager.h"

// For the DirectX Math library
using namespace DirectX;
using namespace Input;
using Input::GamePad;


#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the game object.
	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance), gamePad(gamePad.FromId(0))
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"My Super Fancy GGP Game";

	// Custom window size - will be created by Init() later
	windowWidth = 800;
	windowHeight = 600;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	//It may be a good idea to stick to this order of destruction,
	//which is the opposite order from construction.  Just for consistentcy's
	//sake, and in case of any strange issues cropping up.  

	//It's also worth mentioning that I'm not checking most of these objects
	//to see if they exist, because in this simplified demo we know that they
	//will.  Good practice dictates that we should check the existence of
	//these things before deleting them in the full project.
	delete renderer;

	delete debugCamera;
	delete trackingCamera;

	delete mat;
	delete mesh;
	delete discMesh;

	delete object;

	delete vertexShader;
	delete pixelShader;

	//delete wireframeRS;
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	CreateGeometry();
	LoadShaders();
	CreateObjects();

	debugCamera = new DebugCamera(XMFLOAT3(0, 0, -5), XMFLOAT3(0, 0, 1), aspectRatio);
	trackingCamera = new TrackingCamera(XMFLOAT3(0, 2, -7), XMFLOAT3(0, 0, 1), object, XMFLOAT3(0, 1, 0), .3f, aspectRatio);
	useDebugCamera = false;

	//TODO:  set up these lights in the correct places
	renderer = new Renderer(debugCamera, deviceContext);

	DirectionalLight testLight = {
		XMFLOAT4(0.0f, 0.4f, 0.0f, 1.0f),
		XMFLOAT3(1, -1, 1)
	};

	DirectionalLight testLight2 = {
		XMFLOAT4(0.0f, 0.0f, 0.7f, 1.0f),
		XMFLOAT3(-1, -0.3f, 0)
	};

	LightManager* lm = renderer->GetLightManager();
	lm->SetDirectionalLight(0, &testLight);
	lm->SetDirectionalLight(1, &testLight2);
	lm->SetAmbientLight(&XMFLOAT4(0.3f, 0, 0, 1));

	lm->UpdateLights(0.1f);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//set the gamestate
	gState = MAIN;

	//initialize render states
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&wireframeDesc, &wireframeRS);

	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_SOLID;
	wireframeDesc.CullMode = D3D11_CULL_FRONT;
	wireframeDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&solidDesc, &solidRS);

	// setup mouse mode

	Input::SetMouseMode(Input::MouseMode::MODE_RELATIVE);

	// Successfully initialized
	return true;
}

// --------------------------------------------------------
// TODO:  Create 'mesh manager' or equivalent.
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	MeshManager::SetDevice(device);

	mesh = MeshManager::LoadModel("../Resources/playerOne.obj");
	p2Mesh = MeshManager::LoadModel("../Resources/playerTwo.obj");

	arenaMesh = MeshManager::LoadModel("../Resources/cube.obj");

	discMesh = MeshManager::LoadModel("../Resources/dotDisc.obj");
	platformMesh = MeshManager::LoadModel("../Resources/dotPlatform.obj");
}

// --------------------------------------------------------
// TODO:  Have renderer manage what shaders are loaded
// TODO:  Create 'technique' class for sets of shaders (for renderer)
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

// --------------------------------------------------------
// This could probably be replaced by the state machine.
// --------------------------------------------------------
void MyDemoGame::CreateObjects()
{
	mat = new Material;
	arenaMat = new Material;

	mat->VertexShader = vertexShader;
	mat->PixelShader = pixelShader;
	arenaMat->VertexShader = vertexShader;
	arenaMat->PixelShader = pixelShader;

	HR(CreateWICTextureFromFile(device, L"../Resources/blueGlow.jpg", nullptr, &mat->ResourceView));
	HR(CreateWICTextureFromFile(device, L"../Resources/white.jpg", nullptr, &arenaMat->ResourceView));

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&samplerDesc, &mat->SamplerState));

	object = new Player(mesh, mat);
	for (auto &disc : discs) disc = new Disc(discMesh, mat, object);
	player2 = new Player(p2Mesh, mat);
	for (auto &disc : p2discs) disc = new Disc(discMesh, mat, player2);
	object->Scale(XMFLOAT3(.1f, .1f, .1f));
	player2->Scale(XMFLOAT3(.1f, .1f, .1f));
	player2->Translate(XMFLOAT3(0, 0.0, 12.0));


	arena = new GameObject(arenaMesh, mat);
	p1Platform = new GameObject(platformMesh, mat);
	p2Platform = new GameObject(platformMesh, mat);

	/*
	object->SetRotation(XMFLOAT3(0, 90.0f, 0));
	object->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	*/

	arena->SetScale(XMFLOAT3(7, 7, 17));
	arena->SetTranslation(XMFLOAT3(0, 0, 6));

	p1Platform->SetScale(XMFLOAT3(0.5, 0.5, 0.5));
	p2Platform->SetScale(XMFLOAT3(0.5, 0.5, 0.5));

	p1Platform->Translate(XMFLOAT3(0, -0.2, 0.0));
	p2Platform->Translate(XMFLOAT3(0, -0.2, 12.0));
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();
	
	if (debugCamera)
	{
		debugCamera->CreateProjMatrix(aspectRatio);
	}
	if (trackingCamera)
	{
		trackingCamera->CreateProjMatrix(aspectRatio);
	}
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	auto &padState = gamePad.GetState();
	auto &trackedPadState = gamePad.GetTrackedState();

	if (KeyPressedThisFrame(Keys::Escape) ||
		gamePad.ButtonPressedThisFrame(trackedPadState.back)
		)
		Quit();


	switch (gState) {
	case GAME:
		if (KeyPressedThisFrame(Keys::Q) || gamePad.ButtonPressedThisFrame(trackedPadState.start))
			EndGame();
		// shouldn't get here anywho
		break;

	case MAIN:
		if (KeyPressedThisFrame(Keys::Enter) || gamePad.ButtonPressedThisFrame(trackedPadState.start))
			StartGame();

		break;
	}

	debugCamera->Update(deltaTime, totalTime);
	trackingCamera->Update(deltaTime, totalTime);

	if (KeyPressedThisFrame(Keys::T))
		useDebugCamera = false;
	else if (KeyPressedThisFrame(Keys::Y))
		useDebugCamera = true;

	//this stuff is here to demonstrate the flow for working with a dynamic point light
	PointLight pl;
	pl.DiffuseColor = { 1, 1, 1, 1 };
	pl.Location = { debugCamera->GetPosition().x, debugCamera->GetPosition().y, debugCamera->GetPosition().z };
	renderer->GetLightManager()->SetPointLight(0, &pl);
	//we shouldn't call the next method unless we actually changed the location of the light,
	//but I got a bit lazy here since we're really not actually going to need this later.
	//see what happens if you comment it out!
	renderer->GetLightManager()->UpdateLights(totalTime);

	switch (gState) {
	case GAME:
		if (KeyIsDown(Keys::J)) object->Translate(XMFLOAT3(-deltaTime, 0, 0));
		else if (KeyIsDown(Keys::K)) object->Translate(XMFLOAT3(deltaTime, 0, 0));
		if (KeyIsDown(Keys::U)) object->Rotate(XMFLOAT3(0, -deltaTime, 0));
		else if (KeyIsDown(Keys::I)) object->Rotate(XMFLOAT3(0, deltaTime, 0));

		if (padState.IsConnected()) {
			object->Translate(XMFLOAT3(padState.thumbSticks.leftX * deltaTime, 0, 0));
			float rotAmt = padState.triggers.right - padState.triggers.left;
			object->Rotate(XMFLOAT3(0, rotAmt * deltaTime, 0));
		}

		if (KeyPressedThisFrame(Keys::Space) || gamePad.ButtonPressedThisFrame(trackedPadState.a))
		{
			Disc* toUse = DiscToLaunch();
			if(toUse) 
				object->Fire(toUse);
		}
		else
		{
			object->ReloadDisc();
		}

		CylinderCollider playerCollider = object->colliderComp;

		static float collisionTimer = 0.0f;

		collisionTimer += deltaTime;

		for (unsigned int i = 0; i < discs.size(); ++i)
		{
			auto &disc = discs[i];

			if (!disc->IsActive()) continue;

			disc->MoveDisc(deltaTime);

			CylinderCollider discCollider = disc->colliderComp;
			auto colliding = IsColliding(playerCollider, discCollider);

			if (colliding) discIsColliding[i] = colliding;
		}

		if (collisionTimer > 1.0f)
		{
			std::wstringstream newCaption;
			for (auto &colliding : discIsColliding)
			{
				newCaption << '[' << ((colliding) ? 'x' : ' ') << ']';
				colliding = false;
			}

			windowCaption = newCaption.str();
			collisionTimer = 0.0f;
		}

	}
}
void MyDemoGame::StartGame()
{
	gState = GAME;
}
void MyDemoGame::EndGame()
{
	gState = MAIN;
}
// ----------------------------------------------------------
// Temporary Test to figure out how to launch different Discs
// ----------------------------------------------------------
Disc* MyDemoGame::DiscToLaunch()
{
	for (auto &disc : discs)
		if (!disc->IsActive()) return disc;

	return NULL;
}
// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// use correct camera
	renderer->SetCamera(useDebugCamera ? (Camera*)debugCamera : (Camera*)trackingCamera);


	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	//is the game going
	if (gState == GAME)
	{

		//Drawing is done simply by asking the renderer to do so.
		deviceContext->RSSetState(solidRS);
		renderer->DrawObject(object);
		renderer->DrawObject(player2);
		renderer->DrawObject(p1Platform);
		renderer->DrawObject(p2Platform);

		for (auto &disc : discs) renderer->DrawObject(disc);
		for (auto &disc : p2discs) renderer->DrawObject(disc);

		deviceContext->RSSetState(wireframeRS);
		renderer->DrawObject(arena);

	}
	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion



#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion
