#include "Renderer.h"

#define BLEND_STATE_SAMPLE_DEFAULT 0xFFFFFFFF

/// <summary>
/// Default constructor.
/// </summary>
/// <param name="c">the camera holding the correct view and projection matrices</param>
/// <param name="con">the device context to draw objects with</param>
Renderer::Renderer(Camera* c, ID3D11DeviceContext* con)
{
	camera = c;
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

void Renderer::EndFrame()
{
	context->OMSetBlendState(nullptr, nullptr, BLEND_STATE_SAMPLE_DEFAULT);
	for (GameObject *go : standardBucket)
		doDraw(go);
	standardBucket.clear();

	while (!transparentBucket.empty()) {
		GameObject *go = transparentBucket.top();
		transparentBucket.pop();

		doDrawTransparent(go);
	}
}

void Renderer::doDraw(GameObject* obj)
{
	Material &mat = *obj->GetMaterial();

	mat.VertexShader->SetShader(false);
	mat.PixelShader->SetShader(false);

	if (lightsLastUpdated < lm->GetLastUpdated())
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

	if (lightsLastUpdated < lm->GetLastUpdated())
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
