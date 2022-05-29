#include "stdafx.h"
#include "Management.h"
#include "Stage.h"
#include "Export_Function.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "StaticMesh.h"
#include "CollisionMgr.h"
#include "Collision.h"
#include "Stage.h"
#include "GameObject.h"
#include "TimeMgr.h"
#include "DynamicMesh.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr(void)
{
	ZeroMemory(m_OBB, sizeof(OBB) * 2);
}

CCollisionMgr::~CCollisionMgr(void)
{

}

bool CCollisionMgr::Collision_AABB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax)
{
	float		fMin = 0.f;
	float		fMax = 0.f;

	fMin = max(pDestMin->x, pSourMin->x);
	fMax = min(pDestMax->x, pSourMax->x);
	if(fMin > fMax)
		return false;

	fMin = max(pDestMin->y, pSourMin->y);
	fMax = min(pDestMax->y, pSourMax->y);
	if(fMin > fMax)
		return false;

	fMin = max(pDestMin->z, pSourMin->z);
	fMax = min(pDestMax->z, pSourMax->z);
	if(fMin > fMax)
		return false;

	return true;
}

bool CCollisionMgr::Collision_OBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax, const D3DXMATRIX* pDestWorld , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax, const D3DXMATRIX* pSourWorld)
{
	ZeroMemory(m_OBB, sizeof(OBB) * 2);

	Init_Point(&m_OBB[0], pDestMin, pDestMax);
	Init_Point(&m_OBB[1], pSourMin, pSourMax);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_OBB[0].vPoint[i], &m_OBB[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&m_OBB[1].vPoint[i], &m_OBB[1].vPoint[i], pSourWorld);
	}

	D3DXVec3TransformCoord(&m_OBB[0].vCenter, &m_OBB[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&m_OBB[1].vCenter, &m_OBB[1].vCenter, pSourWorld);

	Init_Axis(&m_OBB[0]);
	Init_Axis(&m_OBB[1]);

	float	fDistance[3];

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[0], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[1], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[0].vProjAxis[2], &m_OBB[i].vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[0], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[1], &m_OBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_OBB[1].vProjAxis[2], &m_OBB[i].vAxis[j]));

			D3DXVECTOR3		vTemp = m_OBB[1].vCenter - m_OBB[0].vCenter;

			fDistance[2] = fabs(D3DXVec3Dot(&vTemp, &m_OBB[i].vAxis[j]));

			if(fDistance[2] > fDistance[1] + fDistance[0])
				return false;
		}
	}
	return true;
}

bool CCollisionMgr::Collision_Camera(LPD3DXMESH* pMesh, const D3DXMATRIX* pWorldMatrix)
{
	D3DXVECTOR3 vRayPos, vRayDir;

	FLOAT fLength;
	
	vRayPos = *Engine::CCameraMgr::GetInstance()->GetRayPos();
	vRayDir = *Engine::CCameraMgr::GetInstance()->GetRayDir();
	fLength = Engine::CCameraMgr::GetInstance()->GetDistance();

	BOOL	bCollision;
	FLOAT	fDistance;

	D3DXIntersect(*pMesh, &vRayPos, &vRayDir, &bCollision, NULL, NULL, NULL, &fDistance, NULL, NULL);

	if(bCollision)
	{
		if(fDistance - 10 <= fLength)
		{
			Engine::CCameraMgr::GetInstance()->SetDistance(fDistance);
			return true;
		}
	}

	return false;
}


void CCollisionMgr::Collision_MonsterToPlayer_MoveCheck()
{
	float	fTime = Engine::Get_TimeMgr()->GetTime();

	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vMonsterPos = ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos;
		float fDistance = D3DXVec3Length( &(vPlayerPos - vMonsterPos));

		if(fDistance < 50)
		{
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bAttackSight = true;
			((Engine::CTransform*)pComponent)->m_vPos += ((Engine::CTransform*)pComponent)->m_vDir * -1 * ((Engine::CTransform*)pComponent)->m_fSpeed * fTime;
		}
		else
		{
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bAttackSight = false;
		}
	}
}

void CCollisionMgr::Collision_MonsterToPlayer_SightCheck()
{
	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vMonsterPos = ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos;
		float fDistance = D3DXVec3Length( &(vPlayerPos - vMonsterPos));

		if(fDistance < 500)
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bSight = true;
	}
}

