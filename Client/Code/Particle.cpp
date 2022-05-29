#include "stdafx.h"
#include "Particle.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pTargetInfo(NULL)
, m_fTime(0.f)
{

}

CParticle::~CParticle()
{

}
HRESULT CParticle::Initialize(void)
{
	return S_OK;
}
void CParticle::Update(void)
{
	
}

void CParticle::Render(void)
{

}
/*int CParticleObject::SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2)
{
	FLOAT	v1, v2;

	v1 = p1->PrsZ;
	v2 = p2->PrsZ;

	if(v1 < v2)
		return 1;

	else if(v1 == v2)
		return 0;

	else 
		return -1;
}*/
void CParticle::SetAni(BOOL bAni/*=TRUE*/)
{

}

void CParticle::SetAvgTime(FLOAT fTime)
{

}

void CParticle::SetPart(int nIdx)
{

}



void CParticle::Release(void)
{

}
