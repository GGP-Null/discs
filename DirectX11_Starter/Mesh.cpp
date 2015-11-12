#include "Mesh.h"

using namespace DirectX;

Mesh::Mesh(VerticesAndIndices &verts_and_indices, ID3D11Device *device)
{
	setup(
		verts_and_indices.vertices.data(),
		verts_and_indices.vertices.size(),
		verts_and_indices.indices.data(),
		verts_and_indices.indices.size(),
		device
		);
}

/// <summary>
/// Constructor for a procedurally generated mesh.
/// </summary>
/// <param name="vertices">array of indexed vertices</param>
/// <param name="numVertices">number of vertices</param>
/// <param name="indices">array of indices</param>
/// <param name="numIndices">number of indices</param>
/// <param name="device">the device to create the buffers on</param>
Mesh::Mesh(const Vertex* vertices,
	int numVertices,
	UINT* indices,
	int numIndices,
	ID3D11Device* device)
{
	setup(vertices, numVertices, indices, numIndices, device);
}

void Mesh::setup(const Vertex* vertices,
	int numVertices,
	UINT* indices,
	int numIndices,
	ID3D11Device* device)
{
	//create vertex buffer

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;

	HR(device->CreateBuffer(&vbd, &vertexData, &vertexBuffer));

	// create index buffer

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * numIndices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;

	HR(device->CreateBuffer(&ibd, &indexData, &indexBuffer));

	// we'll need this to draw the mesh
	indexCount = numIndices;
}

/// <summary>
/// Gets the pointer to the ID3D11Buffer storing the indexed vertices for this mesh.
/// </summary>
/// <returns>pointer to the vertex buffer</returns>
ID3D11Buffer* Mesh::GetVertexBuffer() { return vertexBuffer; }

/// <summary>
/// Gets the pointer to the ID3D11Buffer storing the indices for this mesh.
/// </summary>
/// <returns>pointer to the index buffer</returns>
ID3D11Buffer* Mesh::GetIndexBuffer() { return indexBuffer; }

/// <summary>
/// Gets the number of indices in the index buffer.
/// </summary>
/// <returns>number of vertices in the index buffer</returns>
int Mesh::GetIndexCount() { return indexCount; }

// draw the mesh to the screen
// allows for reusability of the basic steps required to draw the mesh,
// and cleans up the main draw method so it's a bit clearer
/// <summary>
/// Draws the mesh.
/// </summary>
/// <param name="context">the context to use for drawing the mesh</param>
void Mesh::Draw(XMFLOAT4X4 &worldMat, ID3D11DeviceContext* context)
{
	// set the buffers to be used
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//do the actual draw operation
	context->DrawIndexed(indexCount, 0, 0);
}

/// <summary>
/// Destructor.  Releases the vertex and index buffers on the graphics card.  
/// </summary>
Mesh::~Mesh()
{
	//release the buffers (this should be the only reference to them (we hope))
	vertexBuffer->Release();
	indexBuffer->Release();
}
