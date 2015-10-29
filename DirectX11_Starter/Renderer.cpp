#include "Renderer.h"

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

/// <summary>
/// Draw a GameObject.
/// </summary>
/// <param name="object">the object to draw</param>
void Renderer::DrawObject(GameObject* object)
{
	Material &mat = *object->mesh->material;

	mat.VertexShader->SetShader(false);
	mat.PixelShader->SetShader(false);

	if (lightsLastUpdated < lm->GetLastUpdated())
	{
		lm->SetLightBufferData(mat.PixelShader);
		lightsLastUpdated = lm->GetLastUpdated();
	}

	mat.PixelShader->CopyAllBufferData();

	camera->SetViewAndProjMatrices(mat.VertexShader);

	object->Draw(context);
}
