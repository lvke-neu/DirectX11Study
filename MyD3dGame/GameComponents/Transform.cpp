#include "Transform.h"

Transform::Transform()
{
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(const XMFLOAT3& scale, const XMFLOAT3& rotation, const XMFLOAT3& position)
{
	m_scale = scale;
	m_rotation = rotation;
	m_position = position;
}

Transform::Transform(const Transform& transform)
{
	m_scale = transform.m_scale;
	m_rotation = transform.m_rotation;
	m_position = transform.m_position;
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

bool Transform::operator==(const Transform& transform)
{
	return (m_scale.x == transform.m_scale.x && m_scale.y == transform.m_scale.y && m_scale.z == transform.m_scale.z &&
		m_rotation.x == transform.m_rotation.x && m_rotation.y == transform.m_rotation.y && m_rotation.z == transform.m_rotation.z &&
		m_position.x == transform.m_position.x && m_position.y == transform.m_position.y && m_position.z == transform.m_position.z);
}