void CCollisionMgr::Coliision_MonsterToPlayer_AttackRangeCheck()
{
	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vMonsterPos = ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos;
		float fDistance = D3DXVec3Length( &(vPlayerPos - vMonsterPos));

		if(fDistance < 150
			&& ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bAttackCasting == true)
		{
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bAttRange = true;
		}
		else
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bAttRange = false;

	}
}
void CCollisionMgr::ReviveMonster()
{
	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		if(((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bAlive == false)
		{
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bRevive = true;
		}
	}
}
void CCollisionMgr::Collision_MonsterToMonster_MoveCheck()
{
	float	fTime = Engine::Get_TimeMgr()->GetTime();

	list<Engine::CGameObject*>::iterator Dest_iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator Dest_iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; Dest_iter != Dest_iter_end; ++Dest_iter)
	{
		list<Engine::CGameObject*>::iterator Sour_iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
		list<Engine::CGameObject*>::iterator Sour_iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();
		
		for(; Sour_iter != Sour_iter_end; ++Sour_iter)
		{
			if((*Dest_iter) == (*Sour_iter))
				continue;

			D3DXVECTOR3 vDestPos = ((Engine::CTransform*)(*Dest_iter)->GetComponent(L"Transform"))->m_vPos;
			D3DXVECTOR3 vSourPos = ((Engine::CTransform*)(*Sour_iter)->GetComponent(L"Transform"))->m_vPos;
			float fDistance = D3DXVec3Length( &(vDestPos - vSourPos));

			if(fDistance < 50)
			{
				D3DXVECTOR3 vDestDir = vDestPos - vSourPos;
				D3DXVECTOR3 vSourDir = vSourPos - vDestPos;

				D3DXVec3Normalize(&vDestDir, &vDestDir);
				D3DXVec3Normalize(&vSourDir, &vSourDir);

				if(!((Engine::CTransform*)(*Dest_iter)->GetComponent(L"Transform"))->m_bAttackCasting)
					((Engine::CTransform*)(*Dest_iter)->GetComponent(L"Transform"))->m_vPos += vDestDir * ((Engine::CTransform*)(*Dest_iter)->GetComponent(L"Transform"))->m_fSpeed * fTime;
				if(!((Engine::CTransform*)(*Sour_iter)->GetComponent(L"Transform"))->m_bAttackCasting)
					((Engine::CTransform*)(*Sour_iter)->GetComponent(L"Transform"))->m_vPos += vSourDir * ((Engine::CTransform*)(*Sour_iter)->GetComponent(L"Transform"))->m_fSpeed * fTime;
			}
		}
	}
}

