#pragma once
#include <d3d11.h>

class PostProcess
{
public:
	PostProcess();

	virtual void Do(ID3D11ShaderResourceView* render, ID3D11RenderTargetView* backbuffer) = 0;

	~PostProcess();
};

