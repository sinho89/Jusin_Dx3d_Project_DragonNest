
#include "ShaderBase.hpp"

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;
float		g_fPower;

vector		g_vCamPos;

float		g_fDetail;

vector		g_vBrushPos;
float		g_fBrushSize;


texture		g_SrcTexture;
texture		g_DestTexture;
texture		g_MultiTexture;
texture		g_BrushTexture;

sampler BrushSampler = sampler_state
{
	texture = g_BrushTexture;
	/*m_pDevice->SetSamplerState();*/
	minfilter = linear;
	magfilter = linear;
};

sampler SrcSampler = sampler_state
{
	texture = g_SrcTexture;
	/*m_pDevice->SetSamplerState();*/
	minfilter = linear;
	magfilter = linear;
};

sampler DestSampler = sampler_state
{
	texture = g_DestTexture;
	/*m_pDevice->SetSamplerState();*/
	minfilter = linear;
	magfilter = linear;
};

sampler MultiSampler = sampler_state
{
	texture = g_MultiTexture;
	/*m_pDevice->SetSamplerState();*/
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	//ºû~
	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector		vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = pow(max(dot(normalize(vLook * -1.f), normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);
	Out.vWorldPos = vWorldPos;
	return Out;
}

struct PS_IN
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	vector		vColor[4];
	vColor[0] = tex2D(SrcSampler, In.vTexUV * g_fDetail);
	vColor[1] = tex2D(DestSampler, In.vTexUV * g_fDetail);
	vColor[2] = tex2D(MultiSampler, In.vTexUV);
	vColor[3] = (vector)0.f;
	/*vColor[3] = tex2D(BrushSampler, In.vTexUV);*/

	if(g_vBrushPos.x - g_fBrushSize < In.vWorldPos.x && In.vWorldPos.x < g_vBrushPos.x + g_fBrushSize
		&& g_vBrushPos.z - g_fBrushSize < In.vWorldPos.z && In.vWorldPos.z < g_vBrushPos.z + g_fBrushSize)
	{
		float2		vNewUV = (float2)0.f;

		vNewUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushSize)) / (g_fBrushSize * 2.f);
		vNewUV.y = ((g_vBrushPos.z + g_fBrushSize) - In.vWorldPos.z) / (g_fBrushSize * 2.f);

		vColor[3] = tex2D(BrushSampler, vNewUV);
	}

	vector		vResultColor = (vector)0.f;
	vResultColor = vColor[0] * vColor[2] + vColor[1] * (1.f - vColor[2]);

	Out.vColor = vResultColor * In.vShade + In.vSpecular + vColor[3];

	return Out;
}

technique Default_Technique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};