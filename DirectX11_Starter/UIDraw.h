#pragma once
#include <SpriteBatch.h>
#include "SpriteFont.h"
#include <DirectXMath.h>

class UIDraw
{
public:
	UIDraw(ID3D11DeviceContext*, ID3D11Device*);
	~UIDraw();
	void DrawUI(int);
	void DrawUI(int, int);
	ID3D11ShaderResourceView* logo;
	ID3D11ShaderResourceView* credits;
	ID3D11ShaderResourceView* instructions;
	ID3D11ShaderResourceView* bars;
	ID3D11ShaderResourceView* bg;
	ID3D11ShaderResourceView* buttons[4];
	ID3D11ShaderResourceView* selected[4];
	DirectX::XMFLOAT2 center;
private:
	ID3D11BlendState* blendState;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
};