void CCollisionMgr::Collision_BossToPlayer()
{
	float	fTime = Engine::Get_TimeMgr()->GetTime();

	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");
	const Engine::CComponent* pBossMeshComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"DynamicMesh");

	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);
	Engine::CDynamicMesh* pBossMesh = ((Engine::CDynamicMesh*)pBossMeshComponent);

	const D3DXMATRIX* pmatBossWorld = &pBossInfo->m_matWorld;

	m_pmatBossBone[BOSS_HAIR] = pBossMesh->FindFrame("Hair_Bone_14");
	m_pmatBossBone[BOSS_HEAD] = pBossMesh->FindFrame("Bip01_Head");
	m_pmatBossBone[BOSS_NECK1] = pBossMesh->FindFrame("Bip01_Neck1");
	m_pmatBossBone[BOSS_NECK2] = pBossMesh->FindFrame("Bip01_Neck");
	m_pmatBossBone[BOSS_R_HAND] = pBossMesh->FindFrame("Bip01_R_Hand");
	m_pmatBossBone[BOSS_L_HAND] = pBossMesh->FindFrame("Bip01_L_Hand");
	m_pmatBossBone[BOSS_SPINE_1] = pBossMesh->FindFrame("Bip01_Spine1");
	m_pmatBossBone[BOSS_SPINE_2] = pBossMesh->FindFrame("Spin_Bone_01");
	m_pmatBossBone[BOSS_R_FOOT] = pBossMesh->FindFrame("Bip01_R_Foot");
	m_pmatBossBone[BOSS_L_FOOT] = pBossMesh->FindFrame("Bip01_L_Foot");
	m_pmatBossBone[BOSS_TAIL_1] = pBossMesh->FindFrame("Tail_Bone_01");
	m_pmatBossBone[BOSS_TAIL_2] = pBossMesh->FindFrame("Tail_Bone_02");
	m_pmatBossBone[BOSS_TAIL_3] = pBossMesh->FindFrame("Tail_Bone_03");
	m_pmatBossBone[BOSS_TAIL_4] = pBossMesh->FindFrame("Tail_Bone_04");
	m_pmatBossBone[BOSS_TAIL_5] = pBossMesh->FindFrame("Tail_Bone_05");
	m_pmatBossBone[BOSS_TAIL_6] = pBossMesh->FindFrame("Tail_Bone_06");
	m_pmatBossBone[BOSS_TAIL_7] = pBossMesh->FindFrame("Tail_Bone_07");
	m_pmatBossBone[BOSS_TAIL_8] = pBossMesh->FindFrame("Tail_Bone_08");

	for(int i = 0; i < BOSS_END; ++i)
	{
		m_pmatBossBoneHead[i] = (*m_pmatBossBone[i]) * (*pmatBossWorld);
	}

	for(int i = 0; i < BOSS_END; ++i)
	{
		D3DXVECTOR3 vBonePos;

		memcpy(&vBonePos, &m_pmatBossBoneHead[i].m[3][0], sizeof(D3DXVECTOR3));

		float fDistance = D3DXVec3Length( &(vBonePos - pPlayerInfo->m_vPos));
		float fCollisionRange;

		if(i == BOSS_NECK1)
			fCollisionRange = 300.f;
		if(i == BOSS_NECK2)
			fCollisionRange = 400.f;
		if(i == BOSS_SPINE_1)
			fCollisionRange = 275.f;
		if(i == BOSS_SPINE_2)
			fCollisionRange = 275.f;
		else
			fCollisionRange = 100.f;

		if(fDistance < fCollisionRange)
		{
			pPlayerInfo->m_vPos += pPlayerInfo->m_vDir * -1 * pPlayerInfo->m_fSpeed * fTime;
		}
		
	}
	
}


void CCollisionMgr::Collision_BossStateCheck()
{

	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");

	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

	float fDistance = D3DXVec3Length( &(pBossInfo->m_vPos - pPlayerInfo->m_vPos));

	int iRand = rand() % 2;
	
	if( fDistance > 1000					// Dash
		&& fDistance < 1300)
	{

		if(pBossInfo->m_bBossTurnRight
			|| pBossInfo->m_bBossTurnLeft
			|| pBossInfo->m_bBossFly
			|| pBossInfo->m_bBossFlyLoop
			|| pBossInfo->m_bBossFlyEnd
			|| pBossInfo->m_bBossFire
			|| pBossInfo->m_bBossFireLoop
			|| pBossInfo->m_bBossFireEnd) 
			return;
		pBossInfo->m_bBossDash = true;
	}


}

void CCollisionMgr::Collision_BossBite()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);
	
	if(pBossInfo->m_bBossRightAtt 
		|| pBossInfo->m_bBossLeftAtt
		|| pBossInfo->m_bBossDash
		|| pBossInfo->m_bBossTurnRight
		|| pBossInfo->m_bBossTurnLeft
		|| pBossInfo->m_bBossFly
		|| pBossInfo->m_bBossFlyLoop
		|| pBossInfo->m_bBossFlyEnd
		|| pBossInfo->m_bBossFire
		|| pBossInfo->m_bBossFireLoop
		|| pBossInfo->m_bBossFireEnd)
		return;
	D3DXVECTOR3 vBonePos;

	memcpy(&vBonePos, &m_pmatBossBoneHead[BOSS_HEAD].m[3][0], sizeof(D3DXVECTOR3));

	float fDistance = D3DXVec3Length( &(vBonePos - pPlayerInfo->m_vPos));

	if(fDistance < 200.f)
	{
		if(pBossInfo->m_bBossBite)
			return;
		pBossInfo->m_bBossBite = true;
	}
}

