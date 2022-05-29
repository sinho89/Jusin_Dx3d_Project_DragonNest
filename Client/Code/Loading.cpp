#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include "Include.h"


CLoading::CLoading(LOADID eLoadID)
: m_pDevice(NULL)
, m_eLoadID(eLoadID)
, m_hThread(NULL)
, m_bComplete(false)
{
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
	ZeroMemory(m_szLoadingMessage, sizeof(TCHAR) * 128);
}

CLoading::CLoading(void)
{
	Release();
}

const TCHAR* CLoading::GetLoadingMessage(void)
{
	return m_szLoadingMessage;
}

bool CLoading::GetComplete(void)
{
	return m_bComplete;
}

HRESULT CLoading::InitLoading(void)
{
	InitializeCriticalSection(&m_CSKey);

	m_pDevice = Engine::Get_GraphicDev()->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
	NULL_CHECK_RETURN(m_hThread, E_FAIL);

	return S_OK;
}

void CLoading::StageLoading1(void)
{
	HRESULT	hr = NULL;
	// buffer
	lstrcpy(m_szLoadingMessage, L"Buffer Loading....");
	
	Engine::Get_ResourceMgr()->AddBuffer(m_pDevice
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_CUBETEX
		, L"Buffer_CubeTex");

	Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_Stage_RcTex");

	//texture
	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_MainBar"
		, L"../bin/Resources/Texture/mainbarback.png", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Aim"
		, L"../bin/Resources/Texture/crosshair.png", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Petal"
		, L"../bin/Resources/Texture/alpha.png", 1);


		Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Skill"
		, L"../bin/Resources/Texture/cycle.png", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Player_Hair"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/npc_wacolosseum_hair.tga", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Player_Head"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/npc_wacolosseum_head.tga", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Player_Body"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/npc_wacolosseum_body.tga", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Player_Guntle"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/npc_wacolosseum_gauntlet.tga", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Player_Hand"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/npc_wacolosseum_glove.tga", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Player_Leg"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/npc_wacolosseum_leg.tga", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Player_Boots"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/npc_wacolosseum_boots.tga", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Trail"
		, L"../bin/Resources/Texture/swordtrail02-b.png", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Fire_Alpha"
		, L"../bin/Resources/Texture/swordtrail02-b.png", 1);

	Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Fire_Noise"
		, L"../bin/Resources/Texture/alpha.png", 1);


	// Mesh
	lstrcpy(m_szLoadingMessage, L"Mesh Loading....");	
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Terrain"
		, L"../bin/Resources/Mesh/StaticMesh/Terrain/"
		, L"Terrain2.x"
		, Engine::OBJ_END);

	
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain1"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_Tree1.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain2"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_Tree2.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain3"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_Church.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain4"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_HomeWall1.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain5"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_HomeWall2.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain6"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_HomeWall3.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain7"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_HomeWall4.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain8"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_Bridge.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain9"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_BossRoom1.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain10"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_BossRoom2.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain11"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_BossRoom3.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Obtain12"
		, L"../bin/Resources/Mesh/StaticMesh/Obtain/"
		, L"Obtain_BossRoom4.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Hit"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"Hit.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_HarfMoon"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"HarfMoon.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_HarfCasting"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"HarfCasting.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_HarfDust"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"HarfDust.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_HarfWind"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"HarfWind.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Hurrycane"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"Hurricane.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_CresentCasting"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"CresentCasting.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Cresent"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"Cresent.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_HarfAura"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"HarfAura.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_CresentAura"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"CresentAura.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_CresentDust"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"CresentDust.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Great"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"Great.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_GreatHit"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"GreatHit.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_GreatAura"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"RingAura.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_GreatRing"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"Ring.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );


	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Wind"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"Wind.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Cycle"
		, L"../bin/Resources/Mesh/StaticMesh/Effect/"
		, L"Cycle.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_SkyBox"
		, L"../bin/Resources/Mesh/StaticMesh/SkySphere/"
		, L"SkyBox.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );
	//SkyBox

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Door"
		, L"../bin/Resources/Mesh/DynamicMesh/Obtain/"
		, L"Door.x"
		, Engine::OBJ_END);
	FAILED_CHECK_RETURN(hr, );
	//Door

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Player"
		, L"../bin/Resources/Mesh/DynamicMesh/Player/"
		, L"Player.x"
		, Engine::OBJ_PLAYER);

	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Org"
		, L"../bin/Resources/Mesh/DynamicMesh/Monster/"
		, L"Orge.x"
		, Engine::OBJ_ORGE);

	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Goblin"
		, L"../bin/Resources/Mesh/DynamicMesh/Monster/"
		, L"Goblin.x"
		, Engine::OBJ_GOBLIN);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_Boss"
		, L"../bin/Resources/Mesh/DynamicMesh/Boss/"
		, L"BlackDragon.x"
		, Engine::OBJ_BOSS);

	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_Weapon"
		, L"../bin/Resources/Mesh/StaticMesh/"
		, L"Weapon.x"
		, Engine::OBJ_END);

	FAILED_CHECK_RETURN(hr, );
	

	lstrcpy(m_szLoadingMessage, L"Complete!!");
	

	m_bComplete = true;

}

void CLoading::StageLoading2(void){}
void CLoading::StageLoading3(void){}

CLoading* CLoading::Create(LOADID eLoadID)
{
	CLoading*		pLoading = new CLoading(eLoadID);

	if(FAILED(pLoading->InitLoading()))
		Engine::Safe_Delete(pLoading);

	return pLoading;
}

UINT __stdcall CLoading::LoadingFunction(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	//EnterCriticalSection(&pLoading->m_CSKey);

	switch(pLoading->m_eLoadID)
	{
	case CLoading::LOADID_STAGE:
		pLoading->StageLoading1();
		break;

	case CLoading::LOADID_STAGE1:
		pLoading->StageLoading2();
		break;

	case CLoading::LOADID_STAGE2:
		pLoading->StageLoading3();
		break;
	}

	//LeaveCriticalSection(&pLoading->m_CSKey);

	return 0;
}

void CLoading::Release(void)
{
	DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
}

