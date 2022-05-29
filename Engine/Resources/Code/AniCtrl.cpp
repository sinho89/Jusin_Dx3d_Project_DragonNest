#include "AniCtrl.h"

Engine::CAniCtrl::CAniCtrl(void)
: m_pAniCtrl(NULL)
, m_iMaxAniSet(0)
, m_iCurrentTrack(0)
, m_iNewTrack(1)
, m_dAccTime(0.0)
, m_dPeriod(0.0)
{

}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
: m_pAniCtrl(NULL)
, m_iMaxAniSet(0)
, m_iCurrentTrack(0)
, m_iNewTrack(1)
, m_dAccTime(0.0)
, m_dPeriod(0.0)
{
	rhs.m_pAniCtrl->CloneAnimationController(
		rhs.m_pAniCtrl->GetMaxNumAnimationOutputs()
		, rhs.m_pAniCtrl->GetMaxNumAnimationSets()
		, rhs.m_pAniCtrl->GetMaxNumTracks()
		, rhs.m_pAniCtrl->GetMaxNumEvents()
		, &m_pAniCtrl);

	m_iMaxAniSet = rhs.m_pAniCtrl->GetMaxNumAnimationSets();
}

Engine::CAniCtrl::~CAniCtrl(void)
{
	Release();
}

void Engine::CAniCtrl::SetAnimationSet(UINT iAniIdx, FLOAT fWeight, DOUBLE dDuration)
{
	if(iAniIdx >= m_iMaxAniSet)
		return ;

	if(m_iCurrentTrack == 0)
		m_iNewTrack = 1;
	else
		m_iNewTrack = 0;

	LPD3DXANIMATIONSET		pAS = NULL;
	m_pAniCtrl->GetAnimationSet(iAniIdx, &pAS);

	m_dPeriod = pAS->GetPeriod();

	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);
	pAS->Release();

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + dDuration);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.1f, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, fWeight, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.1f, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 10.f - fWeight, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);

	m_dAccTime = 0.0;
	m_iCurrentTrack = m_iNewTrack;
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_pAniCtrl->ResetTime();
}

void Engine::CAniCtrl::FrameMove(const float& fTimePerSec)
{
	m_pAniCtrl->AdvanceTime(fTimePerSec, NULL);
	m_dAccTime += DOUBLE(fTimePerSec);
}

void Engine::CAniCtrl::Release(void)
{
	Safe_Release(m_pAniCtrl);
}

LPD3DXANIMATIONCONTROLLER* Engine::CAniCtrl::GetAniCtrl(void)
{
	return &m_pAniCtrl;
}

DOUBLE Engine::CAniCtrl::GetTrackPos(void)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	return TrackInfo.Position;
}

DOUBLE Engine::CAniCtrl::GetPeriod(void)
{
	return m_dPeriod;
}

void Engine::CAniCtrl::SetTrackPos(DOUBLE dPostion)
{
	m_pAniCtrl->SetTrackPosition(m_iCurrentTrack, dPostion);
}

