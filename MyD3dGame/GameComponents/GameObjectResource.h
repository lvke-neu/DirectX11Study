#pragma once
#include <wrl/client.h>
#include <DirectXMath.h>
#include "../GameFramework/DXTrace.h"
#include "../GameFramework/d3dUtil.h"
#include "Geometry.h"
#include "LightHelper.h"
#include "RenderStates.h"

using namespace DirectX;
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
class GameObjectResource
{

public:
	struct  GoConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX worldInvTranspose;
		Material material;
	};

	struct RefectionConstantBuffer
	{
		int isReflection;
		XMFLOAT3 pad;
		XMMATRIX reflection;
		int isShadow;
		XMFLOAT3 pad2;
		XMMATRIX shadow;
		XMMATRIX refShadow;
	};

public:

	static void init(ComPtr<ID3D11Device> pd3dDevice, ComPtr<ID3D11DeviceContext> pd3dImmediateContext);
	static ComPtr<ID3D11Buffer> getGoConstantBuffer(XMMATRIX worldMatrix, Material material);

	static void getVertexBuffer(const Mesh& mesh, ComPtr<ID3D11Buffer>& vertexBuffer);
	static void getIndexBuffer(const Mesh& mesh, ComPtr<ID3D11Buffer>& indexBuffer);
	static void getShader(ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& vertexLayout, ComPtr<ID3D11PixelShader>& pixelShader);	// 顶点着色器 获取ID3D11VertexShader和ID3D11InputLayout
	static void getTexture(const wchar_t* texturePath, ComPtr<ID3D11ShaderResourceView>& texture);
	static void setIsReflectionAndIsShadow(bool bReflection, bool bShadow);


	static void SetWriteStencilOnly(UINT stencilRef, bool bShowMesh);
	static void SetRenderDefaultWithStencil(UINT stencilRef, bool bShowMesh);
	static void SetRenderNoDoubleBlend(UINT stencilRef, bool bShowMesh);
	static void SetRenderAlphaBlendWithStencil(UINT stencilRef, bool bShowMesh);
	static void SetRenderDefault(bool bShowMesh);
	
	
	
public:
	static ComPtr<ID3D11Buffer> m_pGoConstantBuffer;
	static ComPtr<ID3D11Buffer> m_pReflectionCB;
	static ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;
	static ComPtr<ID3D11Device> m_pd3dDevice;
	
	
};