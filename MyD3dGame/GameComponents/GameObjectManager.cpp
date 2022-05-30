#include "GameObjectManager.h"

GameObjectManager GameObjectManager::gameObjectManager;


void GameObjectManager::init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pd3dImmediateContext = pd3dImmediateContext;


	/*盒子*/
	m_box = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_box->init(m_boxMesh, L"Texture\\WoodCrate.dds",
		Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)), m_aspectRatio);
	m_goVector.push_back(m_box);
	/*四面墙*/
	for (int i = 0; i < 4; ++i)
	{
		m_around[i] = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
		m_around[i]->init(m_aroundMesh, L"Texture\\brick.dds",
			Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, XM_PIDIV2 * i, 0.0f), XMFLOAT3(i % 2 ? -10.0f * (i - 2) : 0.0f, 3.0f, i % 2 == 0 ? -10.0f * (i - 1) : 0.0f)),
			m_aspectRatio);
		m_goVector.push_back(m_around[i]);
	}

	/*地板*/
	m_ground = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_ground->init(m_groundMesh, L"Texture\\checkboard.dds",
		Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)),
		m_aspectRatio);
	m_goVector.push_back(m_ground);
}

void GameObjectManager::onUpdateTime(float dt)
{
	/*
	让盒子动起来
	*/
	static float rotx = 0;
	static float roty = 0;

	rotx += dt;
	roty += dt;

	XMFLOAT3 boxRot = m_box->getRotation();
	boxRot.x = rotx;
	boxRot.y = roty;

	m_box->setRotation(boxRot);
	for (auto go : m_goVector)
	{
		if (go)
		{
			go->updateWorldViewProjMatrix(m_aspectRatio);
		}
	}
}

void GameObjectManager::draw()
{
	/*盒子*/
	m_box->draw(m_boxMesh.indexbuffer.size());
	/*四面墙*/
	for (int i = 0; i < 4; ++i)
	{
		m_around[i]->draw(m_aroundMesh.indexbuffer.size());
	}
	/*地板*/
	m_ground->draw(m_groundMesh.indexbuffer.size());
}

GameObjectManager::~GameObjectManager()
{
	for (auto go : m_goVector)
	{
		if (go)
		{
			delete go;
		}
	}
}