#include "Renderer.h"

#define BLEND_STATE_SAMPLE_DEFAULT 0xFFFFFFFF

/// <summary>
/// Default constructor.
/// </summary>
/// <param name="c">the camera holding the correct view and projection matrices</param>
/// <param name="con">the device context to draw objects with</param>
Renderer::Renderer(Camera* c, ID3D11Device* dev, ID3D11DeviceContext* con)
{
	camera = c;
	device = dev;
	context = con;
	lm = new LightManager(0);
	lightsLastUpdated = -1;
}


/// <summary>
/// Empty destructor.  (not anymore, but this isn't worth updating until future updates)
/// </summary>
Renderer::~Renderer()
{
	delete lm;
}

/// <summary>
/// Gets the light manager that is used for this renderer.
/// </summary>
/// <returns>the light manager</returns>
LightManager * Renderer::GetLightManager()
{
	return lm;
}

void Renderer::SetCamera(Camera * c)
{
	camera = c;
}

void Renderer::SetPostProcess(PostProcess* post, int windowWidth, int windowHeight, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView* bb)
{
	postProcess = post;
	depthStencilView = dsv;
	backBuffer = bb;

	if (post != nullptr && postShaderResourceView == nullptr)
	{
		// Create post-process resources
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = windowWidth;
		textureDesc.Height = windowHeight;
		textureDesc.ArraySize = 1;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.MipLevels = 1;
		textureDesc.MiscFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* ppTexture;
		device->CreateTexture2D(&textureDesc, 0, &ppTexture);

		// Create the Render Target View
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		device->CreateRenderTargetView(ppTexture, &rtvDesc, &postRenderTargetView);

		// Create the Shader Resource View
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		device->CreateShaderResourceView(ppTexture, &srvDesc, &postShaderResourceView);

		// We don't need the texture reference no mo'
		ppTexture->Release();

	}
}

// In the future, this will add an object to a group
// of objects drawn with the same shaders, and when we're finished queuing up all
// objects to be drawn, this class will draw each batch of objects in front-to-back
// order.  This aims to reduce the number of times that shaders need to be switched and
// reduce the number of pixels that need to be redrawn, without putting the onus of
// deciding draw order on the developer writing the game logic.

void Renderer::StartFrame()
{
}

/// <summary>
/// Draw a GameObject.
/// </summary>
/// <param name="object">the object to draw</param>
void Renderer::DrawObject(GameObject* object)
{
	if (object->GetMaterial()->IsTransparent())
		transparentBucket.push(object);
	else
		standardBucket.push_back(object);
}

void Renderer::EndFrame(Skybox* sky)
{
	if (postProcess != nullptr)
	{
		context->OMSetRenderTargets(1, &postRenderTargetView, depthStencilView);

		const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
		context->ClearRenderTargetView(postRenderTargetView, color);
		context->ClearDepthStencilView(
			depthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0);
	}

	drawSky(sky);

	for (GameObject *go : standardBucket)
		doDraw(go);
	standardBucket.clear();

	while (!transparentBucket.empty()) {
		GameObject *go = transparentBucket.top();
		transparentBucket.pop();

		doDrawTransparent(go);
	}
	context->OMSetBlendState(nullptr, nullptr, BLEND_STATE_SAMPLE_DEFAULT);

	if (postProcess != nullptr)
	{
		postProcess->Do(postShaderResourceView, backBuffer);
	}
}

void Renderer::drawSky(Skybox* sky)
{
	// Now that solid "stuff" is drawn, draw the sky
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* skyVB = sky->skyMesh->GetVertexBuffer();
	ID3D11Buffer* skyIB = sky->skyMesh->GetIndexBuffer();
	context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

	sky->skyVS->SetShader();

	sky->skyPS->SetShaderResourceView("sky", sky->skyTexture);
	//SUPER ugly fix.  I wish we had another reference to this
	sky->skyPS->SetSamplerState("trilinear", MaterialManager::GetStandardSamplerState());
	sky->skyPS->SetShader();

	context->RSSetState(sky->rsSky);
	context->OMSetDepthStencilState(sky->dsSky, 0);
	context->DrawIndexed(sky->skyMesh->GetIndexCount(), 0, 0);

	// Reset the states to their defaults
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}

void Renderer::doDraw(GameObject* obj)
{
	Material &mat = *obj->GetMaterial();

	mat.VertexShader->SetShader(false);
	mat.PixelShader->SetShader(false);

	//if (lightsLastUpdated < lm->GetLastUpdated())
	{
		lm->SetLightBufferData(mat.PixelShader);
		lightsLastUpdated = lm->GetLastUpdated();
	}

	mat.PixelShader->CopyAllBufferData();

	camera->SetViewAndProjMatrices(mat.VertexShader);

	context->RSSetState(mat.RasterizerState);

	obj->Draw(context);
}

void Renderer::doDrawTransparent(GameObject* obj)
{
	Material &mat = *obj->GetMaterial();

	mat.VertexShader->SetShader(false);
	mat.PixelShader->SetShader(false);

	//if (lightsLastUpdated < lm->GetLastUpdated())
	{
		lm->SetLightBufferData(mat.PixelShader);
		lightsLastUpdated = lm->GetLastUpdated();
	}

	context->OMSetBlendState(mat.BlendState, nullptr, BLEND_STATE_SAMPLE_DEFAULT);

	mat.PixelShader->SetFloat("alpha", mat.transparency);

	mat.PixelShader->CopyAllBufferData();

	camera->SetViewAndProjMatrices(mat.VertexShader);

	context->RSSetState(mat.RasterizerState);

	obj->Draw(context);

}