void CCollisionMgr::Collision_BossRightAtt()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

	if(pBossInfo->m_bBossLeftAtt 
		|| pBossInfo->m_bBossBite
		|| pBossInfo->m_bBossDash
		|| pBossInfo->m_bBossTurnRight
		|| pBossInfo->m_bBossTurnLeft
		|| pBossInfo->m_bBossFly
		|| pBossInfo->m_bBossFlyLoop
		|| pBossInfo->m_bBossFlyEnd
		|| pBossInfo->m_bBossFire
		|| pBossInfo->m_bBossFireLoop
		|| pBossInfo->m_bBossFireEnd)
		return;

	D3DXVECTOR3 vBonePos;

	memcpy(&vBonePos, &m_pmatBossBoneHead[BOSS_L_HAND].m[3][0], sizeof(D3DXVECTOR3));

	float fDistance = D3DXVec3Length( &(vBonePos - pPlayerInfo->m_vPos));

	if(fDistance < 200.f)
	{
		if(pBossInfo->m_bBossRightAtt)
			return;
		pBossInfo->m_bBossRightAtt = true;
	}
}

void CCollisionMgr::Collision_BossLeftAtt()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

	if(pBossInfo->m_bBossRightAtt 
		|| pBossInfo->m_bBossBite
		|| pBossInfo->m_bBossDash
		|| pBossInfo->m_bBossTurnRight
		|| pBossInfo->m_bBossTurnLeft
		|| pBossInfo->m_bBossFly
		|| pBossInfo->m_bBossFlyLoop
		|| pBossInfo->m_bBossFlyEnd
		|| pBossInfo->m_bBossFire
		|| pBossInfo->m_bBossFireLoop
		|| pBossInfo->m_bBossFireEnd)
		return;

	D3DXVECTOR3 vBonePos;

	memcpy(&vBonePos, &m_pmatBossBoneHead[BOSS_R_HAND].m[3][0], sizeof(D3DXVECTOR3));

	float fDistance = D3DXVec3Length( &(vBonePos - pPlayerInfo->m_vPos));

	if(fDistance < 200.f)
	{
		if(pBossInfo->m_bBossLeftAtt)
			return;
		pBossInfo->m_bBossLeftAtt = true;
	}
}



void CCollisionMgr::Coliision_BossToPlayerKnockBack()
{
	const Engine::CComponent* pPlayerComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	const Engine::CComponent* pBossComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Boss", L"Transform");

	Engine::CTransform* pPlayerInfo = ((Engine::CTransform*)pPlayerComponent);
	Engine::CTransform* pBossInfo = ((Engine::CTransform*)pBossComponent);

	float fDistance = D3DXVec3Length( &(pBossInfo->m_vPos - pPlayerInfo->m_vPos));

	if(fDistance < 600 && pBossInfo->m_bAttackCasting == true)
	{
		pBossInfo->m_bAttRange = true;
	}
	else
	{
		pBossInfo->m_bAttRange = false;
	}
}


void CCollisionMgr::Create_Sphere(LPDIRECT3DDEVICE9 pDevice)
{
}

void CCollisionMgr::Render_BossSphere()
{

}

void CCollisionMgr::Collision_Attack_RangeSet()
{
	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vMonsterPos = ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos;
		float fDistance = D3DXVec3Length( &(vPlayerPos - vMonsterPos));

		if(fDistance < 200)
		{
			((Engine::CTransform*)pComponent)->m_bPlayerAttRange = true;
			((Engine::CTransform*)pComponent)->m_bAttRange = true;
			break;
		}
		else
		{
			((Engine::CTransform*)pComponent)->m_bPlayerAttRange = false;
			((Engine::CTransform*)pComponent)->m_bAttRange = false;
		}
	}
}

void CCollisionMgr::Collision_PlayerAttack_Normal()
{
	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vMonsterPos = ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos;
		float fDistance = D3DXVec3Length( &(vPlayerPos - vMonsterPos));

		if(fDistance < 200
			&& ((Engine::CTransform*)pComponent)->m_bAttackCasting == true)
		{
			CSoundMgr::GetInstance()->PlayerHitSound(L"Hit.wav");
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bNormalHit = true;
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bCreateHitPart = true;
		}
		else
		{
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bNormalHit = false;
		}
	}
}

