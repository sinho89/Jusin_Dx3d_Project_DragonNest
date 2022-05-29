matrix	g_matWorldViewProj	: WorldViewProjection;
matrix	g_matLightViewProj	: LightViewProjection;
matrix	g_matWorld			: World;
matrix	g_matWorldIT		: WorldInverseTranspose;
matrix	g_matTexture		: Texture;

vector	g_vLightPos			: LightPosition;
vector	g_vEyePos			: EyePosition;
vector	g_vLightColor		: LightColor;

float2  g_vSampleOffsets[15];
float   g_fSampleWeights[15];

int		i_SampleChoice;


texture tShadowMap;
texture tScreenMap;
texture tBlurHMap;
texture tBlurVMap;
texture tColorMap;
texture	tPlayerHairMap;
texture	tPlayerHeadMap;
texture	tPlayerBodyMap;
texture	tPlayerGuntleMap;
texture	tPlayerHandMap;
texture	tPlayerLegMap;
texture	tPlayerBootMap;

sampler ShadowSampler = sampler_state
{
	texture = (tShadowMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler ScreenSampler = sampler_state
{
	texture = (tScreenMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler BlurHSampler = sampler_state
{
	texture = (tBlurHMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler BlurVSampler = sampler_state
{
	texture = (tBlurVMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler ColorSampler = sampler_state
{
	texture = (tColorMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler HairSampler = sampler_state
{
	texture = (tPlayerHairMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

	AddressU = Wrap;
	AddressV = Wrap;
};
sampler HeadSampler = sampler_state
{
	texture = (tPlayerHeadMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

	AddressU = Wrap;
	AddressV = Wrap;
};
sampler BodySampler = sampler_state
{
	texture = (tPlayerBodyMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

	AddressU = Wrap;
	AddressV = Wrap;
};
sampler GuntleSampler = sampler_state
{
	texture = (tPlayerGuntleMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

	AddressU = Wrap;
	AddressV = Wrap;
};
sampler HandSampler = sampler_state
{
	texture = (tPlayerHandMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

	AddressU = Wrap;
	AddressV = Wrap;
};
sampler LegSampler = sampler_state
{
	texture = (tPlayerLegMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

	AddressU = Wrap;
	AddressV = Wrap;
};
sampler BootSampler = sampler_state
{
	texture = (tPlayerBootMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

	AddressU = Wrap;
	AddressV = Wrap;
};


struct VSOUTPUT_SHADOW
{
	float4 vPosition	: POSITION;
	float  fDepth		: TEXCOORD0;
};

VSOUTPUT_SHADOW VS_Shadow( float4 inPosition : POSITION )
{
	VSOUTPUT_SHADOW OUT = (VSOUTPUT_SHADOW)0;

	OUT.vPosition = mul( inPosition, g_matLightViewProj );

	OUT.fDepth = OUT.vPosition.z;

	return OUT;
}

struct VSOUTPUT_UNLIT
{
	float4 vPosition	: POSITION;
	float4 vTexCoord	: TEXCOORD0;
};

VSOUTPUT_UNLIT VS_Unlit( float4 inPosition : POSITION )
{
	VSOUTPUT_UNLIT OUT = (VSOUTPUT_UNLIT)0;

	OUT.vPosition = mul( inPosition, g_matWorldViewProj );
		
	OUT.vTexCoord = mul( inPosition, g_matTexture );

	return OUT;
}

struct VSOUTPUT_BLUR
{
	float4 vPosition	: POSITION;
	float2 vTexCoord	: TEXCOORD0;
};

VSOUTPUT_BLUR VS_Blur( float4 inPosition : POSITION, float2 inTexCoord : TEXCOORD0 )
{
	VSOUTPUT_BLUR OUT = (VSOUTPUT_BLUR)0;

	OUT.vPosition = inPosition;

	OUT.vTexCoord = inTexCoord;

	return OUT;
}
		
struct VSOUTPUT_SCENE
{
	float4 vPosition		: POSITION;
	float2 vTexCoord		: TEXCOORD0;
	float4 vProjCoord		: TEXCOORD1;
	float4 vScreenCoord		: TEXCOORD2;
	float3 vNormal			: TEXCOORD3;
	float3 vLightVec		: TEXCOORD4;
	float3 vEyeVec			: TEXCOORD5;
};

VSOUTPUT_SCENE VS_Scene( float4 inPosition : POSITION, float3 inNormal : NORMAL, float2 inTexCoord : TEXCOORD0 )
{
	VSOUTPUT_SCENE OUT = (VSOUTPUT_SCENE)0;

	OUT.vPosition = mul( inPosition, g_matWorldViewProj );

	OUT.vTexCoord = inTexCoord;

	OUT.vProjCoord = mul( inPosition, g_matTexture );

	OUT.vScreenCoord.x = ( OUT.vPosition.x * 0.5 + OUT.vPosition.w * 0.5 );
	OUT.vScreenCoord.y = ( OUT.vPosition.w * 0.5 - OUT.vPosition.y * 0.5 );
	OUT.vScreenCoord.z = OUT.vPosition.w;
	OUT.vScreenCoord.w = OUT.vPosition.w;

	float4 vWorldPos = mul( inPosition, g_matWorld );

	OUT.vNormal = mul( inNormal, g_matWorldIT );

	OUT.vLightVec = g_vLightPos.xyz - vWorldPos.xyz;

	OUT.vEyeVec = g_vEyePos.xyz - vWorldPos.xyz;
	
	return OUT;
}

float4  PS_Shadow( VSOUTPUT_SHADOW IN ) : COLOR0
{
	return float4( IN.fDepth, IN.fDepth, IN.fDepth, 1.0f );
}
	
float4  PS_Unlit( VSOUTPUT_UNLIT IN ) : COLOR0
{
	float4 vTexCoords[9];

	float fTexelSize = 1.0f / 512.0f;

	vTexCoords[0] = IN.vTexCoord;

	vTexCoords[1] = IN.vTexCoord + float4( -fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[2] = IN.vTexCoord + float4(  fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[3] = IN.vTexCoord + float4( 0.0f, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[6] = IN.vTexCoord + float4( 0.0f,  fTexelSize, 0.0f, 0.0f );

	vTexCoords[4] = IN.vTexCoord + float4( -fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[5] = IN.vTexCoord + float4(  fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[7] = IN.vTexCoord + float4( -fTexelSize,  fTexelSize, 0.0f, 0.0f );
	vTexCoords[8] = IN.vTexCoord + float4(  fTexelSize,  fTexelSize, 0.0f, 0.0f );

	float fShadowTerm = 0.0f;

	for( int i = 0; i < 9; i++ )
	{
		float A = tex2Dproj( ShadowSampler, vTexCoords[i] ).r;
		float B = (IN.vTexCoord.z - 0.001f);
		
		fShadowTerm += A < B ? 0.1f : 1.0f;
	}

	fShadowTerm /= 9.0f;

	return fShadowTerm;
}

float4 PS_BlurH( VSOUTPUT_BLUR IN ) : COLOR0
{
	float4 vAccum = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	for( int i = 0; i < 15; i++ )
	{
		vAccum += tex2D( ScreenSampler, IN.vTexCoord + g_vSampleOffsets[i] ) * g_fSampleWeights[i];
	}

	return vAccum;
}

float4 PS_BlurV( VSOUTPUT_BLUR IN ) : COLOR0
{
	float4 vAccum = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	for( int i = 0; i < 15; i++ )
	{
		vAccum += tex2D( BlurHSampler, IN.vTexCoord + g_vSampleOffsets[i] ) * g_fSampleWeights[i];
	}

	return vAccum;
}

float4 PS_Scene( VSOUTPUT_SCENE IN ) : COLOR0
{
	IN.vNormal	 = normalize( IN.vNormal );
	IN.vLightVec = normalize( IN.vLightVec );
	IN.vEyeVec   = normalize( IN.vEyeVec );

	float4 vColor;

	if(i_SampleChoice == 0)
		vColor  = tex2D( HairSampler, IN.vTexCoord );
	if(i_SampleChoice == 1)
		vColor  = tex2D( HeadSampler, IN.vTexCoord );
	if(i_SampleChoice == 2)
		vColor  = tex2D( BodySampler, IN.vTexCoord );
	if(i_SampleChoice == 3)
		vColor  = tex2D( GuntleSampler, IN.vTexCoord );
	if(i_SampleChoice == 4)
		vColor  = tex2D( HandSampler, IN.vTexCoord );
	if(i_SampleChoice == 5)
		vColor  = tex2D( LegSampler, IN.vTexCoord );
	if(i_SampleChoice == 6)
		vColor  = tex2D( BootSampler, IN.vTexCoord );

	float diffuse  = max( dot( IN.vNormal, IN.vLightVec ), 0 );
	float specular = pow( max( dot( 2 * dot( IN.vNormal, IN.vLightVec ) * IN.vNormal - IN.vLightVec, IN.vEyeVec ), 0 ), 8 );

	if( diffuse == 0 ) specular = 0;

	float fShadowTerm = tex2Dproj( BlurVSampler, IN.vScreenCoord );

	return (diffuse * vColor * g_vLightColor * fShadowTerm) +
		   (specular * vColor * g_vLightColor.a * fShadowTerm);
}

float4 PS_SceneHard( VSOUTPUT_SCENE IN ) : COLOR0
{
	IN.vNormal	 = normalize( IN.vNormal );
	IN.vLightVec = normalize( IN.vLightVec );
	IN.vEyeVec   = normalize( IN.vEyeVec );

	float4 vColor;

	if(i_SampleChoice == 0)
		vColor  = tex2D( HairSampler, IN.vTexCoord );
	if(i_SampleChoice == 1)
		vColor  = tex2D( HeadSampler, IN.vTexCoord );
	if(i_SampleChoice == 2)
		vColor  = tex2D( BodySampler, IN.vTexCoord );
	if(i_SampleChoice == 3)
		vColor  = tex2D( GuntleSampler, IN.vTexCoord );
	if(i_SampleChoice == 4)
		vColor  = tex2D( HandSampler, IN.vTexCoord );
	if(i_SampleChoice == 5)
		vColor  = tex2D( LegSampler, IN.vTexCoord );
	if(i_SampleChoice == 6)
		vColor  = tex2D( BootSampler, IN.vTexCoord );

	float diffuse  = max( dot( IN.vNormal, IN.vLightVec ), 0 );
	float specular = pow( max( dot( 2 * dot( IN.vNormal, IN.vLightVec ) * IN.vNormal - IN.vLightVec, IN.vEyeVec ), 0 ), 8 );

	if( diffuse == 0 ) specular = 0;

	float fShadowTerm = 0.0f;
	fShadowTerm = tex2Dproj( ShadowSampler, IN.vProjCoord ) < (IN.vProjCoord.z - 0.001f) ? 0.1f : 1.0f;

	return (diffuse * vColor * g_vLightColor * fShadowTerm) +
		   (specular * vColor * g_vLightColor.a * fShadowTerm);
}

technique techShadow
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_3_0 VS_Shadow();
		PixelShader  = compile ps_3_0 PS_Shadow();
	}
}

technique techUnlit
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_3_0 VS_Unlit();
		PixelShader  = compile ps_3_0 PS_Unlit();
	}
}

technique techBlurH
{
	pass p0
	{
		Lighting	= False;
		CullMode	= None;
		
		VertexShader = compile vs_3_0 VS_Blur();
		PixelShader  = compile ps_3_0 PS_BlurH();
	}
}

technique techBlurV
{
	pass p0
	{
		Lighting	= False;
		CullMode	= None;
		
		VertexShader = compile vs_3_0 VS_Blur();
		PixelShader  = compile ps_3_0 PS_BlurV();
	}
}

technique techScene
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_3_0 VS_Scene();
		PixelShader  = compile ps_3_0 PS_Scene();
	}
}

technique techSceneHard
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_3_0 VS_Scene();
		PixelShader  = compile ps_3_0 PS_SceneHard();
	}
}