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
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

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
	ImGui::Begin("Lighting");
	ImGui::Text("Material");
	ImGui::End();
	ImGui::Render();
#endif

	static float sumDt = 0;
	sumDt += dt;
	//让立方体动起来
	m_box->setRotation(XMFLOAT3(XM_PI / 4 * sumDt, 0, 0));
	m_box->UpdateWorldViewProjMatrix(AspectRatio());


}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);
	static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

#ifdef USE_IMGUI
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	m_box->Draw(m_boxMesh.indexbuffer.size());

	HR(m_pSwapChain->Present(0, 0));
}

void GameApp::InitResource()
{
	if (m_box)
		delete m_box;
	m_box = new GameObject(m_pd3dDevice, m_pd3dImmediateContext);
	m_box->CreateBuffer(m_boxMesh);
	m_box->CreateTexture(L"Texture\\WoodCrate.dds");
	m_box->CreateShader();
	m_box->setTransform(Transform(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
	m_box->UpdateWorldViewProjMatrix(AspectRatio());
	m_box->UpdateLight(DLight);
	m_box->BindToRenderPipeline();
}