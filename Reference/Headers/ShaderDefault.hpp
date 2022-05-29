#include "ShaderBase.hpp"

float		g_fAlpha = 1.f;

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
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT VS_MAIN_PARTICLE(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
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
	Out.vColor.a = g_fAlpha;

	return Out;
}
PS_OUT PS_MAIN_PARTICLE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}
technique Default_Technique
{
	pass Default
	{

		/*LPDIRECT3DDEVICE9	pDevice;
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);*/

/*
		srcalpha = SRC.A = 0.5;
		invsrcalpha = (1 - SRC.A);
*/

		/*SRCRGB * SRC.A + DESTRGB * (1 - SRC.A);*/

		/*D3DBLEND_ONE		(r = 1 g = 1 b = 1 a = 1)
		D3DBLEND_ZERO		(r = 0 g = 0 b = 0 a = 0)*/

		/*결과색상 = SRC색상 * SRC블랜딩 계수 + 대상색상 * 대상 블랜딩 계수;*/

		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass DefaultAlphaTest
	{
		Alphablendenable = true;
		Alphatestenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;
		Alpharef = 0x00000088;
		Alphafunc = GREATER;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass DefaultParticle
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN_PARTICLE();
		PixelShader = compile ps_3_0 PS_MAIN_PARTICLE();
	}
};

/*
technique asdf
{
	pass Default
	{
		VertexShader = compile vs_2_0 VS_MAIN();
	}
};*/