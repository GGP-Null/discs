#include "UIDraw.h"



UIDraw::UIDraw(ID3D11DeviceContext* dc, ID3D11Device* d)
{
	//set up spritebatch
	m_spriteBatch.reset(new DirectX::SpriteBatch(dc));
	
}


UIDraw::~UIDraw()
{

}

void UIDraw::DrawUI(int state)
{
	
}
void UIDraw::DrawUI(int state, int choice)
{
	m_spriteBatch->Begin();
	switch (state)
	{
	case 0:
		m_spriteBatch->Draw(bg, DirectX::XMFLOAT2(center.x - 1024, center.y - 512));
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


