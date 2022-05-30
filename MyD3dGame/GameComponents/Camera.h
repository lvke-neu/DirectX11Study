#pragma once

#include "Transform.h"
#include <wrl/client.h>
#include "../GameFramework/d3dUtil.h"
#include "../GameFramework/DXTrace.h"


template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
class Camera
{
public:
	Camera()
	{
		m_transform.setScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
		m_transform.setRotation(XMFLOAT3(1.0f, 1.0f, 1.0f));
		m_transform.setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

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

public:
	void startRasterization(bool b, ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext);

private:
	Transform m_transform;
	static Camera camera;


	ComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;	// 光栅化状态: 线框模式
};


