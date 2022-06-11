#include "GameObject.h"



void GameObject::setMesh(const Mesh& mesh)
{
	m_mesh = mesh;
    GameObjectResource::getVertexBuffer(mesh, m_pVertexBuffer);
    GameObjectResource::getIndexBuffer(mesh, m_pIndexBuffer);
}

void GameObject::setShader()
{
	GameObjectResource::getShader(m_pVertexShader, m_pVertexLayout, m_pPixelShader);
}

void GameObject::setTexture(const wchar_t* texturePath)
{
	m_texturePath = texturePath;

	GameObjectResource::getTexture(texturePath, m_pTexture);
}

void GameObject::setMaterial(const  Material& material)
{
	m_material = material;
}


void GameObject::draw()
{
	//绑定vertexbuffer、indexbuffer到渲染管线
	UINT stride = sizeof(VertexPosNormalTex);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量
	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	
	//绑定vertexshader、输入布局、pixelshader到渲染管线
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	//绑定贴图资源到渲染管线
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());

	//绑定世界矩阵、材质到渲染管线
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, GameObjectResource::getGoConstantBuffer(m_transform.getWorldMatrix(), m_material).GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(0, 1, GameObjectResource::getGoConstantBuffer(m_transform.getWorldMatrix(), m_material).GetAddressOf());



	//绘制
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->DrawIndexed(m_mesh.indexbuffer.size(), 0, 0);
}


void GameObject::moveForward(float dis)
{

	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&m_transform.getForwarDir()));

	XMVECTOR pos;
	pos = XMLoadFloat3(&m_transform.getPosition());

	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, pos + dir * dis);

	m_transform.setPosition(newPos);
}

void GameObject::moveRight(float dis)
{

	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&m_transform.getRightDir()));

	XMVECTOR pos;
	pos = XMLoadFloat3(&m_transform.getPosition());

	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, pos + dir * dis);

	m_transform.setPosition(newPos);
}
