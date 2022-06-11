#include "LightHelper.hlsli"

cbuffer GoConstantBuffer : register(b0)
{
	matrix g_World; // matrix可以用float4x4替代。不加row_major的情况下，矩阵默认为列主矩阵，
	matrix g_WorldInvTranspose;
	Material g_Material;
}

cbuffer ViewTransform : register(b1)
{
	matrix g_View;
}

cbuffer ProjTransform : register(b2)
{
	matrix g_Proj;
}





cbuffer LightConstantBuffer : register(b3)
{
	DirectionalLight g_DirLight;
	PointLight g_PointLight;
	SpotLight g_SpotLight;
	float3 g_EyePosW;
	float g_Pad;
}

cbuffer RefectionConstantBuffer : register(b4)
{
	int g_IsReflection;
	float3 g_Pad1;
	matrix g_Reflection;
	int g_IsShadow;
	float3 g_Pad2;
	matrix g_Shadow;
	matrix g_RefShadow;

}


struct VertexIn
{
	float3 posL : POSITION; //局部坐标下位置
	float3 normalL: NORMAL; //局部坐标下法线
	float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;//齐次坐标下位置
	float3 posW : POSITION; //世界坐标下位置
	float3 normalW: NORMAL; //世界坐标下的法线
	float2 tex : TEXCOORD;
};
