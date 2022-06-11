#include "GameObjectResource.h"

ComPtr<ID3D11Buffer> GameObjectResource::m_pGoConstantBuffer = nullptr; 
ComPtr<ID3D11Buffer> GameObjectResource::m_pReflectionCB = nullptr;
ComPtr<ID3D11DeviceContext> GameObjectResource::m_pd3dImmediateContext = nullptr;
ComPtr<ID3D11Device> GameObjectResource::m_pd3dDevice = nullptr;

void GameObjectResource::init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	m_pd3dImmediateContext = pd3dImmediateContext;
	m_pd3dDevice = pd3dDevice;

	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	cbd.ByteWidth = sizeof(GoConstantBuffer);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pGoConstantBuffer.GetAddressOf()));

	cbd.ByteWidth = sizeof(RefectionConstantBuffer);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pReflectionCB.GetAddressOf()));



}

ComPtr<ID3D11Buffer>	GameObjectResource::getGoConstantBuffer(XMMATRIX worldMatrix, Material material)
{
	GoConstantBuffer goConstantBuffer;
	goConstantBuffer.world = XMMatrixTranspose(worldMatrix);
	XMMATRIX A = worldMatrix;
	A.r[3] = g_XMIdentityR3;
	goConstantBuffer.worldInvTranspose = XMMatrixTranspose(XMMatrixTranspose(XMMatrixInverse(nullptr, A)));
	goConstantBuffer.material = material;

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pGoConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(GoConstantBuffer), &goConstantBuffer, sizeof(GoConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pGoConstantBuffer.Get(), 0);

	return m_pGoConstantBuffer;

}

void	GameObjectResource::getVertexBuffer(const Mesh& mesh, ComPtr<ID3D11Buffer>& vertexBuffer)
{
	// VertexBuffer
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)mesh.vertexbuffer.size() * sizeof(VertexPosNormalTex);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mesh.vertexbuffer.data();
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, vertexBuffer.GetAddressOf()));

}

void	GameObjectResource::getIndexBuffer(const Mesh& mesh, ComPtr<ID3D11Buffer>& indexBuffer)
{
	//IndexBuffer
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = mesh.indexbuffer.size() * sizeof(DWORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mesh.indexbuffer.data();
	HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, indexBuffer.GetAddressOf()));
}

void GameObjectResource::getShader(ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& vertexLayout, ComPtr<ID3D11PixelShader>& pixelShader)
{
	ComPtr<ID3DBlob> blob;

	// 创建顶点着色器
	HR(CreateShaderFromFile(L"HLSL\\Common_VS.cso", L"HLSL\\Common_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// 创建顶点布局
	D3D11_INPUT_ELEMENT_DESC inputLayout[3] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	HR(m_pd3dDevice->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), vertexLayout.GetAddressOf()));

	// 创建像素着色器
	HR(CreateShaderFromFile(L"HLSL\\Common_PS.cso", L"HLSL\\Common_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));
}

void GameObjectResource::getTexture(const wchar_t* texturePath, ComPtr<ID3D11ShaderResourceView>& texture)
{
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), texturePath, nullptr, texture.GetAddressOf()));
}

void GameObjectResource::setIsReflectionAndIsShadow(bool bReflection, bool bShadow)
{
	GameObjectResource::RefectionConstantBuffer rf;
	rf.isReflection = bReflection;
	rf.reflection = XMMatrixTranspose(XMMatrixReflect(XMVectorSet(0.0f, 0.0f, -1.0f, 10.0f)));
	rf.isShadow = bShadow;
	rf.shadow = XMMatrixTranspose(XMMatrixShadow(XMVectorSet(0.0f, 1.0f, 0.0f, 0.99f), XMVectorSet(0.0f, 10.0f, -10.0f, 1.0f)));
	rf.refShadow = XMMatrixTranspose(XMMatrixShadow(XMVectorSet(0.0f, 1.0f, 0.0f, 0.99f), XMVectorSet(0.0f, 10.0f, 30.0f, 1.0f)));
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pReflectionCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(GameObjectResource::RefectionConstantBuffer), &rf, sizeof(GameObjectResource::RefectionConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pReflectionCB.Get(), 0);
	m_pd3dImmediateContext->VSSetConstantBuffers(4, 1, m_pReflectionCB.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(4, 1, m_pReflectionCB.GetAddressOf());
}


void GameObjectResource::SetWriteStencilOnly(UINT stencilRef, bool bShowMesh)
{
	m_pd3dImmediateContext->RSSetState(bShowMesh ? RenderStates::RSWireframe.Get() : nullptr);
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSWriteStencil.Get(), stencilRef);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSNoColorWrite.Get(), nullptr, 0xFFFFFFFF);
}

void GameObjectResource::SetRenderDefaultWithStencil(UINT stencilRef, bool bShowMesh)
{
	m_pd3dImmediateContext->RSSetState(bShowMesh ? RenderStates::RSWireframe.Get() : RenderStates::RSCullClockWise.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSDrawWithStencil.Get(), stencilRef);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void GameObjectResource::SetRenderNoDoubleBlend(UINT stencilRef, bool bShowMesh)
{
	m_pd3dImmediateContext->RSSetState(bShowMesh ? RenderStates::RSWireframe.Get() : RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSNoDoubleBlend.Get(), stencilRef);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);
}


void GameObjectResource::SetRenderAlphaBlendWithStencil(UINT stencilRef, bool bShowMesh)
{
	m_pd3dImmediateContext->RSSetState(bShowMesh ? RenderStates::RSWireframe.Get() : RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSDrawWithStencil.Get(), stencilRef);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);
}

void GameObjectResource::SetRenderDefault(bool bShowMesh)
{
	m_pd3dImmediateContext->RSSetState(bShowMesh ? RenderStates::RSWireframe.Get() : nullptr);
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

