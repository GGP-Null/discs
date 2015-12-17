#include "UIDraw.h"



UIDraw::UIDraw(ID3D11DeviceContext* dc, ID3D11Device* d)
{
	//set up spritebatch
	m_spriteBatch.reset(new DirectX::SpriteBatch(dc));
	
	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	d->CreateBlendState(&bd, &blendState);
}


UIDraw::~UIDraw()
{
	blendState->Release();
}

void UIDraw::DrawUI(int state)
{
	if (state == 1) return;
	m_spriteBatch->Begin();
	switch (state)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		m_spriteBatch->Draw(bg, DirectX::XMFLOAT2(0, 0));
		m_spriteBatch->Draw(instructions, DirectX::XMFLOAT2(0, center.y - 512));
		break;
	case 3:
		m_spriteBatch->Draw(bg, DirectX::XMFLOAT2(0, 0));
		m_spriteBatch->Draw(credits, DirectX::XMFLOAT2(0, center.y - 512));
		break;
	default:
		break;
	}
	m_spriteBatch->End();
}
void UIDraw::DrawUI(int state, int choice)
{
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, blendState);
	switch (state)
	{
	case 0:
		m_spriteBatch->Draw(bg, DirectX::XMFLOAT2(0, 0));
		m_spriteBatch->Draw(logo, DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
		m_spriteBatch->Draw(bars, DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
		switch (choice)
		{
		case 0:
			m_spriteBatch->Draw(selected[0], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[1], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[2], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[3], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			break;
		case 1:
			m_spriteBatch->Draw(buttons[0], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(selected[1], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[2], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[3], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			break;
		case 2:
			m_spriteBatch->Draw(buttons[0], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[1], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(selected[2], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[3], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			break;
		case 3:
			m_spriteBatch->Draw(buttons[0], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[1], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(buttons[2], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			m_spriteBatch->Draw(selected[3], DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
			break;
		default:
			break;
		}
	default:
		break;
	}
	m_spriteBatch->End();
}


