#include "Transform.h"

Engine::CTransform::CTransform(const D3DXVECTOR3& vLook)
: m_vScale(1.f, 1.f, 1.f)
, m_vPos(0.f, 0.f, 0.f)
, m_vKnockBackDir(0.f, 0.f, 0.f)
, m_vOriPos(0.f, 0.f, 0.f)
, m_vOriDir(0.f, 0.f, 0.f)
, m_fOriAngle(0.f)
, m_bRevive(false)
, m_bCreateHitPart(false)
, m_vDir(vLook)
, m_bPart(false)
, m_bAlive(true)
, m_bSight(false)
, m_bAttackSight(false)
, m_bSkillRender(false)
, m_fSpeed(0.f)
, m_fAniTime(0.f)
, m_fCresentCreateTime(0.f)
, m_fHarfMoonCreateTime(0.f)
, m_fGreatCreateTime(0.f)
, m_bPlayerFrontCheck(false)
, m_bPlayerBackCheck(false)
, m_bPlayerLeftCheck(false)
, m_bPlayerRightCheck(false)
, m_bCreateHarfMoon(false)
, m_bCreateCresent(false)
, m_bCreateGreat(false)
, m_bKnockBackHit(false)
, m_bNormalHit(false)
, m_bBounceHit(false)
, m_bNormalAtt(false)
, m_bBounceAtt(false)
, m_bPlayerAttRange(false)
, m_bAttRange(false)
, m_bPlayerDamRange(false)
, m_bOrgeDamRange(false)
, m_bAttackCasting(false)
, m_bMove(false)
, m_bClose(false)
, m_bClosing(false)
, m_bOpen(false)
, m_bOpening(false)
, m_iHp(0)
, m_iNormalAtt(0)
, m_iBounceAtt(0)
, m_iKillCount(0)
, m_bDownGate(false)
, m_bUpGate(false)
, m_bTownCulling(false)
, m_bBridgeCulling(true)
, m_bBossDash(false)
, m_bBossKnockBackHit(false)
, m_bBossBite(false)
, m_bBossRightAtt(false)
, m_bBossLeftAtt(false)
, m_bBossTurnRight(false)
, m_bBossTurnLeft(false)
, m_bBossFly(false)
, m_bBossFlyLoop(false)
, m_bBossFlyEnd(false)
, m_bBossFire(false)
, m_bBossFireLoop(false)
, m_bBossFireEnd(false)
, m_bSkillHit(false)
, m_bCreateHarfCasting(false)
, m_bCreateCresentCasting(false)
, m_bCreateGreatCasting(false)
, m_bWindArrive(false)
, m_bCreateParticle(false)
, m_fHitTime(0.f)
, m_bHit(false)
{
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matBoneMouse);
}

Engine::CTransform::~CTransform(void)
{

}

void Engine::CTransform::Update(void)
{
	D3DXMATRIX				matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRotX, m_fAngle[ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_fAngle[ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle[ANGLE_Z]);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
}

Engine::CTransform* Engine::CTransform::Create(const D3DXVECTOR3& vLook)
{
	return new CTransform(vLook);
}

DWORD Engine::CTransform::Release(void)
{
	return 0;
}
