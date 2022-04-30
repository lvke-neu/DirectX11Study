#include "Transform.h"

Transform::Transform()
{
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(const Transform& transform)
{
	m_scale = transform.m_scale;
	m_rotation = transform.m_rotation;
	m_position = transform.m_position;
}

Transform::Transform(const XMFLOAT3& scale, const XMFLOAT3& rotation, const XMFLOAT3& position)
{
	m_scale = scale;
	m_rotation = rotation;
	m_position = position;
}

Transform& Transform::operator=(const Transform& transform)
{
	m_scale = transform.m_scale;
	m_rotation = transform.m_rotation;
	m_position = transform.m_position;
	return *this;
}

XMMATRIX Transform::getWorldMatrix()
{
	return	XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)) *
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotation)) *
		XMMatrixTranslationFromVector(XMLoadFloat3(&m_position));
}