void CCollisionMgr::Collision_PlayerAttack_KnockBack()
{
	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");

	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	list<Engine::CGameObject*>::iterator iter = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::Get_Management()->GetObjList(CStage::LAYER_GAMELOGIC, L"Monster")->end();

	for(; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vMonsterPos = ((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos;
		float fDistance = D3DXVec3Length( &(vPlayerPos - vMonsterPos));

		if(fDistance < 200
			&& ((Engine::CTransform*)pComponent)->m_bAttackCasting == true)
		{
			CSoundMgr::GetInstance()->MonsterDownSound(L"Down.wav");
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bBounceHit = true;
		}
		else
		{
			((Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_bBounceHit = false;
		}
	}

}

bool CCollisionMgr::Collision_PlayerToGate()
{
	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Door", L"Transform");
	D3DXVECTOR3 vDoorPos = ((Engine::CTransform*)pComponent)->m_vPos;

	float fDistance = D3DXVec3Length( &(vPlayerPos - vDoorPos));

	if(fDistance < 300)
	{
		return true;
	}
	return false;
}

void CCollisionMgr::Collision_PlayerToGate_MoveCheck()
{
	float	fTime = Engine::Get_TimeMgr()->GetTime();

	const Engine::CComponent* pComponent = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");
	D3DXVECTOR3 vPlayerPos = ((Engine::CTransform*)pComponent)->m_vPos;

	const Engine::CComponent* pComponent2 = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Door", L"Transform");
	D3DXVECTOR3 vDoorPos = ((Engine::CTransform*)pComponent2)->m_vPos;

	float fDistance = D3DXVec3Length( &(vPlayerPos - vDoorPos));

	if(fDistance < 200)
	{
		if(((Engine::CTransform*)pComponent2)->m_bClose)
		{
			((Engine::CTransform*)pComponent)->m_vPos += ((Engine::CTransform*)pComponent)->m_vDir * -1 * ((Engine::CTransform*)pComponent)->m_fSpeed * fTime;
		}

	}
}

void CCollisionMgr::Init_Point(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax)
{
	pOBB->vPoint[0] = D3DXVECTOR3(pMin->x, pMax->y, pMin->z);
	pOBB->vPoint[1] = D3DXVECTOR3(pMax->x, pMax->y, pMin->z);
	pOBB->vPoint[2] = D3DXVECTOR3(pMax->x, pMin->y, pMin->z);
	pOBB->vPoint[3] = D3DXVECTOR3(pMin->x, pMin->y, pMin->z);

	pOBB->vPoint[4] = D3DXVECTOR3(pMin->x, pMax->y, pMax->z);
	pOBB->vPoint[5] = D3DXVECTOR3(pMax->x, pMax->y, pMax->z);
	pOBB->vPoint[6] = D3DXVECTOR3(pMax->x, pMin->y, pMax->z);
	pOBB->vPoint[7] = D3DXVECTOR3(pMin->x, pMin->y, pMax->z);

	for(int i = 0; i < 8; ++i)
		pOBB->vCenter += pOBB->vPoint[i];

	pOBB->vCenter *= 0.125;
}

void CCollisionMgr::Init_Axis(OBB* pOBB)
{
	// +x
	pOBB->vProjAxis[0] = (pOBB->vPoint[1] + pOBB->vPoint[2] + pOBB->vPoint[5] + pOBB->vPoint[6]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[0] = pOBB->vPoint[1] - pOBB->vPoint[0];

	//+y
	pOBB->vProjAxis[1] = (pOBB->vPoint[0] + pOBB->vPoint[1] + pOBB->vPoint[4] + pOBB->vPoint[5]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[1] = pOBB->vPoint[0] - pOBB->vPoint[3];
	//+z
	pOBB->vProjAxis[2] = (pOBB->vPoint[4] + pOBB->vPoint[5] + pOBB->vPoint[6] + pOBB->vPoint[7]) * 0.25f - pOBB->vCenter;
	pOBB->vAxis[2] = pOBB->vPoint[7] - pOBB->vPoint[3];

	for(int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pOBB->vAxis[i], &pOBB->vAxis[i]);
}

HRESULT CCollisionMgr::AddColObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator	iter = m_mapCollision.find(eCollisionID);
	if(iter != m_mapCollision.end())
	{
		MSG_BOX(L"Collision 객체 이미 있어");
		return E_FAIL;
	}
	Engine::CCollision*		pCollision = NULL;

	switch(eCollisionID)
	{
	case COLLISION_AABB:
		pCollision = NULL;
		break;
	}
	NULL_CHECK_RETURN(pCollision, E_FAIL);

	m_mapCollision.insert(MAPCOLLISION::value_type(eCollisionID, pCollision));
	return S_OK;
}

Engine::CCollision* CCollisionMgr::GetColObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator iter = m_mapCollision.find(eCollisionID);
	if(iter == m_mapCollision.end())
		return NULL;

	iter->second->AddRefCnt();
	return iter->second;
}



