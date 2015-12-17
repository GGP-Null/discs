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
#include "DDSTextureLoader.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "Prototypes.h"
#include "Globals.h"


// For the DirectX Math library
using namespace DirectX;
using namespace Input;
using Input::GamePad;

#define SCROLL_WHEEL_TO_TRANSPARENCY (0.1f / 120.0f)


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
	delete post;
	delete renderer;

	delete debugCamera;
	delete trackingCamera;

	MaterialManager::DestroyAllMaterials();

	delete player;

	delete vertexShader;
	delete pixelShader;

	delete sky;

	MeshManager::DestroyAllMeshes();

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

	Globals::device = device;
	Globals::deviceContext = deviceContext;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	CreateGeometry();
	LoadShaders();

	//initialize render states
	wireframeRS = MaterialManager::GetWireframeRasterizerState();
	solidRS = MaterialManager::GetStandardRasterizerState();
	transRS = solidRS;

	sky = new Skybox(device, deviceContext);
	CreateObjects();

	debugCamera = new DebugCamera(XMFLOAT3(0, 0, -5), XMFLOAT3(0, 0, 1), aspectRatio);
	trackingCamera = new TrackingCamera(XMFLOAT3(0, 2, -7), XMFLOAT3(0, 0, 1), player, XMFLOAT3(0, 1, 0), .3f, aspectRatio);
	useDebugCamera = false;

	//TODO:  set up these lights in the correct places
	renderer = new Renderer(debugCamera, device, deviceContext);

	post = new BloomPost(device, deviceContext, windowWidth, windowHeight);
	renderer->SetPostProcess(post, windowWidth, windowHeight, depthStencilView, renderTargetView);

	DirectionalLight testLight = {
		XMFLOAT4(0.0f, 0.4f, 0.0f, 1.0f),
		XMFLOAT3(1, -1, 1)
	};

	DirectionalLight testLight2 = {
		XMFLOAT4(0.0f, 0.0f, 0.7f, 1.0f),
		XMFLOAT3(-1, -0.3f, 0)
	};

	LightManager* lm = renderer->GetLightManager();
	//lm->SetDirectionalLight(0, &testLight);
	//lm->SetDirectionalLight(1, &testLight2);
	lm->SetAmbientLight(&XMFLOAT4(0.3f, 0, 0, 1));

	lm->UpdateLights(0.1f);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//set the gamestate
	gState = MAIN;

	// setup mouse mode
	Input::SetMouseMode(Input::MouseMode::MODE_ABSOLUTE);
	Input::ResetScrollWheel();

	// Successfully initialized
	return true;
}

// --------------------------------------------------------
// TODO:  Create 'mesh manager' or equivalent.
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	mesh = MeshManager::LoadModel("../Resources/playerOne.obj");
	p2Mesh = MeshManager::LoadModel("../Resources/playerTwo.obj");

	arenaMesh = MeshManager::LoadModel("../Resources/cube.obj", true);

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

	glowPixelShader = new SimplePixelShader(device, deviceContext);
	glowPixelShader->LoadShaderFile(L"GlowPS.cso");

	MaterialManager::SetStandardVertexShader(vertexShader);
	MaterialManager::SetStandardPixelShader(pixelShader);
}

