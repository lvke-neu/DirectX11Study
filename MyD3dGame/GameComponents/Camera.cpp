#include "Camera.h"


Camera Camera::camera;



void Camera::init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	m_pd3dImmediateContext = pd3dImmediateContext;
	m_pd3dDevice = pd3dDevice;

	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	cbd.ByteWidth = sizeof(CBView);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pViewBuffer.GetAddressOf()));
	cbd.ByteWidth = sizeof(CBProj);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pProjBuffer.GetAddressOf()));
	//初始化灯光
	cbd.ByteWidth = sizeof(LightConstantBuffer);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pLightConstantBuffer.GetAddressOf()));
}


void Camera::updateViewMatrix()
{
	CBView cbview;
	cbview.view = XMMatrixTranspose(getViewMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pViewBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBView), &cbview, sizeof(CBView));
	m_pd3dImmediateContext->Unmap(m_pViewBuffer.Get(), 0);


	DirectionalLight dirLight;
	dirLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.direction = XMFLOAT3(0.0f, -1.0f, 0.0f);

	PointLight pointLight;
	pointLight.position = XMFLOAT3(0.0f, 10.0f, -10.0f);
	pointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pointLight.diffuse = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	pointLight.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	pointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	pointLight.range = 25.0f;

	LightConstantBuffer lightConstantBuffer;
	lightConstantBuffer.DirLight = dirLight;
	lightConstantBuffer.PointLight = pointLight;
	lightConstantBuffer.g_EyePosW = m_transform.getPosition();


	HR(m_pd3dImmediateContext->Map(m_pLightConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(LightConstantBuffer), &lightConstantBuffer, sizeof(LightConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pLightConstantBuffer.Get(), 0);


	//绑定视图矩阵到渲染管线
	m_pd3dImmediateContext->VSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());
	//绑定灯光到渲染管线
	m_pd3dImmediateContext->PSSetConstantBuffers(3, 1, m_pLightConstantBuffer.GetAddressOf());

}
void Camera::updateProjMatrix()
{
	CBProj cbproj;
	cbproj.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(m_FovAngleY, m_AspectRatio, m_NearZ, m_FarZ));

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pProjBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBProj), &cbproj, sizeof(CBProj));
	m_pd3dImmediateContext->Unmap(m_pProjBuffer.Get(), 0);

	m_pd3dImmediateContext->VSSetConstantBuffers(2, 1, m_pProjBuffer.GetAddressOf());
}