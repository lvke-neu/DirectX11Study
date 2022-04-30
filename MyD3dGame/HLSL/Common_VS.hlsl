#include "Common.hlsli"

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	float4 posW = mul(float4(vIn.posL, 1.0f), g_World);
	vOut.posH = mul(posW, g_View);
	vOut.posH = mul(vOut.posH, g_Proj);
	vOut.posW = posW.xyz;
	vOut.normalW = mul(vIn.normalL, (float3x3) g_WorldInvTranspose);
	vOut.tex = vIn.tex;
	return vOut;
}
