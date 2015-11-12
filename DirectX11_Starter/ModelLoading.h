#pragma once

#include <vector>
#include <d3d11.h>

struct VerticesAndIndices
{
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
};