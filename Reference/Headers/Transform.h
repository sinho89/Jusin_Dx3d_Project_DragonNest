/*!
 * \file Transform.h
 * \date 2015/04/05 8:37
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
#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform
	: public CComponent
{
private:
	explicit CTransform( const D3DXVECTOR3& vLook );

public:
	virtual ~CTransform(void);

public:
	virtual void Update(void);
	virtual DWORD Release(void);

public:
	static CTransform* Create(const D3DXVECTOR3& vLook);

public:
	bool				m_bPart;
	bool				m_bAlive;
	bool				m_bRevive;
	float				m_fAngle[ANGLE_END];
	float				m_fSpeed;
	float				m_fAniTime;
	float				m_fOriAngle;
	COLORTYPE			m_eColorType;
	D3DXVECTOR3			m_vScale;
	D3DXVECTOR3			m_vPos;
	D3DXVECTOR3			m_vOriPos;
	D3DXVECTOR3			m_vDir;
	D3DXVECTOR3			m_vOriDir;
	D3DXVECTOR3			m_vKnockBackDir;
	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matBoneMouse;			

	bool				m_bHit;
	float				m_fHitTime;
	float				m_fHarfMoonCreateTime;
	float				m_fCresentCreateTime;
	float				m_fGreatCreateTime;
	int					m_iAttackCount;
	bool				m_bCreateHitPart;
	bool				m_bCreateHarfMoon;
	bool				m_bCreateCresent;
	bool				m_bCreateGreat;
	bool				m_bSight;
	bool				m_bAttackSight;
	bool				m_bPlayerFrontCheck;
	bool				m_bPlayerBackCheck;
	bool				m_bPlayerLeftCheck;
	bool				m_bPlayerRightCheck;
	bool				m_bKnockBackHit;
	bool				m_bBossKnockBackHit;
	bool				m_bNormalHit;
	bool				m_bBounceHit;
	bool				m_bNormalAtt;
	bool				m_bBounceAtt;

	bool				m_bPlayerAttRange;
	bool				m_bAttRange;
	bool				m_bPlayerDamRange;
	bool				m_bOrgeDamRange;
	bool				m_bAttackCasting;

	bool				m_bMove;
	bool				m_bClose;
	bool				m_bClosing;
	bool				m_bOpen;
	bool				m_bOpening;

	int					m_iHp;
	int					m_iNormalAtt;
	int					m_iBounceAtt;

	int					m_iKillCount;
	bool				m_bDownGate;
	bool				m_bUpGate;

	bool				m_bTownCulling;
	bool				m_bBridgeCulling;

	bool				m_bBossDash;
	bool				m_bBossBite;
	bool				m_bBossRightAtt;
	bool				m_bBossLeftAtt;
	bool				m_bBossTurnRight;
	bool				m_bBossTurnLeft;
	bool				m_bBossFly;
	bool				m_bBossFlyLoop;
	bool				m_bBossFlyEnd;
	bool				m_bBossFire;
	bool				m_bBossFireLoop;
	bool				m_bBossFireEnd;

	bool				m_bSkillRender;
	bool				m_bSkillHit;
	bool				m_bCreateHarfCasting;
	bool				m_bCreateCresentCasting;
	bool				m_bCreateGreatCasting;
	bool				m_bWindArrive;
	bool				m_bCreateParticle;
};

END

#endif // Transform_h__