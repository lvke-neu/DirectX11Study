#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"


GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
	if (m_box)
		delete m_box;

	for (int i = 0; i < 4; ++i)
		if (m_around[i])
			delete m_around[i];

	if (m_ground)
		delete m_ground;
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	/*
	1、初始化相机
	2、初始化光源
	*/
	Camera::getInstance().setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -5.0f)));

	InitResource();

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
#ifdef USE_IMGUI
	ImGuiIO& io = ImGui::GetIO();


	static XMFLOAT3 rot = Camera::getInstance().getRotation();
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		rot.y += io.MouseDelta.x * dt * 10;
		rot.x += io.MouseDelta.y * dt * 10;
	}
	Camera::getInstance().setRotation(rot);
	

	if (ImGui::IsKeyDown('W'))
		Camera::getInstance().moveForward(dt * 10);
	if (ImGui::IsKeyDown('S'))
		Camera::getInstance().moveForward(-dt * 10);
	if (ImGui::IsKeyDown('A'))
		Camera::getInstance().moveRight(-dt * 10);
	if (ImGui::IsKeyDown('D'))
		Camera::getInstance().moveRight(dt * 10);

	XMFLOAT3 adjustedPos;
	XMStoreFloat3(&adjustedPos, XMVectorClamp(XMLoadFloat3(&Camera::getInstance().getPosition()), XMVectorSet(-8.9f, 0.0f, -8.9f, 0.0f), XMVectorReplicate(8.9f)));
	Camera::getInstance().setPosition(adjustedPos);

	
	ImGui::Begin("Camera");
	ImGui::Text("Camera Position\n%.2f %.2f %.2f", Camera::getInstance().getPosition().x, Camera::getInstance().getPosition().y, Camera::getInstance().getPosition().z);
	ImGui::End();
	
	ImGui::Render();
#endif



	/*盒子*/
	m_box->UpdateWorldViewProjMatrix(AspectRatio());

	/*四面墙*/
	for (int i = 0; i < 4; ++i)
	{
		m_around[i]->UpdateWorldViewProjMatrix(AspectRatio());

	}

	/*地板*/
	m_ground->UpdateWorldViewProjMatrix(AspectRatio());
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);
	static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	/*盒子*/
	m_box->BindToRenderPipeline();
	m_box->Draw(m_boxMesh.indexbuffer.size());

	/*四面墙*/
	for (int i = 0; i < 4; ++i)
	{
		m_around[i]->BindToRenderPipeline();
		m_around[i]->Draw(m_aroundMesh.indexbuffer.size());
	}

	/*地板*/
	m_ground->BindToRenderPipeline();
	m_ground->Draw(m_groundMesh.indexbuffer.size());


#ifdef USE_IMGUI
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	HR(m_pSwapChain->Present(0, 0));
}

void GameApp::InitResource()
{
	/*盒子*/
	if (m_box)
		delete m_box;
	m_box = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_box->CreateBuffer(m_boxMesh);
	m_box->CreateTexture(L"Texture\\WoodCrate.dds");
	m_box->CreateShader();
	m_box->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	m_box->UpdateWorldViewProjMatrix(AspectRatio());

	/*四面墙*/
	for (int i = 0; i < 4; ++i)
		if (m_around[i])
			delete m_around[i];
	for (int i = 0; i < 4; ++i)
	{
		m_around[i] = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
		m_around[i]->CreateBuffer(m_aroundMesh);
		m_around[i]->CreateTexture(L"Texture\\brick.dds");
		m_around[i]->CreateShader();
		m_around[i]->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-XM_PIDIV2, XM_PIDIV2 * i, 0.0f), XMFLOAT3(i % 2 ? -10.0f * (i - 2) : 0.0f, 3.0f, i % 2 == 0 ? -10.0f * (i - 1) : 0.0f)));
		m_around[i]->UpdateWorldViewProjMatrix(AspectRatio());

	}

	/*地板*/
	if (m_ground)
		delete m_ground;
	m_ground = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_ground->CreateBuffer(m_groundMesh);
	m_ground->CreateTexture(L"Texture\\checkboard.dds");
	m_ground->CreateShader();
	m_ground->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)));
	m_ground->UpdateWorldViewProjMatrix(AspectRatio());
}