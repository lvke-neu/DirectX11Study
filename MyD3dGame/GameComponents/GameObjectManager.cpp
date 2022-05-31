#include "GameObjectManager.h"
#include "../GameFramework/RenderStates.h"
GameObjectManager GameObjectManager::gameObjectManager;


void GameObjectManager::init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pd3dImmediateContext = pd3dImmediateContext;

	Material material; 
	material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	/*盒子*/
	m_box = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_box->setMesh(m_boxMesh);
	m_box->setShader();
	m_box->setTexture(L"Texture\\WoodCrate.dds");
	m_box->setMaterial(material);
	m_box->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 2.0f, 0.0f)));
	m_goVector.push_back(m_box);


	/*铁丝围栏*/
	m_wireFence = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_wireFence->setMesh(m_boxMesh);
	m_wireFence->setShader();
	m_wireFence->setTexture(L"Texture\\WireFence.dds");
	m_wireFence->setMaterial(material);
	m_goVector.push_back(m_wireFence);


	/*地板*/
	m_ground = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_ground->setMesh(m_groundMesh);
	m_ground->setShader();
	m_ground->setTexture(L"Texture\\floor.dds");
	m_ground->setMaterial(material);
	m_ground->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)));
	m_goVector.push_back(m_ground);


	/*四面墙*/
	for (int i = 0; i < 4; ++i)
	{
		m_around[i] = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
		m_around[i]->setMesh(m_aroundMesh);
		m_around[i]->setShader();
		m_around[i]->setTexture(L"Texture\\brick.dds");
		m_around[i]->setMaterial(material);
		m_around[i]->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, XM_PIDIV2 * i, 0.0f), XMFLOAT3(i % 2 ? -10.0f * (i - 2) : 0.0f, 3.0f, i % 2 == 0 ? -10.0f * (i - 1) : 0.0f)));
		m_goVector.push_back(m_around[i]);
	}




	material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	material.specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	/*水*/
	m_water = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_water->setMesh(m_groundMesh);
	m_water->setShader();
	m_water->setTexture(L"Texture\\water.dds");
	m_water->setMaterial(material);
	m_water->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)));
	m_goVector.push_back(m_water);
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

void GameObjectManager::draw(bool bShowMesh)
{
	//绘制不透明对象
	m_pd3dImmediateContext->RSSetState(nullptr);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	if (bShowMesh)
		m_pd3dImmediateContext->RSSetState(RenderStates::RSWireframe.Get());

	m_box->draw();
	for (auto around : m_around)
	{
		around->draw();
	}
	m_ground->draw();


	//绘制透明对象
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);
	if (bShowMesh)
		m_pd3dImmediateContext->RSSetState(RenderStates::RSWireframe.Get());


	m_wireFence->setPosition(XMFLOAT3(2.0f, 1.0f, 0.0f));
	m_wireFence->updateWorldViewProjMatrix(m_aspectRatio);
	m_wireFence->draw();

	m_wireFence->setPosition(XMFLOAT3(-2.0f, 1.0f, 0.0f));
	m_wireFence->updateWorldViewProjMatrix(m_aspectRatio);
	m_wireFence->draw();

	m_water->draw();
	
	

	//for (auto go : m_goVector)
	//{
	//	if (go)
	//	{
	//		go->draw();
	//	}
	//}
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