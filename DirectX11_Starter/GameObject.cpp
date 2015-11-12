#include "GameObject.h"

/// <summary>
/// Standard constructor.
/// </summary>
/// <param name="m">the mesh to be used</param>
GameObject::GameObject(Mesh* m, Material* mat) : mesh(m), material(mat)
{
	init();
}

Material * GameObject::GetMaterial()
{
	return material;
}

void GameObject::init()
{
	worldMat = XMFLOAT4X4();

	translation = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	worldMatIsDirty = true;
}

/// <summary>
/// Gets the translation vector of the object.
/// </summary>
/// <returns>the translation vector</returns>
XMFLOAT3 GameObject::GetTranslation()
{
	return translation;
}

/// <summary>
/// Gets the rotation information of the object as a vector.  This vector is
/// sent to XMMatrixRollPitchYawAsVector to obtain the transformation matrix.
/// </summary>
/// <returns>the rotation vector</returns>
XMFLOAT3 GameObject::GetRotation()
{
	return rotation;
}

/// <summary>
/// Gets the scaling vector of the object.
/// </summary>
/// <returns>the scaling vector of the object</returns>
XMFLOAT3 GameObject::GetScale()
{
	return scale;
}


/// <summary>
/// Gets the translation vector of the object.
/// </summary>
/// <param name="t">the new translation vector</param>
void GameObject::SetTranslation(XMFLOAT3 t)
{
	translation = t;
	worldMatIsDirty = true;
}

/// <summary>
/// Sets the rotation information of the object as a vector.  This vector will be
/// sent to XMMatrixRollPitchYawAsVector to obtain the transformation matrix.
/// </summary>
/// <param name="r">the new rotation vector</param>
void GameObject::SetRotation(XMFLOAT3 r)
{
	rotation = r;
	worldMatIsDirty = true;
}

/// <summary>
/// Sets the scaling vector of the object.
/// </summary>
/// <param name="s">the new scaling vector</param>
void GameObject::SetScale(XMFLOAT3 s)
{
	scale = s;
	worldMatIsDirty = true;
}

/// <summary>
/// Translates the object.
/// </summary>
/// <param name="t">the translation vector</param>
void GameObject::Translate(XMFLOAT3 t)
{
	XMVECTOR vTranslation = XMLoadFloat3(&translation);
	XMVECTOR vNewTranslation = XMLoadFloat3(&t);
	XMStoreFloat3(&translation, vTranslation + vNewTranslation);

	worldMatIsDirty = true;
}

void GameObject::Rotate(XMFLOAT3 r)
{
	XMVECTOR vRotation = XMLoadFloat3(&rotation);
	XMVECTOR vNewRotation = XMLoadFloat3(&r);
	XMStoreFloat3(&rotation, vRotation + vNewRotation);

	worldMatIsDirty = true;
}

/// <summary>
/// Scales the object.  Each component of the input vector will be
/// multiplied by the corresponding component of the existing 
/// scale vector to get the final scaling factor.
/// </summary>
/// <param name="s">the vector by which to scale</param>
void GameObject::Scale(XMFLOAT3 s)
{
	XMVECTOR vScale = XMLoadFloat3(&scale);
	XMVECTOR vNewScale = XMLoadFloat3(&s);
	XMStoreFloat3(&scale, XMVectorMultiply(vScale, vNewScale));

	worldMatIsDirty = true;
}

void GameObject::updateWorldMatrix()
{
	//create matrices from transformations
	XMMATRIX mTrans = XMMatrixTranslationFromVector(XMLoadFloat3(&translation));
	XMMATRIX mRot = XMMatrixRotationRollPitchYawFromVector(
		XMLoadFloat3(&rotation));
	XMMATRIX mScale = XMMatrixScalingFromVector(XMLoadFloat3(&scale));

	//combine transformations
	XMMATRIX mWorldMat = mScale * mRot * mTrans;

	//store the result
	XMStoreFloat4x4(&worldMat, XMMatrixTranspose(mWorldMat));
}

/// <summary>
/// Draw the current mesh using the current material.  
/// </summary>
/// <param name="context">the device context to use to draw the mesh</param>
void GameObject::Draw(ID3D11DeviceContext* context)
{
	if (worldMatIsDirty) updateWorldMatrix();

	material->VertexShader->SetMatrix4x4("world", worldMat);
	material->VertexShader->CopyAllBufferData();

	material->PixelShader->SetShaderResourceView("diffuseTexture", material->ResourceView);
	material->PixelShader->SetSamplerState("basicSampler", material->SamplerState);

	mesh->Draw(worldMat, context);
}