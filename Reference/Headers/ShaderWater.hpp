
float4x4	m_mtWld;
float4x4	m_mtViw;
float4x4	m_mtPrj;
float		m_fTime;


texture m_TxCbm;
samplerCUBE SmpCbm = sampler_state
{
	texture = (m_TxCbm);

	MinFilter	 = LINEAR;
	MagFilter	 = LINEAR;
	MipFilter	 = LINEAR;
	AddressU	 = Wrap;
	AddressV	 = Wrap;
};

// N Mapping Texture
texture m_TxNor;
sampler SmpNor =
sampler_state
{
	Texture = <m_TxNor>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


struct SVsOut
{
	float4 iPos : POSITION0;
	float2 Ds1 : TEXCOORD0;	// Distortion 1
	float2 Ds2 : TEXCOORD1;	// Distortion 2

	float3 Eye : TEXCOORD6;	// Eye Vector
	float3 Nor : TEXCOORD7;	// N Vector
};




static float4 wvK	  = float4( 0.002F, 0.001F, 0.01F, 0.004F);
static float4 wvSpeed = float4( 4.0F, 3.5F,  1.4F, 2.8F);
static float4 wvOmega = float4( 0.35F, 0.2F,  0.4F, 0.3F);



float3	WavePos(float3 Pos, float3 eps=float3(0,0,0))
{
	float3	tPos;
	float	r;
	float3	p;

	Pos +=eps;

	tPos = Pos;

	p = Pos - float3(-256, 0, 0);
	r = sqrt( p.x * p.x + p.z * p.z);
	tPos.y += exp(-wvK.x * r)*sin(r*wvOmega.x - m_fTime* wvSpeed.x);


	p = Pos - float3(256, 0, 0);
	r = sqrt( p.x * p.x + p.z * p.z);
	tPos.y += exp(-wvK.y * r)*sin(r*wvOmega.y - m_fTime* wvSpeed.y);


	p = Pos - float3(0, 0, 256);
	r = sqrt( p.x * p.x + p.z * p.z);
	tPos.y += exp(-wvK.z * r)*sin(r*wvOmega.z - m_fTime* wvSpeed.z);


	p = Pos - float3(0, 0, -256);
	r = sqrt( p.x * p.x + p.z * p.z);
	tPos.y += exp(-wvK.w * r)*sin(r*wvOmega.w - m_fTime* wvSpeed.w);

	return tPos;
}


SVsOut VtxPrc(float3 iPos : POSITION0)
{
	SVsOut Out = (SVsOut)0;				// Initialize to Zero

	float4	P = 0;
	float3	E = 1;


	float3	tPos;
	float3	tPosX;
	float3	tPosZ;

	float3	N= 0;

	tPos = WavePos(iPos);

	P = float4(tPos, 1);
	P = mul(P, m_mtWld);
	P = mul(P, m_mtViw);

	E = -normalize(P);

	P = mul(P, m_mtPrj);


	tPosX = WavePos(iPos, float3(1,0,0.1) );
	tPosX = normalize(tPosX - tPos);

	tPosZ = WavePos(iPos, float3(0,0,0.1) );
	tPosZ = normalize(tPosZ - tPos);

	N = cross( tPosX, tPosZ);
	N.x*=0.1F;
	N.z*=0.1F;
	N = normalize(N);

	N = mul(N, m_mtWld);
	N = mul(N, m_mtViw);
	N = normalize(N);


	Out.iPos	= P;
	Out.Eye		= E;
	Out.Nor		= N;


	float2	wvSpdU=float2(0.02f, +0.02f);
	float2	wvSpdV=float2(0.02f, -0.02f);
	float Time = m_fTime;
	float2	Tex= 1;
	float2	Ds1;
	float2	Ds2;

	Tex.x = iPos.x/16.;
	Tex.y = 1- iPos.z/16.;

	Ds1 = Tex.xy + wvSpdU * Time;
	Ds2 = Tex.yx + wvSpdV * Time;

	Out.Ds1 = Ds1;
	Out.Ds2 = Ds2;


	return Out;
}


// Normal Vector Processing
float3 NorPrc(float3 B, float3 T, float3 N, float2 Tex)
{
	float3 O = 0;
	float3 C = 2*tex2D(SmpNor, Tex).xyz-1;	// Extraction Bump Vector from N Map Texture

	O.x = B.x * C.x + T.x * C.y + N.x * C.z;
	O.y = B.y * C.x + T.y * C.y + N.y * C.z;
	O.z = B.z * C.x + T.z * C.y + N.z * C.z;

	return normalize(O);
}



float4	PxlPrc(SVsOut In) : COLOR0
{
	float4 Out = 1;

	float3 E = normalize(In.Eye);
	float3 B = {1,0,0};							// Base X direction Vector for Binormal vector
	float3 T = {0,1,0};							// Base Y direction Vector for Tangent vector
	float3 N = {0,0,1};							// Base Z direction Vector for N vector
	float3 N1;
	float3 N2;

	float3 R;
	float  F;

	float	n1	= 1.0;
	float	n2	= 1.333;

	F	= n2/n1;

	N = normalize(In.Nor);
	T = (cross(N, B));
	B = (cross(T, N));

	N1 = NorPrc(B, T, N, In.Ds1);
	N2 = NorPrc(B, T, N, In.Ds2);

	N	= normalize(N1 + N2);
	R	= reflect(-E, N);						// Reflection Vector

	//	F	= (n1-n2)*(n1-n2)/( (n1+n2) * (n1+n2) );
	//	F	= S + (1.0 - S) * pow(1.0 - dot(-E,N), 5);

	F	= pow(F - dot(-E, N), 2);				// Calcuate Simple Fresnel = (F - I.N)^2

	Out	= texCUBE( SmpCbm, R);
	Out.a = F;

	return Out;
}


technique Tech0
{
	pass P0
	{
		LIGHTING	= FALSE;
		CULLMODE	= NONE;

		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader= compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc();
	}
};
