#include "GameObject.h"
#include "Camera.h"
void GameObject::init(const Mesh& mesh, const wchar_t* texturePath, const Transform& transform, float aspectRatio)
{

	createBuffer(mesh);
	createTexture(texturePath);
	createShader();
	setTransform(transform);
	updateWorldViewProjMatrix(aspectRatio);
	updateLight(PLight);
}

void GameObject::createBuffer(const Mesh& mesh)
{
	// VertexBuffer
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)mesh.vertexbuffer.size()*sizeof(VertexPosNormalTex);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mesh.vertexbuffer.data();
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

	//IndexBuffer
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = mesh.indexbuffer.size() * sizeof(DWORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	InitData.pSysMem = mesh.indexbuffer.data();
	HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));


	//VSConstantBuffer
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(VSConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer[0].GetAddressOf()));
	cbd.ByteWidth = sizeof(PSConstantBuffer);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer[1].GetAddressOf()));


}

void GameObject::createShader()
{
	ComPtr<ID3DBlob> blob;

	// 创建顶点着色器
	HR(CreateShaderFromFile(L"HLSL\\Common_VS.cso", L"HLSL\\Common_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));


	// 创建顶点布局
	D3D11_INPUT_ELEMENT_DESC inputLayout[3] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	HR(m_pd3dDevice->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));


	// 创建像素着色器
	HR(CreateShaderFromFile(L"HLSL\\Common_PS.cso", L"HLSL\\Common_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));


}

void GameObject::createTexture(const wchar_t* texturePath)
{
	// 初始化木箱纹理
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), texturePath, nullptr, m_pTexture.GetAddressOf()));
	// 初始化采样器状态
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(m_pd3dDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()));


}


void GameObject::bindToRenderPipeline()
{
	UINT stride = sizeof(VertexPosNormalTex);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量
	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer[0].GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffer[1].GetAddressOf());

	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pd3dImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
}


void GameObject::draw(UINT indexCount)
{

	bindToRenderPipeline();
	m_pd3dImmediateContext->DrawIndexed(indexCount, 0, 0);
}




void GameObject::updateWorldViewProjMatrix(float AspectRatio)
{
	VSConstantBuffer vsConstantBuffer;
	vsConstantBuffer.world = XMMatrixTranspose(m_transform.getWorldMatrix());
	vsConstantBuffer.view = XMMatrixTranspose(Camera::getInstance().getViewMatrix());
	vsConstantBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PI / 3, AspectRatio, 0.5f, 1000.0f));
	DirectX::XMMATRIX A = m_transform.getWorldMatrix();
	A.r[3] = g_XMIdentityR3;
	vsConstantBuffer.worldInvTranspose = XMMatrixTranspose(XMMatrixTranspose(XMMatrixInverse(nullptr, A)));

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffer[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(vsConstantBuffer), &vsConstantBuffer, sizeof(vsConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffer[0].Get(), 0);
}

void GameObject::updateLight(LightType lt)
{
	// ******************
	// 初始化默认光照
	/*
	*高光反射就是基本上就是光源反射 specular
	*漫反射就是光源打在不光滑的物体上的反射 diffuse
	*环境光是打个太阳的比方，太阳在一天之内的颜色是不一样的。ambient
	*/

	// 方向光
	DirectionalLight dirLight;
	dirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.direction = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	
	// 点光
	PointLight pointLight; 
	pointLight.position = XMFLOAT3(0.0f, 15.0f, 0.0f);
	pointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	pointLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	pointLight.range = 25.0f;
	// 聚光灯
	SpotLight spotLight;
	spotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	spotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	spotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	spotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	spotLight.spot = 12.0f;
	spotLight.range = 10000.0f;

	PSConstantBuffer psConstantBuffer;

	switch (lt)
	{
	case DLight:
	{
		psConstantBuffer.dirLight = dirLight;
		psConstantBuffer.pointLight = PointLight();
		psConstantBuffer.spotLight = SpotLight();
	}
	break;
	case PLight:
	{
		psConstantBuffer.dirLight = DirectionalLight();
		psConstantBuffer.pointLight = pointLight;
		psConstantBuffer.spotLight = SpotLight();
	}
	break;
	case SLight:
	{
		psConstantBuffer.dirLight = DirectionalLight();
		psConstantBuffer.pointLight = PointLight();
		psConstantBuffer.spotLight = spotLight;
	}
	break;
	default:
		break;
	}

	psConstantBuffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	psConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	psConstantBuffer.material.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 5.0f);


	;
	psConstantBuffer.eyePos = XMFLOAT4(Camera::getInstance().getPosition().x, Camera::getInstance().getPosition().y, Camera::getInstance().getPosition().z, 0.0f);

	// 更新PS常量缓冲区资源
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffer[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &psConstantBuffer, sizeof(PSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffer[1].Get(), 0);
}