#include "LightHelper.hlsli"

cbuffer VSConstantBuffer : register(b0)
{
	matrix g_World; // matrix可以用float4x4替代。不加row_major的情况下，矩阵默认为列主矩阵，
	matrix g_View;  // 可以在前面添加row_major表示行主矩阵
	matrix g_Proj;  // 该教程往后将使用默认的列主矩阵，但需要在C++代码端预先将矩阵进行转置。
	matrix g_WorldInvTranspose;

	int g_IsReflection;
	float3 g_Pad1;
	matrix g_Reflection;
}

cbuffer PSConstantBuffer : register(b1)
{
	DirectionalLight g_DirLight;
	PointLight g_PointLight;
	SpotLight g_SpotLight;
	Material g_Material;
	float3 g_EyePosW;
	float g_Pad;
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
