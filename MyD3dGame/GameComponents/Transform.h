#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Transform
{

public:
	Transform();
	Transform(const Transform& transform);
	Transform(const XMFLOAT3& scale, const XMFLOAT3& rotation, const XMFLOAT3& position);
	Transform& operator=(const Transform& transform);
	XMMATRIX getWorldMatrix();

	inline XMFLOAT3 getScale() { return m_scale; }
	void setScale(const XMFLOAT3& scale) { m_scale = scale; }
	inline XMFLOAT3 getRotation() { return m_rotation; }
	void setRotation(const XMFLOAT3& rotation) { m_rotation = rotation; }
	inline XMFLOAT3 getPosition() { return m_position; }
	void setPosition(const XMFLOAT3& position) { m_position = position; }

private:
	XMFLOAT3 m_scale;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_position;
};