// --------------------------------------------------------
// This could probably be replaced by the state machine.
// --------------------------------------------------------
void MyDemoGame::CreateObjects()
{
	menuDrawer = new UIDraw(deviceContext, device);
	menuDrawer->center = DirectX::XMFLOAT2(windowWidth / 2, windowHeight / 2);
	p1mat = MaterialManager::CloneStandardMaterial();
	p1mat->PixelShader = glowPixelShader;

	p2mat = MaterialManager::CloneStandardMaterial();
	p2mat->PixelShader = glowPixelShader;

	mat = MaterialManager::CloneStandardMaterial();
	matWireframe = MaterialManager::CloneStandardMaterial();

	discMat = MaterialManager::CloneStandardMaterial();
	discMat->PixelShader = glowPixelShader;

	platformMat = MaterialManager::CloneStandardMaterial();
	platformMat->PixelShader = glowPixelShader;

	matTrans = MaterialManager::CloneStandardTransparentMaterial();
	matTransWhite = MaterialManager::CloneStandardTransparentMaterial();
	matTrans->transparency = 0.5f;
	matTransWhite->transparency = 0.5f;

	auto loadtex = MaterialManager::LoadWICTextureFromFile;

	p1mat->ResourceView           = loadtex(L"../Resources/Textures/playerOneUV.png");
	p2mat->ResourceView           = loadtex(L"../Resources/Textures/playerTwoUV.png");
	mat->ResourceView             = loadtex(L"../Resources/blueGlow.jpg");
	matTrans->ResourceView        = loadtex(L"../Resources/blueGlow.jpg");
	matWireframe->ResourceView    = loadtex(L"../Resources/white.jpg");
	matTransWhite->ResourceView   = loadtex(L"../Resources/white.jpg");
	discMat->ResourceView         = loadtex(L"../Resources/Textures/discTexture.png");
	platformMat->ResourceView     = loadtex(L"../Resources/Textures/platformTexture.png");
	p1mat->GlowResourceView       = loadtex(L"../Resources/GlowMaps/playerOneUVGlowMap.png");
	p2mat->GlowResourceView       = loadtex(L"../Resources/GlowMaps/playerTwoUVGlowMap.png");
	discMat->GlowResourceView     = loadtex(L"../Resources/GlowMaps/discTextureGlowMap.png");
	platformMat->GlowResourceView = loadtex(L"../Resources/GlowMaps/platformTextureGlowMap.png");
	menuDrawer->logo              = loadtex(L"../Resources/menu/menuGameLogo.png");
	menuDrawer->bars              = loadtex(L"../Resources/menu/menuBars.png");
	menuDrawer->bg                = loadtex(L"../Resources/menu/menuBG.png");
	menuDrawer->credits           = loadtex(L"../Resources/menu/creditsPage.png");
	menuDrawer->instructions      = loadtex(L"../Resources/menu/InstructionsPage.png");
	menuDrawer->buttons[0]        = loadtex(L"../Resources/menu/buttons/default/menuStartButton.png");
	menuDrawer->buttons[1]        = loadtex(L"../Resources/menu/buttons/default/menuInstructionsButton.png");
	menuDrawer->buttons[2]        = loadtex(L"../Resources/menu/buttons/default/menuCreditsButton.png");
	menuDrawer->buttons[3]        = loadtex(L"../Resources/menu/buttons/default/menuQuitGameButton.png");
	menuDrawer->selected[0]       = loadtex(L"../Resources/menu/buttons/selected/menuStartButtonSelected.png");
	menuDrawer->selected[1]       = loadtex(L"../Resources/menu/buttons/selected/menuInstructionsButtonSelected.png");
	menuDrawer->selected[2]       = loadtex(L"../Resources/menu/buttons/selected/menuCreditsButtonSelected.png");
	menuDrawer->selected[3]       = loadtex(L"../Resources/menu/buttons/selected/menuQuitGameButtonSelected.png");

	mat->SamplerState           = MaterialManager::GetStandardSamplerState();
	p1mat->SamplerState         = mat->SamplerState;
	p2mat->SamplerState         = mat->SamplerState;
	matWireframe->SamplerState  = mat->SamplerState;
	matTrans->SamplerState      = mat->SamplerState;
	matTransWhite->SamplerState = mat->SamplerState;
	discMat->SamplerState       = mat->SamplerState;
	platformMat->SamplerState   = mat->SamplerState;
	
	mat->RasterizerState = solidRS;
	matTrans->RasterizerState = transRS;
	matTrans->RasterizerState = transRS;
	matWireframe->RasterizerState = wireframeRS;

	Prototypes::SetPlayerMesh(0, mesh);
	Prototypes::SetPlayerMesh(1, p2Mesh);
	Prototypes::SetPlayerMaterial(0, p1mat);
	Prototypes::SetPlayerMaterial(1, p2mat);
	player = Prototypes::MakePlayer(0);
	player2 = Prototypes::MakePlayer(1);

	Prototypes::SetDiscMesh(discMesh);
	Prototypes::SetDiscMaterial(discMat);

	for (auto &disc : discs) disc = Prototypes::MakeDisc(player);
	for (auto &disc : p2discs) disc = Prototypes::MakeDisc(player2);

	Prototypes::SetArenaMesh(arenaMesh);
	Prototypes::SetArenaMaterial(matTrans);

	arena = Prototypes::MakeArena();

	Prototypes::SetPlatformMesh(platformMesh);
	Prototypes::SetPlatformMaterial(0, platformMat);
	Prototypes::SetPlatformMaterial(1, platformMat);

	p1Platform = Prototypes::MakePlatform(0);
	p2Platform = Prototypes::MakePlatform(1);

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
	if (menuDrawer != NULL)
	{
		menuDrawer->center = DirectX::XMFLOAT2(windowWidth / 2, windowHeight / 2);
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
		case GAME: {
			Input::SetMouseMode(Input::Mouse::MODE_RELATIVE);
			if (KeyPressedThisFrame(Keys::M) || gamePad.ButtonPressedThisFrame(trackedPadState.start))
				Menu();

			int scroll = Input::GetScrollWheel();
			Input::ResetScrollWheel();

			if (scroll) {
				auto &transparency = arena->GetMaterial()->transparency;

				transparency += scroll * SCROLL_WHEEL_TO_TRANSPARENCY;

				transparency = (transparency > 1.0f) ? 1.0f : transparency;
				transparency = (transparency < 0.0f) ? 0.0f : transparency;
			}
			break;
		}


		case MAIN: {
			Input::SetMouseMode(Input::Mouse::MODE_ABSOLUTE);
			if (KeyPressedThisFrame(Keys::Enter) || gamePad.ButtonPressedThisFrame(trackedPadState.a))
			{
				switch (menuChoice)
				{
				case 0:
					StartGame();
					break;
				case 1:
					Instruct();
					break;
				case 2:
					Credit();
					break;
				case 3:
					Quit();
					break;
				default:
					break;
				}
			}
			if (KeyPressedThisFrame(Keys::Down) || gamePad.ButtonPressedThisFrame(trackedPadState.dpadDown) || gamePad.GetState().thumbSticks.leftY < -0.75f)
			{
				menuChoice++;
				if (menuChoice > 3)
					menuChoice = 0;
			}
			if (KeyPressedThisFrame(Keys::Up) || gamePad.ButtonPressedThisFrame(trackedPadState.dpadUp) || gamePad.GetState().thumbSticks.leftY > 0.75f)
			{
				menuChoice--;
				if (menuChoice < 0)
					menuChoice = 3;
			}
			break;
		}
		case INSTRUCT: {
			if (KeyPressedThisFrame(Keys::Enter) || gamePad.ButtonPressedThisFrame(trackedPadState.a))
				Menu();
			break;
		}
		case CREDITS: {
			if (KeyPressedThisFrame(Keys::Enter) || gamePad.ButtonPressedThisFrame(trackedPadState.a))
				Menu();
			break;
		}
	}

	FrameUpdateData upData{ deltaTime, totalTime };

	debugCamera->Update(deltaTime, totalTime);
	trackingCamera->Update(deltaTime, totalTime);

	if (KeyPressedThisFrame(Keys::T))
		useDebugCamera = false;
	else if (KeyPressedThisFrame(Keys::Y))
		useDebugCamera = true;

	//this stuff is here to demonstrate the flow for working with a dynamic point light
	PointLight pl;
	pl.DiffuseColor = { 0.8f, 0.8f, 0.8f, 0.8f };
	pl.Location = { debugCamera->GetPosition().x, debugCamera->GetPosition().y, debugCamera->GetPosition().z };
	renderer->GetLightManager()->SetPointLight(0, &pl);
	//we shouldn't call the next method unless we actually changed the location of the light,
	//but I got a bit lazy here since we're really not actually going to need this later.
	//see what happens if you comment it out!
	renderer->GetLightManager()->UpdateLights(totalTime);

	switch (gState) {
	case GAME:
		player->Update(upData);
		player2->Update(upData);

		if (KeyPressedThisFrame(player->fire) || gamePad.ButtonPressedThisFrame(trackedPadState.a))
		{
			Disc* toUse = DiscToLaunch(0);
			if (toUse)
				player->Fire(toUse);
		}
		else
		{
			player->ReloadDisc();
		}

		if (KeyPressedThisFrame(player2->fire))
		{
			Disc* toUse = DiscToLaunch(1);
			if (toUse)
				player2->Fire(toUse);
		}
		else
		{
			player2->ReloadDisc();
		}

		CylinderCollider playerCollider = player2->colliderComp;

		static float collisionTimer = 0.0f;

		collisionTimer += deltaTime;

		for (unsigned int i = 0; i < discs.size(); ++i)
		{
			auto &disc = discs[i];

			if (!disc->IsActive()) continue;

			disc->MoveDisc(deltaTime);

			CylinderCollider discCollider = disc->colliderComp;
			auto colliding = IsColliding(playerCollider, discCollider);

			if (colliding)
			{
				discIsColliding[i] = colliding;

				player2->OnDiscHit();
				disc->OnHit();
			}
		}

		for (auto &disc : p2discs)
			if (disc->IsActive())
				disc->MoveDisc(deltaTime);

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
void MyDemoGame::Menu()
{
	gState = MAIN;
}
void MyDemoGame::Credit()
{
	gState = CREDITS;
}
void MyDemoGame::Instruct()
{
	gState = INSTRUCT;
}

// ----------------------------------------------------------
// Temporary Test to figure out how to launch different Discs
// ----------------------------------------------------------
Disc* MyDemoGame::DiscToLaunch(int i)
{
	if (i == 0)
	{
		for (auto &disc : discs)
			if (!disc->IsActive()) return disc;
	}
	if (i == 1)
	{
		for (auto &disc : p2discs)
			if (!disc->IsActive()) return disc;
	}

	return NULL;
}
// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// use correct camera
	renderer->SetCamera(useDebugCamera ? (Camera*)debugCamera : (Camera*)trackingCamera);

	renderer->StartFrame();


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

	//gState = GAME;
	//is the game going
	if (gState == GAME)
	{

		deviceContext->RSSetState(solidRS);
		renderer->DrawObject(player);
		renderer->DrawObject(player2);
		renderer->DrawObject(p1Platform);
		renderer->DrawObject(p2Platform);

		sky->skyVS->SetMatrix4x4("view", (useDebugCamera ? (Camera*)debugCamera : (Camera*)trackingCamera)->getView());
		sky->skyVS->SetMatrix4x4("projection", (useDebugCamera ? (Camera*)debugCamera : (Camera*)trackingCamera)->getProjection());

		for (auto &disc : discs) renderer->DrawObject(disc);
		for (auto &disc : p2discs) renderer->DrawObject(disc);

		deviceContext->RSSetState(wireframeRS);
		renderer->DrawObject(arena);
	}

	renderer->EndFrame(sky);
	if (gState != MAIN)
		menuDrawer->DrawUI(gState);
	else
		menuDrawer->DrawUI(gState, menuChoice);
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
