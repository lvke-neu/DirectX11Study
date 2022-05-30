#include "Camera.h"


Camera Camera::camera;


void Camera::startRasterization(bool b, ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	if (!m_pRasterizerState)
	{
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		HR(pd3dDevice->CreateRasterizerState(&rasterizerDesc, m_pRasterizerState.GetAddressOf()));
	}

	pd3dImmediateContext->RSSetState(b ? m_pRasterizerState.Get() : nullptr);
}