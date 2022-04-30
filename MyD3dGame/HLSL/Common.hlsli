#include "LightHelper.hlsli"

cbuffer VSConstantBuffer : register(b0)
{
	matrix g_World; // matrix������float4x4���������row_major������£�����Ĭ��Ϊ��������
	matrix g_View;  // ������ǰ�����row_major��ʾ��������
	matrix g_Proj;  // �ý̳�����ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++�����Ԥ�Ƚ��������ת�á�
	matrix g_WorldInvTranspose;
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
	float3 posL : POSITION; //�ֲ�������λ��
	float3 normalL: NORMAL; //�ֲ������·���
	float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;//���������λ��
	float3 posW : POSITION; //����������λ��
	float3 normalW: NORMAL; //���������µķ���
	float2 tex : TEXCOORD;
};
