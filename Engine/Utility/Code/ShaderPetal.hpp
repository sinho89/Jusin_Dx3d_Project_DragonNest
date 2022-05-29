#include "ShaderBase.hpp"

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;
float		g_fPower;
float		g_fAlpha;

vector		g_vCamPos;
float		g_fDetail;

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
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
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV * g_fDetail;

	//ºû~
	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector		vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCamPos;

	Out.vSpecular = pow(max(dot(normalize(vLook * -1.f), normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);
	return Out;
}

struct PS_IN
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = Out.vColor * In.vShade + In.vSpecular;
	Out.vColor.a = g_fAlpha;

	return Out;
}

struct VS_OUT_ALPHA
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

struct PS_IN_ALPHA
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_ALPHA VS_MAIN_ALPHATEST(VS_IN In)
{
	VS_OUT_ALPHA		Out = (VS_OUT_ALPHA)0;

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

	return Out;
}

PS_OUT PS_MAIN_ALPHATEST(PS_IN_ALPHA In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = Out.vColor * In.vShade + In.vSpecular;

	return Out;
}

technique Petal
{
	pass p1
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass p2
	{
		Alphablendenable = true;
		Alphatestenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;
		Alpharef = 0x00000088;
		Alphafunc = GREATER;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHATEST();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}
}