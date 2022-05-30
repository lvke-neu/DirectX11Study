#pragma once

#include <wrl/client.h>
#include "../GameFramework/DXTrace.h"
#include "../GameFramework/d3dUtil.h"
#include "Transform.h"
#include "Geometry.h"
#include "LightHelper.h"
#include "Camera.h"


template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
class GameObject
{
public:
	struct  VSConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
		XMMATRIX worldInvTranspose;
	};

	struct PSConstantBuffer
	{
		DirectionalLight dirLight;
		PointLight pointLight;
		SpotLight spotLight;
		Material material;
		DirectX::XMFLOAT4 eyePos;
	};

public:
	GameObject(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext) :
		m_pd3dDevice(pd3dDevice), m_pd3dImmediateContext(pd3dImmediateContext) {}

	void createBuffer(const Mesh& mesh);
	void createShader();
	void createTexture( const wchar_t* texturePath);
	void updateWorldViewProjMatrix(float AspectRatio);
	void updateLight(LightType lt);
	void bindToRenderPipeline();
	void draw(UINT indexCount);
	void init(const Mesh& mesh, const wchar_t* texturePath, const Transform& transform, float aspectRatio);

	void setTransform(const Transform& transform){ m_transform = transform; }
	Transform getTransform(){ return m_transform; }

	inline XMFLOAT3 getScale() { return m_transform.getScale(); }
	void setScale(const XMFLOAT3& scale) { m_transform.setScale(scale); }
	inline XMFLOAT3 getRotation() { return m_transform.getRotation(); }
	void setRotation(const XMFLOAT3& rotation) { m_transform.setRotation(rotation); }
	inline XMFLOAT3 getPosition() { return m_transform.getPosition(); }
	void setPosition(const XMFLOAT3& position) { m_transform.setPosition(position); }

private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// 索引缓冲区
	ComPtr<ID3D11Buffer> m_pConstantBuffer[2];		// 常量缓冲区

	ComPtr<ID3D11InputLayout> m_pVertexLayout;	// 顶点输入布局
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// 顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader;	// 像素着色器

	ComPtr<ID3D11ShaderResourceView> m_pTexture;			    // 木盒纹理
	ComPtr<ID3D11SamplerState> m_pSamplerState;				    // 采样器状态


	Transform m_transform; //变换
	ComPtr<ID3D11Device> m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;
};