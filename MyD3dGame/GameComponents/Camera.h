#pragma once

#include "Transform.h"
#include <wrl/client.h>
#include "../GameFramework/d3dUtil.h"
#include "../GameFramework/DXTrace.h"
#include "LightHelper.h"

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
class Camera
{

private:
	Camera()
	{
		m_transform.setScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
		m_transform.setRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_transform.setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
public:
	

	Transform getTransform() 
	{ 
		return m_transform; 
	}
	void setTransform(const Transform& transform) 
	{ 
		m_transform = transform; 
	}

	XMFLOAT3 getPosition() 
	{ 
		return m_transform.getPosition(); 
	}
	void setPosition(const XMFLOAT3& pos) 
	{ 
		m_transform.setPosition(pos); 
	}

	XMFLOAT3 getRotation()
	{
		return m_transform.getRotation();
	}
	void setRotation(const XMFLOAT3& rot)
	{
		m_transform.setRotation(rot);
	}

	void moveForward(float dis)
	{

		XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&m_transform.getForwarDir()));

		XMVECTOR pos;
		pos = XMLoadFloat3(&m_transform.getPosition());

		XMFLOAT3 newPos;
		XMStoreFloat3(&newPos, pos + dir * dis);

		m_transform.setPosition(newPos);
	}

	void moveRight(float dis)
	{

		XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&m_transform.getRightDir()));

		XMVECTOR pos;
		pos = XMLoadFloat3(&m_transform.getPosition());

		XMFLOAT3 newPos;
		XMStoreFloat3(&newPos, pos + dir * dis);

		m_transform.setPosition(newPos);
	}


	XMMATRIX getViewMatrix() 
	{ 
		return XMMatrixInverse(nullptr, m_transform.getWorldMatrix());
	}

	static Camera& getInstance()
	{
		return camera;
	}


//视图矩阵、投影矩阵
public:
	struct  CBView
	{
		XMMATRIX view;
	};
	struct  CBProj
	{
		XMMATRIX proj;
	};

	struct LightConstantBuffer
	{
		DirectionalLight DirLight;
		PointLight PointLight;
		SpotLight SpotLight;
		//Material g_Material;
		XMFLOAT3 g_EyePosW;
		float g_Pad;
	};

	void init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext);
	void setFrustum(float FovAngleY, float AspectRatio, float NearZ, float FarZ) { m_FovAngleY = FovAngleY, m_AspectRatio = AspectRatio, m_NearZ= NearZ, m_FarZ = FarZ; }
	bool isDeviceContextEmpty() { return m_pd3dDevice == nullptr || m_pd3dImmediateContext == nullptr ? 1 : 0; }
	
	void updateViewMatrix();
	void updateProjMatrix();

private:
	Transform m_transform;
	static Camera camera;

	ComPtr<ID3D11Device> m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;
	ComPtr<ID3D11Buffer> m_pViewBuffer;
	ComPtr<ID3D11Buffer> m_pProjBuffer;
	ComPtr<ID3D11Buffer> m_pLightConstantBuffer;

	float m_FovAngleY;
	float m_AspectRatio;
	float m_NearZ;
	float m_FarZ;

	//ComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;	// 光栅化状态: 线框模式
};


