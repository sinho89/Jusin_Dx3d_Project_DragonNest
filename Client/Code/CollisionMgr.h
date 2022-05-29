/*!
 * \file CollisionMgr.h
 * \date 2016/09/22 13:47
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"
#include "Include.h"

namespace Engine
{
	class CCollision;
	class CGameObject;
	class CStaticMesh;
	class CLayer;
};

class CCollisionMgr
{
public:
	DECLARE_SINGLETON(CCollisionMgr)

private:
	typedef struct tagOBB
	{
		D3DXVECTOR3		vPoint[8];
		D3DXVECTOR3		vCenter;
		D3DXVECTOR3		vProjAxis[3];
		D3DXVECTOR3		vAxis[3];
	}OBB;

private:
	CCollisionMgr(void);
	~CCollisionMgr(void);

private:
	const D3DXMATRIX* m_pmatBossBone[BOSS_END];
	D3DXMATRIX m_pmatBossBoneHead[BOSS_END];
public:
	HRESULT AddColObject(COLLISIONID eCollisionID);
	Engine::CCollision* GetColObject(COLLISIONID eCollisionID);

public:
	void Create_Sphere(LPDIRECT3DDEVICE9 pDevice);
	void Render_BossSphere();

public:
	bool Collision_AABB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax
		, const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax);
	bool Collision_OBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax, const D3DXMATRIX* pDestWorld
		, const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax, const D3DXMATRIX* pSourWorld);

	bool Collision_Camera(LPD3DXMESH* pMesh, const D3DXMATRIX* pWorldMatrix);

	void Collision_MonsterToPlayer_MoveCheck();
	void Collision_MonsterToPlayer_SightCheck();
	void Coliision_MonsterToPlayer_AttackRangeCheck();
	void Collision_MonsterToMonster_MoveCheck();
	void Collision_BossToPlayer();
	void Collision_BossStateCheck();
	void Coliision_BossToPlayerKnockBack();
	void Collision_BossBite();
	void Collision_BossRightAtt();
	void Collision_BossLeftAtt();
	void ReviveMonster();

public: // Attack
	void Collision_Attack_RangeSet();
	void Collision_PlayerAttack_Normal();
	void Collision_PlayerAttack_KnockBack();

public: // GateCollision
	bool Collision_PlayerToGate();
	void Collision_PlayerToGate_MoveCheck();
private:
	void Init_Point(OBB* pOBB, const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax);
	void Init_Axis(OBB* pOBB);

private:
	OBB			m_OBB[2];

private:
	typedef map<COLLISIONID, Engine::CCollision*>		MAPCOLLISION;
	MAPCOLLISION			m_mapCollision;
};

#endif // CollisionMgr_h__